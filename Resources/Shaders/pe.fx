struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float2 Tex : TEXCOORD0;
};

VS_OUTPUT vs_main(float4 Position : POSITION0, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Output;

	Output.Position = Position;
	Output.Tex = Tex;

	return Output;
}

texture TexColor;
sampler t1 //: register(s0);
/*
= sampler_state
{
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	AddressU = Wrap;          // Addressing
	AddressV = Wrap;
};
*/

= sampler_state
{
	Texture = (TexColor);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

texture TexDepth;
sampler t2// : register(s1);
/*
= sampler_state
{
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	AddressU = Wrap;          // Addressing
	AddressV = Wrap;
};
*/
= sampler_state
{
	Texture = (TexDepth);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

float4 ps_main(float2 Tx0 : TEXCOORD0) : COLOR0
{
	//PS_OUTPUT  Output = (PS_OUTPUT)0;
	float4 d = tex2D(t1, Tx0);

	float r = 3;
	//if (min(1, d.a) > 0.001)
	//{
	//	r = 1;
	//}
	//else
	//{
	//	r = 0.4;
	//}

	float rs = ceil(r * 2.57);

	/*
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);
	*/

	//float4 In = tex2D(t1, float2(0, 0));
	
	float4 val = 0;
	float wsum = 0;

	float ci = (2 * r * r);

	for (float iy = -rs; iy < rs + 1; iy+=1)
	{
		for (float ix = -rs; ix < rs + 1; ix+=1)
		{
			float x = (Tx0.x + (2.f * ix) / 1024.f);
			float y = (Tx0.y + (2.f * iy) / 800.f);

			float dsp = (ix)* (ix)+(iy)* (iy);
			float wght = exp(-dsp / ci) / (3.141592f * ci);

			float2 T = float2(x, y);
			float4 In = tex2D(t1, T);

			val += In * wght;
			wsum += wght;
		}
	}
	
	return d + (val / wsum);
	
	/*
	float4 val = 0;

	float r = 5;
	int rs = r;
	for (int iy = -rs; iy < rs + 1; iy++)
	{
		for (int ix = -rs; ix < rs + 1; ix++)
		{
			float x = (Tx0.x + (2.f * ix) / 1024.f);
			float y = (Tx0.y + (2.f * iy) / 800.f);

			float2 T = float2(x, y);
			float4 In = tex2D(t1, T);

			val += In / (rs * rs * 2);
		}
	}

	return val;
	*/
}

technique Base
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}
