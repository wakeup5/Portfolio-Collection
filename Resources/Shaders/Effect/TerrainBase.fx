float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4x4 matWVP;
float4 vEyePos : ViewPosition;
float4 worldLightDir;
float4 worldLightColor;
float snow;
float4x4 matLightViewProjection;
float fIntensity;
float bias = 0.002f;
float4x4 matLights[10];
int LightNum;
float fSpecPower = 2.0f;

// Pos.x			Pos.y				Pos.z			lightType ( 0 Direction, 1 Point, 2 Spot )
// Dir.x			Dir.y				Dir.z			DistancePow
// Color.r			Color.g			Color.b		strength
// MaxRange		MinRange		InAngle		OutAngle

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

//
// Base
//

struct VS_INPUT
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float3 Binormal : BINORMAL0;
	float3 Tangent : TANGENT0;
	float2 BaseUV : TEXCOORD0;
	float2 TileUV : TEXCOORD1;
	float4 Splatting : TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float2 BaseUV : TEXCOORD0;
	float2 TileUV : TEXCOORD1;
	float4 Splatting : TEXCOORD2;
	float3 Normal : TEXCOORD3;
	float3 Binormal : TEXCOORD4;
	float3 Tangent : TEXCOORD5;
	float3 ViewDir : TEXCOORD6;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	float4 worldPos = mul(Input.Position, matWorld);
	Output.Position = mul(worldPos, matViewProjection);

	Output.BaseUV = Input.BaseUV;
	Output.TileUV = Input.TileUV;
	Output.Splatting = Input.Splatting;

	Output.Normal = mul(Input.Normal, (float3x3)matWorld);
	Output.Binormal = mul(Input.Binormal, (float3x3)matWorld);
	Output.Tangent = mul(Input.Tangent, (float3x3)matWorld);

	Output.ViewDir = vEyePos.xyz - worldPos.xyz;

	return(Output);
}

struct PS_INPUT
{
	float2 BaseUV : TEXCOORD0;
	float2 TileUV : TEXCOORD1;
	float4 Splatting : TEXCOORD2;
	float3 Normal : TEXCOORD3;
	float3 Binormal : TEXCOORD4;
	float3 Tangent : TEXCOORD5;
	float3 ViewDir : TEXCOORD6;
};

texture Terrain0_Tex;
sampler2D Terrain0 = sampler_state
{
	Texture = (Terrain0_Tex);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Terrain0_Tex_N;
sampler2D Terrain0_N = sampler_state
{
	Texture = (Terrain0_Tex_N);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Terrain1_Tex;
sampler2D Terrain1 = sampler_state
{
	Texture = (Terrain1_Tex);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Terrain1_Tex_N;
sampler2D Terrain1_N = sampler_state
{
	Texture = (Terrain1_Tex_N);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Terrain2_Tex;
sampler2D Terrain2 = sampler_state
{
	Texture = (Terrain2_Tex);
	MAXANISOTROPY = 16;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
	MAGFILTER = ANISOTROPIC;
};

texture Terrain2_Tex_N;
sampler2D Terrain2_N = sampler_state
{
	Texture = (Terrain2_Tex_N);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Terrain3_Tex;
sampler2D Terrain3 = sampler_state
{
	Texture = (Terrain3_Tex);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Terrain3_Tex_N;
sampler2D Terrain3_N = sampler_state
{
	Texture = (Terrain3_Tex_N);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture TerrainControl_Tex;
sampler2D TerrainControl = sampler_state
{
	Texture = (TerrainControl_Tex);
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Terrain_Snow_Tex;
sampler2D TerrainSnow = sampler_state
{
	Texture = Terrain_Snow_Tex;
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

float4 ps_main(PS_INPUT Input) : COLOR0
{
	// TBN Matrix
	float3x3 TBN = float3x3(
	normalize(Input.Tangent),
	normalize(Input.Binormal),
	normalize(Input.Normal)
	);

	// Terrain Tile 컬러를 얻는다.
	float3 terrain0 = tex2D(Terrain0, Input.TileUV).rgb;
	float3 terrain1 = tex2D(Terrain1, Input.TileUV).rgb;
	float3 terrain2 = tex2D(Terrain2, Input.TileUV).rgb;
	float3 terrain3 = tex2D(Terrain3, Input.TileUV).rgb;
	float3 terrain0_N = tex2D(Terrain0_N, Input.TileUV).rgb;
	float3 terrain1_N = tex2D(Terrain1_N, Input.TileUV).rgb;
	float3 terrain2_N = tex2D(Terrain2_N, Input.TileUV).rgb;
	float3 terrain3_N = tex2D(Terrain3_N, Input.TileUV).rgb;
	float3 terrainSnow = tex2D(TerrainSnow, Input.BaseUV).rgb;

	// 컨트롤 Texture 에서 색상비율을 얻는다
	float redFactor = Input.Splatting.r;
	float greenFactor = Input.Splatting.g;
	float blueFactor = Input.Splatting.b;
	float blackFactor = saturate(1.0f - (redFactor + greenFactor + blueFactor));
	float snowFactor = snow;

	float3 terrainNormal = terrain0_N * blackFactor + terrain1_N * redFactor + terrain2_N * greenFactor + terrain3_N * blueFactor;
	float3 worldNormal = mul(terrainNormal, TBN);

	// Light Direction
	float3 dir = worldLightDir.xyz;
	float NdotL = dot(-dir, worldNormal);
	float diff = saturate(NdotL);

	// Light Reflection
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
	float3 viewDir = normalize(Input.ViewDir);
	float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, 2);

	float3 finalDiffuse = worldLightColor.rgb * diff;
	float3 finalSpecular = worldLightColor.rgb * spec;

	float3 texColor0 = terrain0 * blackFactor;
	float3 texColor1 = terrain1 * redFactor;
	float3 texColor2 = terrain2 * greenFactor;
	float3 texColor3 = terrain3 * blueFactor;
	float3 texColorSnow = terrainSnow * snowFactor;

	float3 finalColor = texColor0 + texColor1 + texColor2 + texColor3;
	finalColor.rgb = texColorSnow.rgb + (1 - texColorSnow.rgb) * finalColor.rgb;

	finalColor = finalColor * finalDiffuse + finalColor * finalSpecular;

	return float4(finalColor, 1);
}

technique Base
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}


//
// Create Shadow
//

struct VS_INPUT_DEPTH
{
	float4 Position : POSITION0;
};

struct VS_OUTPUT_DEPTH
{
	float4 Position : POSITION0;
	float4 FinalPos : TEXCOORD0;
};

VS_OUTPUT_DEPTH vs_depth(VS_INPUT_DEPTH Input)
{
	VS_OUTPUT_DEPTH Output = (VS_OUTPUT_DEPTH)0;

	Output.Position = mul(Input.Position, matWVP);
	Output.FinalPos = Output.Position;

	return Output;
}

float4 ps_CreateShadow(VS_OUTPUT_DEPTH Input) : COLOR0
{
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	return float4(depth.xxx, 1);
}

technique CreateShadow
{
	pass Pass_0
	{
		CULLMODE = NONE;

		VertexShader = compile vs_3_0 vs_depth();
		PixelShader = compile ps_3_0 ps_CreateShadow();
	}
}


//
// Render with ShadowMap
//

texture Shadow_Tex;
sampler2D ShadowSampler = sampler_state
{
	Texture = Shadow_Tex;
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;
	MAXANISOTROPY = 16;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

struct VS_INPUT_RECEIVESHADOW
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float3 Binormal : BINORMAL0;
	float3 Tangent : TANGENT0;
	float2 BaseUV : TEXCOORD0;
	float2 TileUV : TEXCOORD1;
	float4 Splatting : TEXCOORD2;
};

struct VS_OUTPUT_RECEIVESHADOW
{
	float4 Position : POSITION0;
	float2 BaseUV : TEXCOORD0;
	float2 TileUV : TEXCOORD1;
	float4 Splatting : TEXCOORD2;
	float4 FinalPos : TEXCOORD3;
	float4 LightClipPos : TEXCOORD4;
	float3 Normal : TEXCOORD5;
	float3 Binormal : TEXCOORD6;
	float3 Tangent : TEXCOORD7;
	float3 ViewDir : TEXCOORD8;
	float3 WorldPos : TEXCOORD9;
};

VS_OUTPUT_RECEIVESHADOW vs_ReceiveShadow(VS_INPUT_RECEIVESHADOW Input)
{
	VS_OUTPUT_RECEIVESHADOW Output = (VS_OUTPUT_RECEIVESHADOW)0;

	float4 worldPos = mul(Input.Position, matWorld);
	Output.Position = mul(worldPos, matViewProjection);

	Output.TileUV = Input.TileUV;
	Output.BaseUV = Input.BaseUV;
	Output.Splatting = Input.Splatting;

	Output.Normal = mul(Input.Normal, (float3x3)matWorld);
	Output.Binormal = mul(Input.Binormal, (float3x3)matWorld);
	Output.Tangent = mul(Input.Tangent, (float3x3)matWorld);

	Output.FinalPos = Output.Position;
	Output.LightClipPos = mul(worldPos, matLightViewProjection);

	Output.ViewDir = vEyePos.xyz - worldPos.xyz;
	Output.WorldPos = worldPos.xyz;

	return Output;
}

float4 ps_ReceiveShadow(VS_OUTPUT_RECEIVESHADOW Input) : COLOR0
{
	// TBN Matrix
	float3x3 TBN = float3x3(
	normalize(Input.Tangent),
	normalize(Input.Binormal),
	normalize(Input.Normal)
	);

	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
	shadowUV.y = -shadowUV.y;
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	float depthDiff = (lightDepth - shadowDepth) * 0.25f;

	float shadowDepthNext[8];

	float pixelSize = 1.0f / 512.0f;

	// 그림자 뭉개기
	shadowUV.x -= pixelSize;
	shadowUV.y -= pixelSize;
	shadowDepthNext[0] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[0]) * 0.0625f;

	shadowUV.x += pixelSize;
	shadowDepthNext[1] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[1]) * 0.125f;

	shadowUV.x += pixelSize;
	shadowDepthNext[2] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[2]) * 0.0625f;

	shadowUV.x -= pixelSize * 2;
	shadowUV.y += pixelSize;
	shadowDepthNext[3] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[3]) * 0.125f;

	shadowUV.x += pixelSize * 2;
	shadowDepthNext[4] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[4]) * 0.125f;

	shadowUV.x -= pixelSize * 2;
	shadowUV.y += pixelSize;
	shadowDepthNext[5] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[5]) * 0.0625f;

	shadowUV.x += pixelSize;
	shadowDepthNext[6] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[6]) * 0.125f;

	shadowUV.x += pixelSize;
	shadowDepthNext[7] = tex2D(ShadowSampler, shadowUV).r;
	depthDiff += (lightDepth - shadowDepthNext[7]) * 0.0625f;

	depthDiff *= 9;

	float3 terrain0 = tex2D(Terrain0, Input.TileUV).rgb;
	float3 terrain1 = tex2D(Terrain1, Input.TileUV).rgb;
	float3 terrain2 = tex2D(Terrain2, Input.TileUV).rgb;
	float3 terrain3 = tex2D(Terrain3, Input.TileUV).rgb;
	float3 terrain0_N = tex2D(Terrain0_N, Input.TileUV).rgb;
	float3 terrain1_N = tex2D(Terrain1_N, Input.TileUV).rgb;
	float3 terrain2_N = tex2D(Terrain2_N, Input.TileUV).rgb;
	float3 terrain3_N = tex2D(Terrain3_N, Input.TileUV).rgb;
	float3 terrainSnow = tex2D(TerrainSnow, Input.BaseUV).rgb;

	// 컨트롤 Texture 에서 색상비율을 얻는다
	float redFactor = Input.Splatting.r;
	float greenFactor = Input.Splatting.g;
	float blueFactor = Input.Splatting.b;
	float blackFactor = saturate(1.0f - (redFactor + greenFactor + blueFactor));
	float snowFactor = snow;

	float3 terrainNormal = terrain0_N * blackFactor + terrain1_N * redFactor + terrain2_N * greenFactor + terrain3_N * blueFactor;
	terrainNormal = (terrainNormal * 2.0f) - 1.0f;
	float3 worldNormal = mul(terrainNormal, TBN);
	worldNormal = normalize(worldNormal);

	float3 texColor0 = terrain0 * blackFactor;
	float3 texColor1 = terrain1 * redFactor;
	float3 texColor2 = terrain2 * greenFactor;
	float3 texColor3 = terrain3 * blueFactor;
	float3 texColorSnow = terrainSnow * snowFactor;

	float3 finalColor = texColor0 + texColor1 + texColor2 + texColor3;
	finalColor.rgb = texColorSnow.rgb + (1 - texColorSnow.rgb) * finalColor.rgb;
	float3 diffuseFinalColor = finalColor;

	// Light Direction
	float3 dir = worldLightDir.xyz;
	float NdotL = dot(-dir, worldNormal);
	float diff = saturate(NdotL);

	// Light Reflection
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
	float3 viewDir = normalize(Input.ViewDir);
	float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, 4);
	
	diff *= min(1, 1 - depthDiff);

	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		diff = 0;

	diff = max(diff, fIntensity * 0.3f);

	float3 finalDiffuse = worldLightColor.rgb * diff;
	float3 finalSpecular = worldLightColor.rgb * spec * diff * 0.8f;
	float3 finalAmbient = worldLightColor.rgb * 0.1f;

	float3 finalAddDiffuse = float3(0, 0, 0);
	float3 finalAddSpecular = float3(0, 0, 0);
	float3 addDiffuse = float3(0, 0, 0);
	float3 addSpecular = float3(0, 0, 0);

	for (int i = 1; i < LightNum; ++i)
	{
		ComputeLight(addDiffuse, addSpecular, Input.WorldPos, worldNormal, viewDir, i);

		finalAddDiffuse += addDiffuse;
		finalAddSpecular += addSpecular;
	}

	finalColor *= (finalDiffuse + finalSpecular) * fIntensity + finalAmbient;

	finalColor += finalAddDiffuse * diffuseFinalColor + finalAddSpecular * diffuseFinalColor;

	return float4(finalColor, 1);
}

technique ReceiveShadow
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_ReceiveShadow();
		PixelShader = compile ps_3_0 ps_ReceiveShadow();
	}
}
