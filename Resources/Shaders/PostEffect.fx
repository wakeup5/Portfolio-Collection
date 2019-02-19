
float pixelSizeU;			//
float pixelSizeV;
float pixelHalfSizeU;
float pixelHalfSizeV;

struct VS_INPUT{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};


VS_OUTPUT vs_Base(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.pos = Input.pos;
	Output.uv = Input.uv;

	return Output;
}


// Base ///////////////////////////////////////////////////////////////

//Sampler
texture screenTex;
sampler2D screenSampler = sampler_state{
	Texture = screenTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	
};

float4 ps_Base( VS_OUTPUT Input ) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	return tex2D( screenSampler, Input.uv );

	//return float4(tex2D(screenSampler, Input.uv).www, 1);

}

// OutLine ///////////////////////////////////////////////////////////////

//Sampler
texture normalTex;
sampler2D normalSampler = sampler_state{
	Texture = normalTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};


float mask[9] = {
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1 };		//라플라스 필터

float coordX[3] = { -1, 0, 1 };
float coordY[3] = { -1, 0, 1 };


float4 ps_Outline(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float outlineStrength = 0.0f;

	float pX = pixelSizeU;
	float pY = pixelSizeV;

	//자신을 포함한 주변 픽셀정보 9개 
	for (int i = 0; i < 9; i++)
	{
		float2 uv = Input.uv + float2(coordX[i % 3] * pX, coordY[i / 3] * pY);

			float3 normal = tex2D(normalSampler, uv).rgb * mask[i];

		outlineStrength += normal.x;
		outlineStrength += normal.y;
		outlineStrength += normal.z;
	}

	outlineStrength = saturate( abs( outlineStrength ) );
	
	float4 outLineColor = float4(0, 0, 0, 1);
	float4 baseColor = tex2D(screenSampler, Input.uv);

	float4 finalColor = lerp(baseColor, outLineColor, outlineStrength);

	return finalColor;
}

// BlurX ///////////////////////////////////////////////////////////////

float BlurWeights[13] =
{
	0.002216,		// -6
	0.008764,       // -5
	0.026995,       // -4
	0.064759,       // -3
	0.120985,       // -2
	0.176033,       // -1
	0.199471,		//Center
	0.176033,		//	1
	0.120985,		//	2
	0.064759,		//	3
	0.026995,		//	4
	0.008764,		//	5
	0.002216		//	6
};

float bulrPos[13] =
{
	-6,		
	-5,     
	-4,     
	-3,     
	-2,     
	-1,     
	0,		
	1,		
	2,		
	3,		
	4,		
	5,		
	6
};

float blurScale = 1.0f;			//블러스케일

float4 ps_BlurX(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 13; i++)
	{
		float2 uv = Input.uv + float2(bulrPos[i] * pixelSizeU, 0) * blurScale;

		//uv = saturate(uv);
		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		finalColor += tex2D(screenSampler, uv) * BlurWeights[i];
	}

	return finalColor;
	/*
	//float depth = tex2D(normalSampler, Input.uv).a;

	float r = 1;
	//if (min(1, d.a) > 0.001)
	//{
	//	r = 1;
	//}
	//else
	//{
	//	r = 0.4;
	//}

	float rs = ceil(r * 2.57);

	*/
	/*
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);
	*/

	//float4 In = tex2D(t1, float2(0, 0));
	/*
	float4 val = 0;
	float wsum = 0;

	float ci = (2 * r * r);
	float pici = (3.141592f * ci);

	for (float iy = -rs; iy < rs + 1; iy += 1)
	{
		for (float ix = -rs; ix < rs + 1; ix += 1)
		{
			float x = (Input.uv.x + (2.f * ix) / 1024.f);
			float y = (Input.uv.y + (2.f * iy) / 800.f);

			float dsp = (ix)* (ix) + (iy)* (iy);
			float wght = exp(-dsp / ci) / pici;

			float4 In = tex2D(screenSampler, float2(x, y));

			val += In * wght;
			wsum += wght;
		}
	}

	return (val / wsum);
	*/
}

float4 ps_BlurY(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 13; i++)
	{
		float2 uv = Input.uv + float2(0, bulrPos[i] * pixelSizeV) * blurScale;
		//uv = saturate(uv);
		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		finalColor += tex2D(screenSampler, uv) * BlurWeights[i];
	}

	return finalColor;
}
float4 ps_Blur(VS_OUTPUT Input) : COLOR0
{
	return (ps_BlurX(Input) + ps_BlurY(Input)) / 2;
}



float4 ps_BlurSunlight(VS_OUTPUT Input) : COLOR0
{
	float r = 3;
	//체크
	//float4 check1 = tex2D(screenSampler, Input.uv);
	//float4 check2 = tex2D(screenSampler, float2((Input.uv.x + (2.f * -r) / 1024.f), (Input.uv.y + (2.f * -r) / 800.f)));
	//float4 check3 = tex2D(screenSampler, float2((Input.uv.x + (2.f * -r) / 1024.f), (Input.uv.y + (2.f * r) / 800.f)));
	//float4 check4 = tex2D(screenSampler, float2((Input.uv.x + (2.f * r) / 1024.f), (Input.uv.y + (2.f * -r) / 800.f)));
	//float4 check5 = tex2D(screenSampler, float2((Input.uv.x + (2.f * r) / 1024.f), (Input.uv.y + (2.f * r) / 800.f)));

	//if (check1.a == 0 && check2.a == 0 && check3.a == 0 && check4.a == 0 && check5.a == 0) return 0;
	

	float rs = ceil(r * 2.57);

	float4 val = 0;
	float wsum = 0;

	float ci = (2 * r * r);
	float pici = (3.141592f * ci);

	for (float iy = -rs; iy < rs + 1; iy += 1)
	{
		for (float ix = -rs; ix < rs + 1; ix += 1)
		{
			float x = (Input.uv.x + (2.f * ix) / 1024.f);
			float y = (Input.uv.y + (2.f * iy) / 800.f);

			float dsp = (ix)* (ix)+(iy)* (iy);
			float wght = exp(-dsp / ci) / pici;

			float4 In = tex2D(screenSampler, float2(x, y));

			if (In.r != 0)
			{
				val += In * wght;
				wsum += wght;
			}
			else
			{
				//val += float4(0.5, 0.5, 0.5, 0.5) * wght;
				wsum += wght / 50;
			}
			//wsum += wght;
		}
	}

return (val / wsum);
}

// BBO ///////////////////////////////////////////////////////////////


//Sampler
texture blurTex;
sampler2D blurSampler = sampler_state{
	Texture = blurTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

float blurAmout = 3.0f;

float4 ps_BBO(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;
	
	float4 orignalTex = tex2D(screenSampler, Input.uv);
	float4 blurTex = tex2D(blurSampler, Input.uv);

	return orignalTex + blurTex * blurAmout;
}

// Depth Of Field ///////////////////////////////////////////////////////////////

float focus = 0.01f;
float fieldRange = 5.0f;

float4 ps_DepthOfField(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 orignalTex = tex2D(screenSampler, Input.uv);
	float4 blurTex = tex2D(blurSampler, Input.uv);
	float depth = tex2D(normalSampler, Input.uv).a;


	return lerp(orignalTex, blurTex, saturate(fieldRange * abs(focus - depth))) + blurTex * 0.15;
}

// Gray ///////////////////////////////////////////////////////////////

float4 ps_Gray(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 texColor = tex2D(screenSampler, Input.uv);

	float gray = texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f;
	return float4(gray, gray, gray, 1);
}



// ColorLevel ///////////////////////////////////////////////////////////////

float InBlack = 0.0f;			//컬러를 0 ~ 255 로 볼때 이값 미만이면 0 으로 처리한다.
float InWhite = 255.0f;			//컬러를 0 ~ 255 로 볼때 이값 이상이면 1 으로 처리한다.
float Gamma = 3.0f;				//증폭 
float OutBlack = 0.0f;			//감마 처리 이후 얻어진 값을 0 ~ 255 로 볼때 이값 미만이면 0 으로 처리한다.
float OutWhite = 255.0f;		//감마 처리 이후 얻어진 값을 0 ~ 255 로 볼때 이값 이상이면 1 으로 처리한다.



float LevelSet(float value){

	// 0 ~ 1 사이의 범위 의 컬러 값을 0 ~ 255 범위로...
	float c = value * 255.0f;

	//0 밑으로 안내려 가게....
	c = max(0, c - InBlack);

	//컬러 값 InBlack 에서 InWhite 에 대한 범위의 Factor 값 
	//이과정에서 c 는 0 ~ 1 사이의 값이 된다.
	c = saturate(c / ( InWhite - InBlack));

	//saturate ( value )  =  value 값은 0 ~ 1 사이의 값으로 Clamp 된다.

	//감마 처리 
	c = pow(c, Gamma);

	//컬러 Out 처리 	( 여기서 다시 0 ~ 255 의 컬러 범위 값이 된다 )	
	// c = 0.5
	// OutBlack = 100
	// OutWhite = 200
	// 최종값은 = 150 
	c = c * ( OutWhite - OutBlack) +  OutBlack;

	//최종 컬러 
	c = saturate(c / 255.0f);

	return c;
}


float4 ps_ColorLevel(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 texColor = tex2D(screenSampler, Input.uv);

	float4 finalColor;
	finalColor.r = LevelSet(texColor.r);
	finalColor.g = LevelSet(texColor.g);
	finalColor.b = LevelSet(texColor.b);
	finalColor.a = 1.0f;

	return finalColor;
}


// motionBlur ///////////////////////////////////////////////////////////////

//Sampler

texture screenTex1;
sampler2D screenSampler1 = sampler_state {
	Texture = screenTex1;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture screenTex2;
sampler2D screenSampler2 = sampler_state {
	Texture = screenTex2;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture screenTex3;
sampler2D screenSampler3 = sampler_state {
	Texture = screenTex3;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture screenTex4;
sampler2D screenSampler4 = sampler_state {
	Texture = screenTex4;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

float4 ps_MotionBlur(VS_OUTPUT Input) : COLOR0
{
	float4 t1 = tex2D(screenSampler1, Input.uv);
	float4 t2 = tex2D(screenSampler2, Input.uv);
	float4 t3 = tex2D(screenSampler3, Input.uv);
	float4 t4 = tex2D(screenSampler4, Input.uv);

	float4 tf = t1 * 0.1 + t2 * 0.2 + t3 * 0.3 + t4 * 0.4;
	float4 t = tex2D(screenSampler, Input.uv);

	float4 output = (t + tf) / 2;// -t * tf;

	float4 orignalTex = tex2D(screenSampler, Input.uv);
	float depth = tex2D(normalSampler, Input.uv).a;

	return lerp(orignalTex, output, saturate(fieldRange * abs(focus - depth)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////6star
float	m_TexW = 640;
float	m_TexH = 400;

texture m_TxDif;
sampler smpDif = sampler_state
{
	texture = <m_TxDif>;
	AddressU = Wrap;
	AddressV = Wrap;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

texture m_TxDif2;
sampler smpDif2 = sampler_state
{
	texture = <m_TxDif2>;
	AddressU = Wrap;
	AddressV = Wrap;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

// 3. 6 Increasing pixel to 6 direction
static const int MAX_SAMP = 14;
float4 m_StarVal[MAX_SAMP];
float	m_StarPow = 1.f;
float4 m_StarColor;

float4 PxlStar(VS_OUTPUT In) : COLOR0
{
	float4	Out = 0;
	float4	t = 0;
	float2	uv = 0;

	//	for(int i=0; i<MAX_SAMP; ++i)
	//	{
	//		uv = In.Tex + float2(0, i*16.f/m_TexW);
	//		Out += tex2D(smpDif, uv) * exp(-i*i/60);
	//	}

	for (int i = 0; i<MAX_SAMP; ++i)
	{
		//r
		uv = In.uv + float2(m_StarVal[i].x, m_StarVal[i].y) * 1.4;

		if (!(uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1))
		{
			Out.r += tex2D(smpDif, uv).r * m_StarVal[i].z;
		}
		//g
		uv = In.uv + float2(m_StarVal[i].x, m_StarVal[i].y) * 1;

		if (!(uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1))
		{
			Out.g += tex2D(smpDif, uv).r * m_StarVal[i].z;
		}
		//b
		uv = In.uv + float2(m_StarVal[i].x, m_StarVal[i].y) * 0.6;

		if (!(uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1))
		{
			Out.b += tex2D(smpDif, uv).r * m_StarVal[i].z;
		}
	}

	//	for(int i=0; i<16; ++i)
	//	{
	//		uv = In.Tex + float2(m_StarVal[i].x, m_StarVal[i].y);
	//		Out += tex2D(smpDif, uv) * m_StarVal[i].z;
	//	}

	Out *= m_StarPow;
	Out.w = 1;
	return  Out;
}

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);
sampler s5 : register(s5);
sampler s6 : register(s6);
sampler s7 : register(s7);

float4 PxlStarRGB(VS_OUTPUT In) : COLOR0
{
	float4	Out = 0;

	Out += tex2D(s0, In.uv);
	Out += tex2D(s1, In.uv);
	Out += tex2D(s2, In.uv);

	Out.w = 1;
	return Out;
}

// 4. merge pixels
float4 PxlStarAll(VS_OUTPUT In) : COLOR0
{
	float4	Out = 0;

	Out += tex2D(s0, In.uv);
	Out += tex2D(s1, In.uv);
	Out += tex2D(s2, In.uv);
	Out += tex2D(s3, In.uv);
	Out += tex2D(s4, In.uv);
	Out += tex2D(s5, In.uv);

	Out.w = 1;
	return Out;
}

/////////////rain
struct SvsOut
{
	float4 Pos : POSITION;
	float4 Dif : COLOR0;
	float2 Tex : TEXCOORD0;
	float3 Nor : TEXCOORD7;
};

float4x4       m_mtWVP;       // World * View * Projection
float4x4       m_mtWV;       // World * View

SvsOut VtxPrcRfc(float3 Pos : POSITION
	, float4 Dif : COLOR0
	, float3 Nor : NORMAL0
	, float2 Tex : TEXCOORD0)
{
	SvsOut Out = (SvsOut)0;
	/*
	float3 N = normalize(mul(Nor, (float3x3)m_mtWV));
	Out.Pos = Pos;// mul(float4(Pos, 1), m_mtWVP);
	Out.Tex = Tex;
	Out.Nor = N;
	*/
	
	return Out;
}

float4 PxlPrcRfc(SvsOut In) : COLOR0
{
	float4  Out = 1;
	float3  Nor = normalize(In.Nor);

	//Out = tex2D(smp0, In.Tex);
	Out.r = (Nor.x + 1)*0.5;
	Out.g = (Nor.y + 1)*0.5;
	Out.b = 0.5f;

	return float4(0.5, 0.5, 0.5, 0);
}


static float   g_Dsp = 0.1;

float4 PxlPrcDst(VS_OUTPUT In) : COLOR0
{
	float4 Out = 0;
	float4 Pert = tex2D(smpDif2, In.uv);

	if (Pert.a == 0) return 0;

	float   x = Pert.x * 2 - 1;
	float   y = Pert.y * 2 - 1;

	x = In.uv.x + (x)* g_Dsp;
	y = In.uv.y + (y)* g_Dsp;

	if (x < 0) x = 0;
	if (x > 1) x = 1;
	if (y < 0) y = 0;
	if (y > 1) y = 1;

	Out = tex2D(smpDif, float2(x, y));
	Out *= 1.5f;
	return Out;
}


technique Base
{
    pass p0
    {
		ZWRITEENABLE = FALSE;
        VertexShader = compile vs_3_0 vs_Base();
        PixelShader = compile ps_3_0 ps_Base();
    }
}

technique OutLine
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Outline();
	}
}

technique BlurX
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BlurX();
	}
}

technique BlurY
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BlurY();
	}
}

technique Blur
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Blur();
	}
}


technique BBo
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BBO();
	}
}


technique DepthOfField
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_DepthOfField();
	}
}


technique Gray
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Gray();
	}
}


technique ColorLevel
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_ColorLevel();
	}
}

technique MotionBlur
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_MotionBlur();
	}
}

technique BlurSunlight
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BlurSunlight();
	}
}

technique SunLightMerge
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 PxlStarRGB();
	}
}

technique SunlightStar
{
	// Star
	pass P3
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 PxlStar();
	}

	// Star All
	pass P4
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 PxlStarAll();
	}
}

technique Rain
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 PxlPrcDst();
	}
}

technique RainNormal
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 VtxPrcRfc();
		PixelShader = compile ps_3_0 PxlPrcRfc();
	}
}
