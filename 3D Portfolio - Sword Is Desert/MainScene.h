#pragma once
#include "Scene.h"
#include "NameTag.h"

class UIScreen;
class UISkill;
class UIInventory;
class UIMouseSocket;
class UIDialog;
class UIEquippedItem;
class UIShop;
class UIQuest;
class UILevel;
class Day;
class MapView;

class PostEffect;
class cQuadParticleEmitter;

class MainScene : public Scene
{
private:
	UIScreen* _ui;
	UISkill* _uiSkill;
	UIInventory* _uiInven;
	UIShop* _uiShop;
	UIQuest* _uiQuest;
	UIMouseSocket* _uiMouseSocket;
	UIDialog* _uiDialog;
	UIEquippedItem* _uiEquip;

	Day* _day;

	//맵 오브젝트
	std::vector<cBaseObject*> _mapObj;

	//LPDIRECT3DTEXTURE9 _coreRenderTexture;
	//LPDIRECT3DTEXTURE9 _coreDepthTexture;
	//LPDIRECT3DSURFACE9 _coreDepthSurface;

	//LPD3DXEFFECT _postEffect;

	//PT_VERTEX _renderTexture[6];

	PostEffect* _postEffect;

	//테스트
	NameTag _tag;

	MapView* _map;

	cQuadParticleEmitter* _testEffect;

	//그림자를 위한
	Camera* _lightCamera;
public:
	MainScene();
	virtual ~MainScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();

	void CoreRender();
	void UIRender();
};

