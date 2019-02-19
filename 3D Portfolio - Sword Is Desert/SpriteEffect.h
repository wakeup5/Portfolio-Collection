#pragma once

class SpriteEffect
{
public:
	Transform* Origin = NULL;
	Transform* Local = NULL;

	D3DXVECTOR3 Scale = D3DXVECTOR3(1, 1, 1);
private:
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

	D3DXMATRIXA16 _prevMat;
	D3DXVECTOR3 _prevScale;


	//매쉬 관련
	SpriteEffect::MyPCT_Vertex _vertex[6];
	SpriteEffect::MyPCT_Vertex _vertex2[6];

	LPDIRECT3DTEXTURE9 _texture = NULL;

	bool _isCross;

	//시간 관련
	float _lifeTime, _currentTime;

	bool _isLoop;
	bool _isRun;

	//좌표 관련
	UINT _maxU = 1, _maxV = 1;

	//카메라 봄?
	bool _isLookCamera;

	//출력 여부
	bool _isRendering;

	//z버퍼 여부
	bool _isZBufferOff;
public:
	//이동. 회전. 스케일
	D3DXVECTOR3 _posDelta;
	D3DXVECTOR3 _rotDelta;
	std::vector<D3DXVECTOR3> _scaleDalta;
	std::vector<DWORD> _colorDalta;
public:
	SpriteEffect();
	~SpriteEffect();

	virtual void Setup(const SEParam* param = NULL);
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();

	void Start();
	void Start(float lifeTime, bool isLoop);
	void Stop();
	
	void SetUV(UINT u, UINT v);
	void SetTexture(LPDIRECT3DTEXTURE9 tex);

	void SetCrossState(bool is);
	void SetLookCameraState(bool is);
	void SetRenderingState(bool is);
	void SetZBufferOffState(bool is);

	SpriteEffect* SetCross(bool is) { SetCrossState(is); return this; }
	SpriteEffect* SetLookCam(bool is) { SetLookCameraState(is); return this; }
	SpriteEffect* SetRender(bool is) { SetRenderingState(is); return this; }
	SpriteEffect* SetZBufferOff(bool is) { SetZBufferOffState(is); return this; }
};