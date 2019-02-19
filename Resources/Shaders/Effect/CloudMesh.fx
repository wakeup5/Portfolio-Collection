float4x4 matWVP;
float4 vLightColor;
float fIntensity;

//
// Vertex Shader
//

struct VS_INPUT
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	Output.Position = mul(Input.Position, matWVP);
	Output.Texcoord = Input.Texcoord;

	return Output;
}

//
// Pixel Shader
//

struct PS_INPUT
{
	float2 Texcoord : TEXCOORD0;
};

texture CloudTex;
sampler2D CloudSample = sampler_state
{
	Texture = CloudTex;
	MAGFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
};

float4 ps_main(PS_INPUT Input) : COLOR0
{
	float3 CloudColor = tex2D(CloudSample, Input.Texcoord).rgb;

	clip(CloudColor.r - 0.1f);

	float4 CloudColor2 = float4(CloudColor, 0.04f);

	float4 finalColor = CloudColor2 * vLightColor * fIntensity;

	return finalColor;
}


float4 ps_dark(PS_INPUT Input) : COLOR0
{
	float3 CloudColor = tex2D(CloudSample, Input.Texcoord).rgb;

	clip(CloudColor.r - 0.1f);

	float4 CloudColor2 = float4(0.98f, 0.98f, 0.98f, 1);

	float4 finalColor = CloudColor2;

	return finalColor;
}


technique Base
{
	pass Pass_0
	{
		CULLMODE = NONE;
		ZWRITEENABLE = FALSE;
		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = ONE;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}

technique Dark
{
	pass Pass_0
	{
		CULLMODE = NONE;
		ZWRITEENABLE = FALSE;
		ALPHABLENDENABLE = TRUE;
		BLENDOP = ADD;
		SRCBLEND = DESTCOLOR;
		DESTBLEND = ZERO;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_dark();
	}
}
