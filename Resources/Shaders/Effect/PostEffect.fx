float pixelSizeU;
float pixelSizeV;
float pixelHalfSizeU;
float pixelHalfSizeV;
float fColorY;
float fColorCb;
float fColorCr;


texture screenTex;
sampler2D screenSampler = sampler_state
{
	Texture = screenTex;
};

texture screenPrevTex;
sampler2D screenPrevSampler = sampler_state
{
	Texture = screenPrevTex;
};

texture screenPPrevTex;
sampler2D screenPPrevSampler = sampler_state
{
	Texture = screenPPrevTex;
};

texture screenPPPrevTex;
sampler2D screenPPPrevSampler = sampler_state
{
	Texture = screenPPPrevTex;
};

texture depthTex;
sampler2D depthSampler = sampler_state
{
	Texture = depthTex;
};


struct VS_INPUT
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
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


float4 ps_Base(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 finalColor = tex2D(screenSampler, Input.uv).rgb;

	return float4(finalColor, 1);
}


float4 ps_Blur(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 finalColor = tex2D(screenSampler, Input.uv).rgb * 0.4f + tex2D(screenPrevSampler, Input.uv).rgb * 0.3f + tex2D(screenPPrevSampler, Input.uv).rgb * 0.2f + tex2D(screenPPPrevSampler, Input.uv).rgb * 0.1f;

	return float4(finalColor, 1);
}


float4 ps_Gray(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 finalColor = tex2D(screenSampler, Input.uv).rgb;

	float gray = finalColor.r * 0.299f + finalColor.g * 0.587f + finalColor.b * 0.114;

	finalColor.r = gray;
	finalColor.g = gray;
	finalColor.b = gray;

	return float4(finalColor, 1);
}


float4 ps_Afterimage(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 finalColor = tex2D(screenSampler, Input.uv).rgb * 0.6f + tex2D(screenPPPrevSampler, Input.uv).rgb * 0.4f;

	return float4(finalColor, 1);
}


float4 ps_Negative(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 finalColor = tex2D(screenSampler, Input.uv).rgb;

	return float4(1 - finalColor.rgb, 1);
}


float4 ps_Binarization(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 finalColor = tex2D(screenSampler, Input.uv).rgb;

	float gray = finalColor.r * 0.299f + finalColor.g * 0.587f + finalColor.b * 0.114;

	if (gray > fColorY)
		return float4(1, 1, 1, 1);
	else
		return float4(0, 0, 0, 1);
}


float4 ps_Sepia(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 finalColor = tex2D(screenSampler, Input.uv).rgb;

	float Y = finalColor.r * 0.299f + finalColor.g * 0.587f + finalColor.b * 0.114;
	float Cb = finalColor.r * 0.595716f - finalColor.g * 0.274453f - finalColor.b * 0.321263f;
	float Cr = finalColor.r * 0.211456f - finalColor.g * 0.522591f + finalColor.b * 0.311135f;

	Y *= fColorY;
	Cb *= fColorCb;
	Cr *= fColorCr;

	finalColor.r = Y + 0.9563f * Cb + 0.621f * Cr;
	finalColor.g = Y - 0.2721f * Cb - 0.6474f * Cr;
	finalColor.b = Y - 1.1070f * Cb + 1.7046 * Cr;

	return float4(finalColor, 1);
}


technique Base
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Base();
	}
}

technique Blur
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Blur();
	}
}

technique Gray
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Gray();
	}
}

technique Afterimage
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Afterimage();
	}
}

technique Negative
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Negative();
	}
}

technique Binarization
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Binarization();
	}
}

technique Sepia
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Sepia();
	}
}


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

float blurScale = 2.0f;

// DOF
float4 ps_DOF(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float3 screenColor = tex2D(screenSampler, Input.uv).rgb;

	float3 depth3 = tex2D(depthSampler, Input.uv).rrr;

	float3 z = depth3.rgb;
	//float a = 100.0f / (100.0f - 0.1f);
	//float b = -0.1f / (100.0f - 0.1f);
	float a = 1.001f;
	float b = -0.001;
	depth3.rgb = b / (z.rgb - a);

	float depth = (depth3.r * 16711680 + depth3.g * 65280 + depth3.b * 255) * 0.000000059604648f;
	depth = saturate(depth);

	float2 centerUV = float2(0.5f, 0.5f);
	float3 centerDepth3 = tex2D(depthSampler, centerUV).rrr;

	z = centerDepth3.rgb;
	centerDepth3.rgb = b / (z.rgb - a);

	float centerDepth = (centerDepth3.r * 16711680 + centerDepth3.g * 65280 + centerDepth3.b * 255) * 0.000000059604648f;
	centerDepth = saturate(centerDepth);

	float3 blurColorX = float3(0, 0, 0);

	// 가로 블러
	for (int i = 0; i < 13; ++i)
	{
		float2 uv = Input.uv + float2(bulrPos[i] * pixelSizeU, 0) * blurScale;

		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		blurColorX += tex2D(screenSampler, uv).rgb * BlurWeights[i];
	}

	// 세로 블러
	float3 blurColorY = float3(0, 0, 0);

	for (int j = 0; j < 13; ++j)
	{
		float2 uv = Input.uv + float2(0, bulrPos[j] * pixelSizeV) * blurScale;

		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		blurColorY += tex2D(screenSampler, uv).rgb * BlurWeights[j];
	}

	float3 blurColor = blurColorX * 0.5f + blurColorY * 0.5f;

	float3 finalColor = lerp(screenColor, blurColor, saturate(3 * abs(depth - centerDepth)));

	return float4(finalColor, 1);
}

technique DOF
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_DOF();
	}
}
