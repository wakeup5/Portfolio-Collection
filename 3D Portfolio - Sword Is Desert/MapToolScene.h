#pragma once
#include "Scene.h"
#include "cTerrain.h"
#include "Day.h"
#include "cBaseObject.h"
#include "MapDataSaveLoad.h"

class UIMapToolWIndow;

class MapToolScene :
	public Scene
{
private:
	//기본
	cTerrain* _map;
	Day* _day;

	//맵 이동
	Transform* _mapViewPoint;
	float _dist;

	POINT prev;

	float _rx, _ry;

	//맵 클릭
	Transform* _lastClickTrans;
	Ray	_lastClickRay;

	//오브젝트
	std::vector<std::pair<std::string, cBaseObject*>> _vecObj; //배치된 오브젝트
	std::vector<std::string> _vecObjName; //로딩 된 오브젝트 이름들

	//플레이어
	cBaseObject* _player;
	std::string _playerName;

	//몬스터
	std::vector<std::pair<std::string, cBaseObject*>> _vecMon;
	std::vector<std::string> _vecMonName;

	//NPC
	std::vector<std::pair<std::string, cBaseObject*>> _vecNpc;
	std::vector<std::string> _vecNpcName;

	//선택된 오브젝트
	enum { PLAYER, MONSTER, OBJECT, NPC, NONE } _selectType = NONE;
	cBaseObject* _selectObject;
	bool _isPlayerSelect; //플레이어가 선택되면 삭제가 불가능 해야 한다.

	//맵 정보 저장, 로드
	MapDataSaveLoad _mapDataSL;

	//ui
	UIMapToolWIndow* _ui;

	//세이브 로드 작업중?
	bool _isSavingLoading = false;
public:
	MapToolScene();
	virtual ~MapToolScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();

	void LoadResources();

	void MapMoveUpdate();
	void RayClickUpdate();
	void SelectUpdate();

	void CreateObject(std::string name);
	void CreateMonster(std::string name);
	void CreateNPC(std::string name);
	void Delete();

	std::string GetObjectName(UINT idx);
	std::string GetMonsterName(UINT idx);
	std::string GetNPCName(UINT idx);

	void SaveData();
	void LoadData();
	void Clear();

	virtual LRESULT CALLBACK WndProc(HWND h, UINT iMessage, WPARAM wParam, LPARAM lParam);

	friend class UIMapToolDeligate;
};

#include "UIWindow.h"
#include "UITextBox.h"

class UIMapToolWIndow : public UIWindow
{
public:
	MapToolScene* _scene;
public:
	virtual void Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render(LPD3DXSPRITE sprite);
};

class UIMapToolDeligate : public UIDeligate
{
public:
	MapToolScene* _scene;
public:
	virtual UINT OnClickDown(UIWindow* caller, UIWindow* root) override;
};