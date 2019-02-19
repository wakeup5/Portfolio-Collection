float4x4		matWVP;
float4			color;

struct VS_INPUT
{
	float3 Position : POSITION;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	float4 worldPos = mul(float4(Input.Position, 1), matWVP);
	Output.Position = worldPos;

	return Output;
}

struct PS_INPUT
{
};

float4 ps_main(PS_INPUT In) : COLOR0
{
	float4 finalColor = color;

	return finalColor;
}

technique MyShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}
