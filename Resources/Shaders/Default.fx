struct VS_INPUT
{
	float4 Position : POSITION0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
};

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

float alpha;

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT output;
	
	float4 pos = mul(Input.Position, matWorld);
	output.Position = mul(pos, matViewProjection);

	return output;
}

struct PS_INPUT
{
	float4 Color : COLOR0;
};

float4 ps_main(PS_INPUT Input) : COLOR0
{
	Input.Color.a = 0;

	return alpha;
}

technique Vertex
{
	pass Pass_0
	{
		AlphaBlendEnable = TRUE;
		SRCBLEND = ONE;
		DESTBLEND = ONE;
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}
