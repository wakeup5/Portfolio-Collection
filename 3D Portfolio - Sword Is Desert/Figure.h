#pragma once

typedef struct tagMyPC_Vertex
{
	D3DXVECTOR3 p = { 0, 0, 0 };
	DWORD		c = 0;
	static enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

	tagMyPC_Vertex()
	{

	}
	tagMyPC_Vertex(D3DXVECTOR3 pos)
	{
		p = pos;
		c = 0;
	}
	tagMyPC_Vertex(float x, float y, float z)
	{
		p = D3DXVECTOR3(x, y, z);
		c = 0;
	}
	tagMyPC_Vertex(D3DXVECTOR3 pos, D3DCOLOR color)
	{
		p = pos;
		c = color;
	}
	tagMyPC_Vertex(float x, float y, float z, D3DCOLOR color)
	{
		p = D3DXVECTOR3(x, y, z);
		c = color;
	}
} MyPC_Vertex, FAR *LPMyPC_Vertex;

typedef struct tagMyPCT_Vertex
{
	D3DXVECTOR3 p = { 0, 0, 0 };
	DWORD c = 0;
	D3DXVECTOR2	t = { 0, 0 };
	static enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };

	tagMyPCT_Vertex()
	{

	}
	tagMyPCT_Vertex(D3DXVECTOR3 &pos)
	{
		p = pos;
	}
	tagMyPCT_Vertex(float x, float y, float z)
	{
		p = D3DXVECTOR3(x, y, z);
	}
	tagMyPCT_Vertex(D3DXVECTOR3 &pos, D3DXVECTOR2 &tex)
	{
		p = pos;
		t = tex;
	}
	tagMyPCT_Vertex(float x, float y, float z, float u, float v)
	{
		p = D3DXVECTOR3(x, y, z);
		t = D3DXVECTOR2(u, v);
	}
} MyPCT_Vertex, FAR *LPMyPCT_Vertex;


class Figure
{
protected:

	std::vector<MyPC_Vertex>	_ver;
	D3DPRIMITIVETYPE		_primitiveType;
	UINT					_count;
	float					_alpha;

	static LPD3DXEFFECT		Effect;

	//∞Ì¡§
	static MyPCT_Vertex _pointBlendVertex[6];
	static bool _initPointVertex;
	
protected:
	LPDIRECT3DTEXTURE9 _pointTexture = NULL;
	float _pointSize = 1;

	Transform* _pointTrans;
	float _size;
private:
	static bool _isRenderBegin;

public:
	Transform				pTransform;
public:
	Figure();
	virtual ~Figure();

	virtual void Setup() {}
	virtual void Render();

	void SetAlpha(float a);
	void SetSize(float s) { _size = s; }

	static void Begin();
	static void End();
};

