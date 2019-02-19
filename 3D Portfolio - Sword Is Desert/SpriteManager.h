#pragma once
#include "Singleton.h"
class SpriteManager : public Singleton < SpriteManager >
{
private:
	LPD3DXSPRITE _sprite;
	LPDIRECT3DTEXTURE9 _texture;

	float _fadeTime = 0;
	float _totalTime = 0;
public:
	SpriteManager();
	virtual ~SpriteManager();

	void Setup(LPDIRECT3DDEVICE9 device);
	void Update(float timeDelta);
	void Render();
	
	void BeginSprite(DWORD flag = D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	void EndSprite();

	LPD3DXSPRITE GetSprite() { return _sprite; }

	void FadeInBlack(float fadeTime);
};

#define SPRITE_MGR SpriteManager::GetInstance()
#define SPRITE SPRITE_MGR->GetSprite()