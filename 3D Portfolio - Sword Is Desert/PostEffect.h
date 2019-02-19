#pragma once
#include "cLight_Direction.h"
class PostEffect
{
public:
	LPD3DXEFFECT _effect;

	//기본
	LPDIRECT3DTEXTURE9 _base;
	LPDIRECT3DTEXTURE9 _depth;

	//블러
	LPDIRECT3DTEXTURE9 _blur;

	//선라이트
	LPDIRECT3DTEXTURE9 _sunlightBase;
	LPDIRECT3DTEXTURE9 _sunlight;
	LPDIRECT3DTEXTURE9 _sunlightStarBase;
	LPDIRECT3DTEXTURE9 _sunlightStar[6];

	LPDIRECT3DSURFACE9 _ds;

	//비
	LPDIRECT3DTEXTURE9 _rainBase;
	LPDIRECT3DTEXTURE9 _rainNormal;

	LPDIRECT3DTEXTURE9 _motionBlur;
	std::vector<LPDIRECT3DTEXTURE9> _mbt;

	PT_VERTEX _renderVertex[6];
	PT_VERTEX _showVertex[6];

	LPDIRECT3DSURFACE9	prevTarget, prevDepth;

	Camera* _lightCamera;

public:
	PostEffect();
	virtual ~PostEffect();

	void BeginBase();
	void EndBase();
	void RenderBase();
	
	void RenderDof();
	
	void RenderBlur();

	void BeginSunlight();
	void EndSunlight();
	void RenderSunlightBlur();

	void BeginRain();
	void EndRain();
	void RenderRain();

	void ShowTexture(LPDIRECT3DTEXTURE9 t);

	void BeginTexture(LPDIRECT3DTEXTURE9 t, DWORD flag = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL);
	void EndTexture();

	void RenderSixStar();
private:
	void BeginBlur();
	void EndBlur();
};

