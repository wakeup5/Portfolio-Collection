
//------------------------------------------------------------------------------------
// SkinnedMesh Vertex Processing
//------------------------------------------------------------------------------------

#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 35
#endif

const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x3 amPalette[MATRIX_PALETTE_SIZE_DEFAULT];

struct VS_SKIN_INPUT
{
	float4      Position;
	float3      BlendWeights;
	int4        BlendIndices;
	float3		Normal;
	float3		Binormal;
	float3		Tangent;
};

struct VS_SKIN_OUTPUT
{
	float4		Position;			//월드 변환 끝 정점
	float3		Normal;				//노말 변환 끝
	float3		Binormal;			//바이노말 변환 끝
	float3		Tangent;			//탄젠트 변환 끝
};

VS_SKIN_OUTPUT VS_Skin(VS_SKIN_INPUT Input, int iNumBones)
{
	VS_SKIN_OUTPUT Output = (VS_SKIN_OUTPUT)0;

	float fLastWeight = 1.0;
	float fWeight = 0.0f;
	float afBlendWeights[3] = (float[3])Input.BlendWeights;
	//int aiIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( Input.BlendIndices );
	int aiIndices[4] = (int[4])Input.BlendIndices;

	for (int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++iBone)
	{
		fWeight = afBlendWeights[iBone];
		fLastWeight -= fWeight;
		Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iBone]]) * fWeight;

		Output.Normal += mul(Input.Normal, amPalette[aiIndices[iBone]]) * fWeight;
		Output.Binormal += mul(Input.Binormal, amPalette[aiIndices[iBone]]) * fWeight;
		Output.Tangent += mul(Input.Tangent, amPalette[aiIndices[iBone]]) * fWeight;
	}

	Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;
	Output.Normal += mul(Input.Normal, amPalette[aiIndices[iNumBones - 1]]) * fWeight;
	Output.Binormal += mul(Input.Binormal, amPalette[aiIndices[iNumBones - 1]]) * fWeight;
	Output.Tangent += mul(Input.Tangent, amPalette[aiIndices[iNumBones - 1]]) * fWeight;

	return Output;
}


struct VS_SKIN_INPUT2
{
	float4      Position;
	float3      BlendWeights;
	int4        BlendIndices;
};

struct VS_SKIN_OUTPUT2
{
	float4		Position;			//월드 변환 끝 정점
};

VS_SKIN_OUTPUT2 VS_Skin2(VS_SKIN_INPUT2 Input, int iNumBones)
{
	VS_SKIN_OUTPUT2 Output = (VS_SKIN_OUTPUT2)0;

	float fLastWeight = 1.0;
	float fWeight = 0.0f;
	float afBlendWeights[3] = (float[3])Input.BlendWeights;
	//int aiIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( Input.BlendIndices );
	int aiIndices[4] = (int[4])Input.BlendIndices;

	for (int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++iBone)
	{
		fWeight = afBlendWeights[iBone];
		fLastWeight -= fWeight;
		Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iBone]]) * fWeight;
	}

	Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;

	return Output;
}


struct VS_SKIN_INPUT3
{
	float4      Position;
	float3      BlendWeights;
	int4        BlendIndices;
	float3		Normal;
};

struct VS_SKIN_OUTPUT3
{
	float4		Position;			//월드 변환 끝 정점
	float3		Normal;				//노말 변환 끝
};

VS_SKIN_OUTPUT3 VS_Skin3(VS_SKIN_INPUT3 Input, int iNumBones)
{
	VS_SKIN_OUTPUT3 Output = (VS_SKIN_OUTPUT3)0;

	float fLastWeight = 1.0;
	float fWeight = 0.0f;
	float afBlendWeights[3] = (float[3])Input.BlendWeights;
	//int aiIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( Input.BlendIndices );
	int aiIndices[4] = (int[4])Input.BlendIndices;

	for (int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++iBone)
	{
		fWeight = afBlendWeights[iBone];
		fLastWeight -= fWeight;
		Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iBone]]) * fWeight;

		Output.Normal += mul(Input.Normal, amPalette[aiIndices[iBone]]) * fWeight;
	}

	Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;
	Output.Normal += mul(Input.Normal, amPalette[aiIndices[iNumBones - 1]]) * fWeight;

	return Output;
}







//------------------------------------------------------------------------------------
// SkinnedMesh Base 
//------------------------------------------------------------------------------------

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos;

float4x4 baseDirectionLight;			//기본라이팅 행렬


struct VS_INPUT
{
	float4  Position        : POSITION;
	float3  BlendWeights    : BLENDWEIGHT;
	float4  BlendIndices    : BLENDINDICES;
	float3  Normal          : NORMAL;
	float3  Binormal        : Binormal;
	float3  Tangent         : TANGENT;
	float2  Texcoord        : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4  Position		: POSITION;
	float2  Texcoord        : TEXCOORD0;
	float3  Normal          : TEXCOORD1;
	float3  Binormal        : TEXCOORD2;
	float3  Tangent         : TEXCOORD3;
	float3  viewDir			: TEXCOORD4;
	float3  worldPos		: TEXCOORD6;
};

//버택스 진입
VS_OUTPUT VertSkinning(VS_INPUT Input, uniform int iNumBones)
{
	VS_OUTPUT Output;

	VS_SKIN_INPUT vsi = { Input.Position, Input.BlendWeights, Input.BlendIndices, Input.Normal, Input.Binormal, Input.Tangent };
	VS_SKIN_OUTPUT vso = VS_Skin(vsi, iNumBones);

	//정점 월드 포지션 변경 완료.....
	float4 worldPos = float4(vso.Position.xyz, 1.0f);

		//Input 의 Tangent 와 Binormal 과 Normal 은 
		//VS_SKIN_OUTPUT 구조체 vso 의 다음으로 대체 된다.
		/*
		struct VS_SKIN_OUTPUT
		{
		float4		Position;			//월드 변환 끝 정점
		float3		Normal;				//노말 변환 끝
		float3		Binormal;			//바이노말 변환 끝
		float3		Tangent;			//탄젠트 변환 끝
		};
		*/


		Output.worldPos = worldPos.xyz;
	Output.Position = mul(worldPos, matViewProjection);

	Output.viewDir = vEyePos.xyz - Output.Position.xyz;

	Output.Normal = mul(vso.Normal, (float3x3)matWorld);
	Output.Binormal = mul(vso.Binormal, (float3x3)matWorld);
	Output.Tangent = mul(vso.Tangent, (float3x3)matWorld);

	Output.Texcoord = Input.Texcoord;

	return Output;
}

struct PS_INPUT{
	float2  Texcoord        : TEXCOORD0;
	float3  Normal          : TEXCOORD1;
	float3  Binormal        : TEXCOORD2;
	float3  Tangent         : TEXCOORD3;
	float3  viewDir			: TEXCOORD4;
	float3 worldPos : TEXCOORD5;
};



texture Diffuse_Tex;
sampler2D Diffuse = sampler_state
{
	Texture = (Diffuse_Tex);
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture Normal_Tex;
sampler2D Normal = sampler_state
{
	Texture = (Normal_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

texture Specular_Tex;
sampler2D Specular = sampler_state
{
	Texture = (Specular_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


texture Emission_Tex;
sampler2D Emission = sampler_state
{
	Texture = (Emission_Tex);
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

float fSpecPower;

// 라이트 행렬
// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
// Dir.x,         Dir.y,          Dir.z,         DistancePow;
// Color.r,       Color.g,        Color.b,       strength
// MaxRange,      MinRange,       InAngle        OutAngle 
float4x4 matLights[10];
int LightNum = 0;


void ComputeLight(out float3 addDiffuse, out float3 addSpecular, float3 position, float3 normal, float3 viewDir, int lightIndex)
{
	float4x4 matLight = matLights[lightIndex];

		float3 lightColor = float3(matLight._31, matLight._32, matLight._33) * matLight._34;

		// Directional Light
		if (matLight._14 == 0.0f)
		{
		// Light Direction
		float3 dir = float3(matLight._21, matLight._22, matLight._23);
			float NdotL = dot(-dir, normal);
		float diff = saturate(NdotL);

		// Light Reflection
		float3 lightRefl = normalize(dir + 2.0f * NdotL * normal);
			float spec = saturate(dot(lightRefl, viewDir));
		spec = pow(spec, fSpecPower);

		addDiffuse = lightColor * diff;
		addSpecular = lightColor * spec;
		}
	// Point Light and Spot Light
		else
		{
			float3 lightPosition = float3(matLight._11, matLight._12, matLight._13);
				float maxRange = matLight._41;
			float minRange = matLight._42;
			float DistancePow = matLight._24;

			float3 lightDir = lightPosition - position;

				float dist = length(lightDir);

			// Normalize
			lightDir *= (1.0f / dist);

			float NdotL = dot(lightDir, normal);
			float diff = saturate(NdotL);

			float t = saturate((dist - minRange) / (maxRange - minRange));
			t = 1.0f - pow(t, DistancePow);

			float tSpot = 1.0f;

			// Spot Light
			if (matLight._14 == 2.0f)
			{
				float inAngleCos = cos(matLight._43);
				float outAngleCos = cos(matLight._44);

				float3 dir = normalize(float3(matLight._21, matLight._22, matLight._23));
					float dotLightPixel = dot(dir, -lightDir);

				tSpot = saturate((inAngleCos - dotLightPixel) / (inAngleCos - outAngleCos));
				tSpot = 1.0f - pow(tSpot, DistancePow);
			}

			diff = diff * t * tSpot;

			float3 lightRefl = normalize(-lightDir + 2.0f * NdotL * normal);
				float spec = saturate(dot(lightRefl, viewDir));
			spec = pow(spec, fSpecPower);

			addDiffuse = lightColor * diff;
			addSpecular = lightColor * spec * diff;
		}
}



float4 ps_main(PS_INPUT Input) : COLOR0
{
	//TBN Matrix
	float3x3 TBN = float3x3(
	normalize(Input.Tangent),
	normalize(Input.Binormal),
	normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

		//Tangent Space Normal
		float3 spaceNor = (norColor * 2.0f) - 1.0f;

		float3 worldNormal = mul(spaceNor, TBN);
		float3 viewDir = normalize(Input.viewDir);

		//최종 색
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//기본 라이팅 처리

		//광원의 방향
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//광원의 컬러
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;
	if (diff < 0.0)
		diff = abs(NdotL) * 0.3f;


	//라이트 반사
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//기본 광원 적용
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular 에 diff 안곱하면 Specular 에 의한 역광이 나온다...




	//추가되는 광원
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
		for (int i = 0; i < LightNum; i++)
		{
		//i 인덱스의 광원이 계산되어 addDiffuse, addSpecular 에 대입된다.
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//최종 광원 량에 쌓인다.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
		}


	//
	// Diffuse
	//
	float3 diffuseColor = tex2D(Diffuse, Input.Texcoord).rgb * finalDiffuse;

		//
		// Specular 
		//
		float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor = diffuseColor + specularColor + emissionColor;

		return float4(finalColor, 1);
}

//------------------------------------------------------------------------------------
// SkinnedMesh CreateShadow 
//------------------------------------------------------------------------------------

float4x4 matLightViewProjection;			//방향성 광원 ViewProjection 행렬

struct VS_INPUT_DEPTH
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
	float3  BlendWeights    : BLENDWEIGHT;
	float4  BlendIndices    : BLENDINDICES;
};

struct VS_OUTPUT_DEPTH
{
	float4 Position : POSITION0;
	float4 FinalPos : TEXCOORD0;
	float2 Texcoord : TEXCOORD1;

};

VS_OUTPUT_DEPTH vs_CreateShadow(VS_INPUT_DEPTH Input, uniform int iNumBones)
{
	VS_OUTPUT_DEPTH Output = (VS_OUTPUT_DEPTH)0;

	VS_SKIN_INPUT2 vsi = { Input.Position, Input.BlendWeights, Input.BlendIndices };
	VS_SKIN_OUTPUT2 vso = VS_Skin2(vsi, iNumBones);

	//정점 월드 포지션 변경 완료.....
	float4 worldPos = float4(vso.Position.xyz, 1.0f);

	Output.Position = mul(worldPos, matLightViewProjection);

	Output.FinalPos = Output.Position;

	Output.Texcoord = Input.Texcoord;

	return(Output);
}

float4 ps_CreateShadow(VS_OUTPUT_DEPTH Input) : COLOR0
{
	//행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
	float depth = Input.FinalPos.z / Input.FinalPos.w;


	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
	clip(diffTex.a - 0.5f);

	return float4(depth.xxx, 1);
}




//--------------------------------------------------------------//
// Render With ShadowMap 
//--------------------------------------------------------------//


//쉐도우 Texture
texture Shadow_Tex;
sampler2D ShadowSampler = sampler_state
{
	Texture = (Shadow_Tex);
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;
};

float bias = 0.01f;


struct VS_INPUT_RECIVESHADOW
{
	float4 Position : POSITION0;
	float3  BlendWeights    : BLENDWEIGHT;
	float4  BlendIndices    : BLENDINDICES;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 Binormal : BINORMAL0;
	float3 Tangent : TANGENT0;
};

struct VS_OUTPUT_RECIVESHADOW
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;

	float4 FinalPos : TEXCOORD6;
	float4 LightClipPos : TEXCOORD7;		//광원 입장에서 바라본 위치
};


VS_OUTPUT_RECIVESHADOW vs_ReciveShadow(VS_INPUT_RECIVESHADOW Input, uniform int iNumBones)
{
	VS_OUTPUT_RECIVESHADOW Output = (VS_OUTPUT_RECIVESHADOW)0;


	VS_SKIN_INPUT vsi = { Input.Position, Input.BlendWeights, Input.BlendIndices, Input.Normal, Input.Binormal, Input.Tangent };
	VS_SKIN_OUTPUT vso = VS_Skin(vsi, iNumBones);

	//정점 월드 포지션 변경 완료.....
	float4 worldPos = float4(vso.Position.xyz, 1.0f);




		Output.Position = mul(worldPos, matViewProjection);

	Output.Texcoord = Input.Texcoord;

	Output.Normal = mul(Input.Normal, (float3x3)matWorld);
	Output.Binormal = mul(Input.Binormal, (float3x3)matWorld);
	Output.Tangent = mul(Input.Tangent, (float3x3)matWorld);

	Output.viewDir = vEyePos.xyz - worldPos.xyz;
	Output.worldPos = worldPos;


	Output.FinalPos = Output.Position;		//변환 정보
	Output.LightClipPos = mul(worldPos, matLightViewProjection);	//광원 입장에서 본 위치

	return(Output);
}


float4 ps_ReciveShadow(VS_OUTPUT_RECIVESHADOW Input) : COLOR0
{
	//광원 입장에서 바라본 위치의 뎁스 값 ( 라이트 행렬을 직교이기때문에 선형으로 안핀다 )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow 맵의 UV 추적
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y반전...
	//0 ~ 1 의 범위
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//현제 그려지는 놈이 위치에 DirectionLight 입장에서 써진 깊이값...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//그림자가 그려지는 상황은 shadowDepth + bias 값 보다 lightDepth 가 큰경우이다.




	//TBN Matrix
	float3x3 TBN = float3x3(
		normalize(Input.Tangent),
		normalize(Input.Binormal),
		normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

	//Tangent Space Normal
	float3 spaceNor = (norColor * 2.0f) - 1.0f;

	float3 worldNormal = mul(spaceNor, TBN);
	worldNormal = normalize(worldNormal);
	float3 viewDir = normalize(Input.viewDir);

	//최종 색
	float3 finalDiffuse = float3(0, 0, 0);
	float3 finalSpecular = float3(0, 0, 0);

	//기본 라이팅 처리

	float4x4 baseDirectionLight = matLights[0];

	float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
	float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;
	float NdotL = dot(-dir, worldNormal);
	float diff = saturate(NdotL);
	float fIntensity = baseDirectionLight._33;

	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		diff = 0;

	diff = max(diff, fIntensity * 0.3f);

	//라이트 반사
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//기본 광원 적용
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular 에 diff 안곱하면 Specular 에 의한 역광이 나온다...


	//추가되는 광원
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
		for (int i = 1; i < LightNum; i++)
		{
		//i 인덱스의 광원이 계산되어 addDiffuse, addSpecular 에 대입된다.
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//최종 광원 량에 쌓인다.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
		}


	//
	// Diffuse
	//
	float3 diffuseColor = tex2D(Diffuse, Input.Texcoord).rgb * finalDiffuse;

		//
		// Specular 
		//
		float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor = diffuseColor + specularColor + emissionColor;

		return float4(finalColor, 1);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int CurNumBones = 1;

VertexShader vsArray20[4] = { compile vs_3_0 VertSkinning(1),
compile vs_3_0 VertSkinning(2),
compile vs_3_0 VertSkinning(3),
compile vs_3_0 VertSkinning(4) };


VertexShader CreateShadowArr[4] = { compile vs_3_0 vs_CreateShadow(1),
compile vs_3_0 vs_CreateShadow(2),
compile vs_3_0 vs_CreateShadow(3),
compile vs_3_0 vs_CreateShadow(4) };


VertexShader ReciveShadowArr[4] = { compile vs_3_0 vs_ReciveShadow(1),
compile vs_3_0 vs_ReciveShadow(2),
compile vs_3_0 vs_ReciveShadow(3),
compile vs_3_0 vs_ReciveShadow(4) };


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique Base
{
	pass p0
	{
		VertexShader = (vsArray20[CurNumBones]);
		PixelShader = compile ps_3_0 ps_main();
	}
}


technique CreateShadow
{
	pass p0
	{
		VertexShader = (CreateShadowArr[CurNumBones]);
		PixelShader = compile ps_3_0 ps_CreateShadow();
	}
}


technique ReceiveShadow
{
	pass p0
	{
		VertexShader = (ReciveShadowArr[CurNumBones]);
		PixelShader = compile ps_3_0 ps_ReciveShadow();
	}
}
