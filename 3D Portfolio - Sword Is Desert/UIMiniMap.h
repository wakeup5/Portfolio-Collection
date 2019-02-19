#pragma once
#include "UIWindow.h"
class UIMiniMap :
	public UIWindow
{
private:
	LPDIRECT3DTEXTURE9 _map = NULL;
	D3DXIMAGE_INFO _mapI;
	LPDIRECT3DTEXTURE9 _sight = NULL;
	D3DXIMAGE_INFO _sightI;
	LPDIRECT3DTEXTURE9 _monster = NULL;
	D3DXIMAGE_INFO _monsterI;
	LPDIRECT3DTEXTURE9 _npc = NULL;
	D3DXIMAGE_INFO _npcI;

	float _zoom = 1;
public:
	UIMiniMap();
	virtual ~UIMiniMap();

	void Setup();
	void Release();
	void Update();
	void Render(LPD3DXSPRITE sprite);

	void SetMapTexture(std::string map, std::string s, std::string m, std::string n);
};

