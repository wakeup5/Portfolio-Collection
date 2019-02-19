#pragma once
#include "Scene.h"
#include "UIScreen.h"
#include "UIWindow.h"
#include "UIButton.h"
#include "Player.h"
#include "cLight_Direction.h"

class CharacterSelect :
	public Scene
{
private:
	LPD3DXSPRITE _sprite;
	UIWindow* _ui;
	float time;
	Player* _player;

	cXMesh_Static* _pMesh;
	Transform* pTransform;
public:
	CharacterSelect();
	~CharacterSelect();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();

};

