float4x4 matWVP : WorldViewProjection;
float4x4 matWorld : World;
float4 vLightColor;
float fIntensity;
float fNightFactor;

//
// Vertex Shader
//

struct VS_INPUT
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float3 Normal : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	// Position
	Output.Position = mul(Input.Position, matWVP);

	// WorldNormal
	Output.Normal = mul(Input.Normal, (float3x3)matWorld);

	return Output;
}

//
// Pixel Shader
//

struct PS_INPUT
{
	float3 Normal : TEXCOORD0;
};

texture EnvironmetTexture;
texture EnvironmentTextureNight;

samplerCUBE MyCubeSampler = sampler_state
{
	Texture = EnvironmetTexture;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

samplerCUBE MyCubeSamplerNight = sampler_state
{
	Texture = EnvironmentTextureNight;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

float4 ps_main(PS_INPUT Input) : COLOR0
{
	float3 normal = normalize(Input.Normal);
	float3 DayColor = texCUBE(MyCubeSampler, normal).rgb;
	float3 NightColor = texCUBE(MyCubeSamplerNight, normal).rgb;
	
	float3 finalColor;

	if (fNightFactor > 0)
		finalColor = NightColor.rgb * vLightColor.rgb * fIntensity * 5.0f * fNightFactor;
	else
		finalColor = DayColor.rgb * vLightColor.rgb * fIntensity;

	return float4(finalColor, 1);
}

//
// technique
//

technique EnvironmentTechnique
{
	pass P0
	{
		CULLMODE = CW;
		ZWRITEENABLE = FALSE;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}
