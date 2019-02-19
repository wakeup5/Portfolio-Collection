#include "stdafx.h"
#include "NameTag.h"

LPDIRECT3DVERTEXBUFFER9 NameTag::Vb = NULL;

NameTag::NameTag()
	: _texture(NULL)
{
	if (Vb != NULL) return;

	DEVICE->CreateVertexBuffer(sizeof(PT_VERTEX) * 6, 0, PT_VERTEX::FVF, D3DPOOL_DEFAULT, &Vb, NULL);

	PT_VERTEX* v;

	Vb->Lock(0, 0, (LPVOID*)&v, 0);

	v[0] = PT_VERTEX(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[1] = PT_VERTEX(1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[2] = PT_VERTEX(1.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	
	v[3] = v[0];
	v[4] = v[2];
	v[5] = PT_VERTEX(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f);

	Vb->Unlock();
}


NameTag::~NameTag()
{
}

void NameTag::SetTexture(LPDIRECT3DTEXTURE9 tex, D3DXIMAGE_INFO info)
{
	if (_texture) _texture->Release();

	_texture = tex;
	_texture->AddRef();

	_texInfo = info;
}

void NameTag::SetTexture(char* path)
{
	if (_texture) _texture->Release();

	_texture = TEXTURE_MGR->GetTexture(path, &_texInfo);
	_texture->AddRef();
}

void NameTag::Release()
{
	_texture->Release();
}

void NameTag::Render()
{
	//DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	DEVICE->SetTransform(D3DTS_WORLD, &trans.GetFinalMatrix());
	DEVICE->SetTransform(D3DTS_VIEW, &CAMERA->GetViewMatrix());
	DEVICE->SetTransform(D3DTS_PROJECTION, &CAMERA->GetProjectionMatrix());

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);

	DEVICE->BeginScene();

	DEVICE->SetTexture(0, _texture);
	DEVICE->SetFVF(PT_VERTEX::FVF);
	DEVICE->SetStreamSource(0, Vb, 0, sizeof(PT_VERTEX));
	DEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	DEVICE->SetTexture(0, NULL);

	DEVICE->EndScene();
}

void NameTag::RenderToSprite(LPD3DXSPRITE sprite, Camera* cam, D3DCOLOR color)
{
	if (_texture == NULL) return;

	D3DXVECTOR3 pos = GetScreenPos(cam);

	D3DXMATRIXA16 matPos, matTotal;

	D3DXMatrixTranslation(&matPos, pos.x - _texInfo.Width / 2, pos.y - _texInfo.Height / 2, 0.0f);

	//гу
	D3DXMatrixIdentity(&matTotal);
	matTotal = matPos;

	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	sprite->SetTransform(&matTotal);

	RECT rc;
	SetRect(&rc, 0, 0, _texInfo.Width, _texInfo.Height);
	sprite->Draw(_texture, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), color); 
	
	sprite->End();
}

D3DXVECTOR3 NameTag::GetScreenPos(Camera* cam)
{
	D3DXVECTOR3 result = trans.GetWorldPosition();

	D3DXMATRIXA16 mVP, mVPT;

	mVP = cam->GetViewProjectionMatrix();
	
	D3DXVec3TransformCoord(&result, &result, &mVP);

	D3DXMatrixIdentity(&mVPT);
	mVPT._11 = WINSIZE_X / 2;
	mVPT._22 = -WINSIZE_Y / 2;
	mVPT._33 = cam->GetFar() - cam->GetNear();
	mVPT._41 = WINSIZE_X / 2;
	mVPT._42 = WINSIZE_Y / 2;
	mVPT._43 = cam->GetNear();
	mVPT._44 = 1;

	D3DXVec3TransformCoord(&result, &result, &mVPT);
	result.z = 0;
	//result.x *= -WINSIZE_X;
	////result.y *= -WINSIZE_Y;
	//result.z = 0;

	return result;
}

void NameTagTexture::Setup(float width, float height)
{
	DEVICE->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_drawTexture, NULL);
	_drawInfo.Width = width;
	_drawInfo.Height = height;
}

void NameTagTexture::Begin()
{
	DEVICE->GetRenderTarget(0, &prevTarget);

	LPDIRECT3DSURFACE9 texSurface = NULL;
	_drawTexture->GetSurfaceLevel(0, &texSurface); DEVICE->SetRenderTarget(0, texSurface); SAFE_RELEASE(texSurface);

	DEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(1, 1, 1, 0), 1, 0);
}

void NameTagTexture::End()
{
	DEVICE->SetRenderTarget(0, prevTarget);

	SAFE_RELEASE(prevTarget);
}
