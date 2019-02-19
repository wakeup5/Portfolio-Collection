#pragma once
#include "Transform.h"

class NameTag
{
private:
	static LPDIRECT3DVERTEXBUFFER9 Vb;

	LPDIRECT3DTEXTURE9 _texture;
	D3DXIMAGE_INFO _texInfo;

	Transform trans;
public:
	NameTag();
	virtual ~NameTag();

	void SetTexture(LPDIRECT3DTEXTURE9 tex, D3DXIMAGE_INFO info);
	void SetTexture(char* path);
	void Release();
	void Render();
	void RenderToSprite(LPD3DXSPRITE sprite, Camera* cam, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));

	void SetPosition(float x, float y, float z) { trans.SetWorldPosition(x, y, z); }
	void SetPosition(D3DXVECTOR3 pos) { trans.SetWorldPosition(pos); }

	void SetScale(float x, float y, float z) { trans.SetScale(x, y, z); }
	void SetScale(D3DXVECTOR3 scale) { trans.SetScale(scale); }

	void SetRotate(float rx, float ry, float rz) { trans.SetRotateLocal(rx, ry, rz); }
	void SetRotate(D3DXVECTOR3 rot) { trans.SetRotateLocal(rot.x, rot.y, rot.z); }

	void Look(D3DXVECTOR3 dir, D3DXVECTOR3 up) { trans.LookDirection(dir, up); }

	D3DXVECTOR3 GetScreenPos(Camera* cam);
};

class NameTagTexture
{
private:
	LPDIRECT3DTEXTURE9 _drawTexture;
	D3DXIMAGE_INFO _drawInfo;

	LPDIRECT3DSURFACE9	prevTarget;
public:
	void Setup(float width, float height);

	void Begin();
	void End();

	LPDIRECT3DTEXTURE9 GetTexture() { return _drawTexture; }
	D3DXIMAGE_INFO &GetInfo() { return _drawInfo; }
};
