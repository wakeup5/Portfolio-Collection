#include "stdafx.h"
#include "SpriteManager.h"


SpriteManager::SpriteManager()
{
}


SpriteManager::~SpriteManager()
{
	_sprite->Release();
	_texture->Release();
}

void SpriteManager::Setup(LPDIRECT3DDEVICE9 device)
{
	D3DXCreateSprite(device, &_sprite);
	_sprite->AddRef();

	D3DXCreateTexture(device, WINSIZE_X, WINSIZE_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &_texture);
	_texture->AddRef();

	IDirect3DSurface9 *surface;
	_texture->GetSurfaceLevel(0, &surface);
	device->ColorFill(surface, NULL, 0xff000000);
	surface->Release();
}

void SpriteManager::Update(float timeDelta)
{
	if (_fadeTime > 0)
	{
		_fadeTime -= timeDelta;
	}
}

void SpriteManager::Render()
{
	BeginSprite();
	if (_fadeTime > 0)
	{
		D3DXMATRIXA16 matFinal;
		D3DXMatrixIdentity(&matFinal);
		_sprite->SetTransform(&matFinal);

		RECT r = { 0, 0, WINSIZE_X, WINSIZE_Y };

		_sprite->Draw(
			_texture,
			&r,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			D3DXCOLOR(1, 1, 1, (_fadeTime / _totalTime))
			);
	}
	EndSprite();
}

void SpriteManager::BeginSprite(DWORD flag)
{
	_sprite->Begin(flag);
}

void SpriteManager::EndSprite()
{
	_sprite->End();
}

void SpriteManager::FadeInBlack(float fadeTime)
{
	_fadeTime = _totalTime = fadeTime;
}
