#include "stdafx.h"
#include "MapToolScene.h"

bool ObjNearCamera(StrObjPair a, StrObjPair b)
{
	float da, db;
	da = D3DXVec3Length(&(a.second->pTransform->GetWorldPosition() - CAMERA->GetWorldPosition()));
	db = D3DXVec3Length(&(b.second->pTransform->GetWorldPosition() - CAMERA->GetWorldPosition()));

	return da < db;
}

MapToolScene::MapToolScene()
	: _selectObject(NULL)
{
}


MapToolScene::~MapToolScene()
{
}

HRESULT MapToolScene::Setup()
{

	PLAYER_MGR->Setup();

	CAMERA_MGR->SetMapView();
	CAMERA->SetFar(3000);
	CAMERA->SetFov(RAD(60), RAD(60), 0);
	LOG_MGR->AddLog("MapToolScene : 카메라 셋팅");

	LoadResources();

	LOG_MGR->AddLog("MapToolScene : 리소스 로딩");

	//플레이어 생성
	_player = new cBaseObject;
	_player->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(20, 40, 20));
	_player->pMesh = RESOURCE_SKINNEDXMESH->GetResource(_playerName);
	_player->SetActive(true);
	_player->pTransform->SetScale(0.05, 0.05, 0.05);

	LOG_MGR->AddLog("MapToolScene : 플레이어 생성");

	TerrainManager::GetInstance()->Create(
		"worldMap",
		"../Resources/Map/asd.bmp",
		"../Resources/map/sahara.jpg",
		"../Resources/map/road.jpg",
		"../Resources/map/grass2.jpg",
		"../Resources/map/grass1.jpg",
		"../Resources/Map/split1.jpg",
		3, 350, 10, 500
		);

	LOG_MGR->AddLog("MapToolScene : 맵 생성");

	_day = new Day;
	_day->Setup();

	LOG_MGR->AddLog("MapToolScene : 날씨 생성");

	_mapViewPoint = new Transform;

	_dist = 500;

	_ui = new UIMapToolWIndow;
	_ui->_scene = this;
	_ui->Setup();

	LOG_MGR->AddLog("MapToolScene : UI 샛팅");

	return S_OK;
}

void MapToolScene::Release()
{
	_day->Release();
	delete _day;

	_ui->Release();
	delete _ui;

	Clear();

	delete _player;
}

void MapToolScene::Update(float timeDelta)
{
	//오브젝트와 카메라 거리에 따라 순서 조정
	//std::sort(_vecObj.begin(), _vecObj.end(), ObjNearCamera);

	_day->Update();
	MapMoveUpdate();
	RayClickUpdate();
	SelectUpdate();

	_player->Update(timeDelta);

	if (KEY_MGR->IsOnceDown('P')) SaveData();
	if (KEY_MGR->IsOnceDown('L')) LoadData();
	//if (KEY_MGR->IsOnceDown('Y')) Clear();

	//보는 위치의 높이를 맵에 맞춘다.
	TerrainManager::GetInstance()->GetHeight("worldMap", _mapViewPoint);
	//플레이어 높이
	TerrainManager::GetInstance()->GetHeight("worldMap", _player->pTransform);
	//update
	_ui->Update();

	D3DXVECTOR3 pos = CAMERA->GetWorldPosition();
	LOG_MGR->AddLog("%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
}

void MapToolScene::Render()
{
	_day->Render();
	TerrainManager::GetInstance()->Render("worldMap", CAMERA);

	//Render
	_ui->Render(SPRITE);

	//if (_isSavingLoading) return;

	_player->Render();

	for (int i = 0; i < _vecObj.size(); i++)
	{
		_vecObj[i].second->Render();
	}

	for (int i = 0; i < _vecMon.size(); i++)
	{
		_vecMon[i].second->Render();
	}

	for (int i = 0; i < _vecNpc.size(); i++)
	{
		_vecNpc[i].second->Render();
	}

	if(_selectObject)
		_selectObject->BoundBox.RenderGizmo(_selectObject->pTransform, 0xffff0000);

	DXFONT_MGR->PrintText(std::to_string(_vecObj.size() + _vecMon.size() + _vecNpc.size() + 1) + " / 300", 10, WINSIZE_Y - 30, 0xffffffff);
}

void MapToolScene::LoadResources()
{
	RESOURCE_TEXTURE->GetResource("../Resources/Map/output2.bmp");
	RESOURCE_TEXTURE->GetResource("../Resources/Textures/i_on/df1_coral_1.jpg");
	RESOURCE_TEXTURE->GetResource("../Resources/Textures/i_on/df1_grass_2.jpg");
	RESOURCE_TEXTURE->GetResource("../Resources/Textures/i_on/df1_leaves_3.jpg");
	RESOURCE_TEXTURE->GetResource("../Resources/Textures/i_on/df1_snow_1.jpg");
	RESOURCE_TEXTURE->GetResource("../Resources/Textures/i_on/df1_snow_1.jpg");
	
	//오브젝트 로드 - 순서 중요
	_vecObjName.push_back(RESOURCE("map/meshes/tree/tree1.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Altor.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Article.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Barricade.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Campfire.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Castle.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Fence.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Gate.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Giant_Statue.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Giant_Wall.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Lamp.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Lamp02.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Macha.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Plane.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Plane02.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Plant.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Simbol.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Skull_Stack.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Stone.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Stone02.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Tent.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Tent02.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/tomb.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Tower.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/TreasureBox.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Wall_01.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Wall_02.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Well_02.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Wood_Box.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/BigHouse1.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/BigHouse2.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/BigHouse3.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/SideHouse.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/StonStairs.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Veranda1.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Veranda2.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Grass.x"));
	_vecObjName.push_back(RESOURCE("map/meshes/Object/Tree1.x"));
	_vecObjName.push_back(RESOURCE("map/Plant/Plant01.X"));
	_vecObjName.push_back(RESOURCE("map/Plant/Plant02.X")); 
	_vecObjName.push_back(RESOURCE("map/Plant/BigTree.X"));

	for (int i = 0; i < _vecObjName.size(); i++)
	{
		RESOURCE_STATICXMESH->GetResource(_vecObjName[i]);
	}

	/*
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/tree/tree1.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Altor.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Article.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Barricade.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Campfire.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Castle.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Fence.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Gate.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Giant_Statue.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Giant_Wall.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Lamp.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Lamp02.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Macha.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Plane.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Plane02.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Plant.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Simbol.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Skull_Stack.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Stone.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Stone02.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Tent.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Tent02.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/tomb.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Tower.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/TreasureBox.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Wall_01.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Wall_02.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Well_02.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Wood_Box.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/BigHouse1.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/BigHouse2.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/BigHouse3.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/SideHouse.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/StonStairs.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Veranda1.x"));
	RESOURCE_STATICXMESH->GetResource(RESOURCE("map/meshes/Object/Veranda2.x"));
	*/

	//플레이어 로드
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/player/MAINCHARACTER.x"));
	_playerName = RESOURCE("Meshes/player/MAINCHARACTER.x");

	//몬스터 로드
	MonsterManager::LoadMonsters();
	for (auto i = MonsterManager::MapMonster.begin(); i != MonsterManager::MapMonster.end(); ++i)
	{
		RESOURCE_SKINNEDXMESH->GetResource(i->second.ResourcePath);
		_vecMonName.push_back(i->first);
	}
	/*
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/Monster/Seth/Seth.X"));
	_vecMonName.push_back(RESOURCE("Meshes/Monster/Seth/Seth.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/Monster/muspel/muspel.X"));
	_vecMonName.push_back(RESOURCE("Meshes/Monster/muspel/muspel.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/Monster/orc/orc.X"));
	_vecMonName.push_back(RESOURCE("Meshes/Monster/orc/orc.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/Monster/pumpkin/pumpkin.X"));
	_vecMonName.push_back(RESOURCE("Meshes/Monster/pumpkin/pumpkin.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/Monster/skullWarrior/skullWarrior.X"));
	_vecMonName.push_back(RESOURCE("Meshes/Monster/skullWarrior/skullWarrior.X"));
	*/
	
	//NPC 로드
	NPCManager::LoadNPC();
	for (auto i = NPCManager::MapNPC.begin(); i != NPCManager::MapNPC.end(); ++i)
	{
		RESOURCE_SKINNEDXMESH->GetResource(i->second.ResourcePath);
		_vecNpcName.push_back(i->first);
	}
	/*
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/NPC/NPC01.X"));
	_vecNpcName.push_back(RESOURCE("Meshes/NPC/NPC01.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/NPC/NPC02.X"));
	_vecNpcName.push_back(RESOURCE("Meshes/NPC/NPC02.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/NPC/NPC03.X"));
	_vecNpcName.push_back(RESOURCE("Meshes/NPC/NPC03.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/NPC/NPC04.X"));
	_vecNpcName.push_back(RESOURCE("Meshes/NPC/NPC04.X"));
	RESOURCE_SKINNEDXMESH->GetResource(RESOURCE("Meshes/NPC/NPC05.X"));
	_vecNpcName.push_back(RESOURCE("Meshes/NPC/NPC05.X"));
	*/

}

void MapToolScene::MapMoveUpdate()
{
	D3DXVECTOR3 pos = _mapViewPoint->GetWorldPosition();
	D3DXVECTOR3 pPos = D3DXVECTOR3(0, 0, -1);

	if (KEY_MGR->IsStayDown(VK_LSHIFT))
	{
		if (KEY_MGR->IsOnceDown(VK_RBUTTON))
		{
			SetMousePos(WINSIZE_X / 2, WINSIZE_Y / 2);
		}

		if (KEY_MGR->IsStayDown(VK_RBUTTON))
		{
			//화면의 중심위치
			int screenCenterX = WINSIZE_X / 2;
			int screenCenterY = WINSIZE_Y / 2;

			//현재 마우스 위치
			POINT mousePos = GetMousePos();

			//이동량 ( 중앙에서 멀어진 량 )
			float deltaX = mousePos.x - screenCenterX;
			float deltaY = mousePos.y - screenCenterY;

			//앵글 추가
			_ry += deltaX * 0.33f;
			_rx += deltaY * 0.33f;

			//앵글값을 min max 범위 안으로
			_rx = Clamp(_rx, -85.0f, 85.0f);

			//다시 마우스 위치를 센터로...
			SetMousePos(screenCenterX, screenCenterY);
		}
	}
	else
	{
		//MAPVIEW_CAMERA->IsHiddenCursor = false;
		if (KEY_MGR->IsOnceDown(VK_RBUTTON))
		{
			prev = GetMousePos();
		}

		if (KEY_MGR->IsStayDown(VK_RBUTTON))
		{
			float power = _dist / 1000;

			POINT mPt = GetMousePos();

			float x = (prev.x - mPt.x) * power;
			float y = (prev.y - mPt.y) * power;

			_mapViewPoint->MovePositionWorld((MAPVIEW_CAMERA->GetRight() * x) + (-MAPVIEW_CAMERA->GetDirectForward() * y));

			D3DXVECTOR3 pos = _mapViewPoint->GetWorldPosition();

			if (pos.x > 1000) pos.x = 1000;
			if (pos.x < -1000) pos.x = -1000;
			if (pos.z > 1000) pos.z = 1000;
			if (pos.z < -1000) pos.z = -1000;

			_mapViewPoint->SetWorldPosition(pos);

			prev = mPt;
		}
	}

	//회전 처리
	D3DXMATRIXA16 mrx, mry, mr;
	D3DXMatrixRotationX(&mrx, RAD(_rx));
	D3DXMatrixRotationY(&mry, RAD(_ry));

	mr = mrx * mry;
	D3DXVec3TransformNormal(&pPos, &pPos, &mr);

	CAMERA->SetLook(pos);
	CAMERA->SetPos(pos + pPos * _dist);
}

void MapToolScene::RayClickUpdate()
{
	if (!_ui->IsMouseOver() && KEY_MGR->IsOnceDown(VK_LBUTTON))
	{
		POINT mPt = GetMousePos();
		MAPVIEW_CAMERA->ComputeRay(&_lastClickRay, &D3DXVECTOR2(mPt.x, mPt.y));

		if (KEY_MGR->IsStayDown(VK_LCONTROL))
		{
			cBaseObject* obj;
			//오브젝트
			for (int i = 0; i < _vecObj.size(); i++)
			{
				obj = _vecObj[i].second;

				if (PHYSICS_MGR->IsRayHitBound(&_lastClickRay, &obj->BoundBox, obj->pTransform, NULL, NULL))
				{
					_selectObject = obj;
					_isPlayerSelect = false;
					_selectType = OBJECT;
					return;
				}
			}
			//몬스터
			for (int i = 0; i < _vecMon.size(); i++)
			{
				obj = _vecMon[i].second;

				if (PHYSICS_MGR->IsRayHitBound(&_lastClickRay, &obj->BoundBox, obj->pTransform, NULL, NULL))
				{
					_selectObject = obj;
					_isPlayerSelect = false;
					_selectType = MONSTER;
					return;
				}
			}

			//플레이어
			if (PHYSICS_MGR->IsRayHitBound(&_lastClickRay, &_player->BoundBox, _player->pTransform, NULL, NULL))
			{
				_selectObject = _player;
				_isPlayerSelect = true;
				_selectType = PLAYER;
				return;
			}

			//NPC
			for (int i = 0; i < _vecNpc.size(); i++)
			{
				obj = _vecNpc[i].second;

				if (PHYSICS_MGR->IsRayHitBound(&_lastClickRay, &obj->BoundBox, obj->pTransform, NULL, NULL))
				{
					_selectObject = obj;
					_isPlayerSelect = false;
					_selectType = NPC;
					return;
				}
			}

			_selectObject = NULL;
			_isPlayerSelect = false;
			_selectType = NONE;
		}
		else
		{
			D3DXVECTOR3 pos;
			TerrainManager::GetInstance()->GetTerrain("worldMap")->IsIntersectRay(&pos, &_lastClickRay);

			if (_selectObject != NULL)
				_selectObject->pTransform->SetWorldPosition(pos);
		}
	}
	
}

void MapToolScene::SelectUpdate()
{
	if (!_selectObject) return;

	Transform* trans = _selectObject->pTransform;
	D3DXVECTOR3 scale = trans->GetScale();

	//사이즈
	if (KEY_MGR->IsStayDown('X')) trans->Scaling(scale * 0.02);
	if (KEY_MGR->IsStayDown('Z')) trans->Scaling(scale * -0.02);

	//회전
	if (KEY_MGR->IsStayDown('R')) trans->RotateSelf(0, -RAD(2), 0);
	if (KEY_MGR->IsStayDown('Y')) trans->RotateSelf(0, RAD(2), 0);
	if (KEY_MGR->IsStayDown('T')) trans->RotateSelf(-RAD(2), 0, 0);
	if (KEY_MGR->IsStayDown('G')) trans->RotateSelf(RAD(2), 0, 0);
	if (KEY_MGR->IsStayDown('F')) trans->RotateSelf(0, 0, -RAD(2));
	if (KEY_MGR->IsStayDown('H')) trans->RotateSelf(0, 0, RAD(2));

	//이동
	if (KEY_MGR->IsStayDown('W')) trans->MovePositionSelf(trans->GetForward());
	if (KEY_MGR->IsStayDown('S')) trans->MovePositionSelf(-trans->GetForward());
	if (KEY_MGR->IsStayDown('A')) trans->MovePositionSelf(-trans->GetRight());
	if (KEY_MGR->IsStayDown('D')) trans->MovePositionSelf(trans->GetRight());
	if (KEY_MGR->IsStayDown('Q')) trans->MovePositionSelf(trans->GetUp());
	if (KEY_MGR->IsStayDown('E')) trans->MovePositionSelf(-trans->GetUp());

	//클리어
	if (KEY_MGR->IsOnceDown('C'))
	{
		trans->SetScale(1, 1, 1);
		trans->SetRotateLocal(0, 0, 0);
	}

	if (KEY_MGR->IsOnceDown(VK_DELETE))
	{
		Delete();
	}
}

void MapToolScene::CreateObject(std::string name)
{
	cBaseObject* obj = new cBaseObject;

	obj->SetMesh(RESOURCE_STATICXMESH->GetResource(name));
	obj->SetActive(true);

	_selectObject = obj;

	_selectObject->pTransform->SetWorldPosition(_mapViewPoint->GetWorldPosition());
	
	_vecObj.push_back(std::make_pair(name, obj));
}

void MapToolScene::CreateMonster(std::string name)
{
	cBaseObject* obj = new cBaseObject;

	obj->SetMesh(RESOURCE_SKINNEDXMESH->GetResource(MonsterManager::MapMonster.find(name.c_str())->second.ResourcePath));
	obj->SetActive(true);
	obj->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(20, 2, 20));
	
	_selectObject = obj;

	_selectObject->pTransform->SetWorldPosition(_mapViewPoint->GetWorldPosition());

	_vecMon.push_back(std::make_pair(name, obj));
}

void MapToolScene::CreateNPC(std::string name)
{
	cBaseObject* obj = new cBaseObject;

	obj->SetMesh(RESOURCE_SKINNEDXMESH->GetResource(NPCManager::MapNPC.find(name.c_str())->second.ResourcePath));
	obj->SetActive(true);
	obj->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(20, 40, 20));

	_selectObject = obj;

	_selectObject->pTransform->SetWorldPosition(_mapViewPoint->GetWorldPosition());
	_selectObject->pTransform->SetScale(0.5f, 0.5f, 0.5f);

	_vecNpc.push_back(std::make_pair(name, obj));
}

void MapToolScene::Delete()
{
	if (_selectObject == NULL) return;

	switch (_selectType)
	{
	case PLAYER: case NONE:
		return;
	case OBJECT:
		for (int i = 0; i < _vecObj.size(); i++)
		{
			if (_vecObj[i].second != _selectObject) continue;

			_vecObj.erase(_vecObj.begin() + i);

			SAFE_DELETE(_selectObject);

			break;
		}
		return;
	case MONSTER:
		for (int i = 0; i < _vecMon.size(); i++)
		{
			if (_vecMon[i].second != _selectObject) continue;

			_vecMon.erase(_vecMon.begin() + i);

			SAFE_DELETE(_selectObject);

			break;
		}
		return;
	case NPC:
		for (int i = 0; i < _vecNpc.size(); i++)
		{
			if (_vecNpc[i].second != _selectObject) continue;

			_vecNpc.erase(_vecNpc.begin() + i);

			SAFE_DELETE(_selectObject);

			break;
		}
		return;
	}
}

std::string MapToolScene::GetObjectName(UINT idx)
{
	if (idx >= _vecObjName.size()) return "";

	return _vecObjName[idx];
}

std::string MapToolScene::GetMonsterName(UINT idx)
{
	if (idx >= _vecMonName.size()) return "";

	return _vecMonName[idx];
}

std::string MapToolScene::GetNPCName(UINT idx)
{
	if (idx >= _vecNpcName.size()) return "";

	return _vecNpcName[idx];
}

void MapToolScene::SaveData()
{
	if (_isSavingLoading) return;
	_isSavingLoading = true;

	_mapDataSL.SetObjectData(_vecObj);
	_mapDataSL.SetPlayerData(_player);
	_mapDataSL.SetMonsterData(_vecMon);
	_mapDataSL.SetNpcData(_vecNpc);

	_mapDataSL.SaveData("test4.xml");

	_isSavingLoading = false;
}

void MapToolScene::LoadData()
{
	if (_isSavingLoading) return;
	_isSavingLoading = true;

	Clear();
	_mapDataSL.LoadData("test4.xml");

	_mapDataSL.GetObjectData(&_vecObj);
	_mapDataSL.GetPlayerData(_player);
	_mapDataSL.GetMonsterData(&_vecMon);
	_mapDataSL.GetNpcData(&_vecNpc);

	_isSavingLoading = false;
}

void MapToolScene::Clear()
{
	_vecObj.clear();
	_vecMon.clear();
	_vecNpc.clear();

	_selectObject = NULL;
}

LRESULT MapToolScene::WndProc(HWND h, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage == WM_MOUSEWHEEL)
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			_dist -= _dist / 10;
		}
		else
		{
			_dist += _dist / 10;
		}

		if (_dist > 2000) _dist = 2000;
		if (_dist < 0) _dist = 0;
	}

	return DefWindowProc(h, iMessage, wParam, lParam);
}


////////////////////////////////////////// map tool ui //////////////////////////////////////////////////////////////
void UIMapToolWIndow::Setup()
{
	UIWindow::Setup();
	SetTagNum(UITag::UI_MAPTOOL);

	UIMapToolDeligate* d = new UIMapToolDeligate;
	d->_scene = _scene;

	//상위 탭
	UITextBox* objTab = new UITextBox;
	objTab->AttachTo(this)->AttackToWindow(this)
		->SetBackground(TEXTURE("UI/ui_background_small.png"))
		->SetSize(155, 30)->SetPosition(WINSIZE_X - 320, 20)->SetDeligate(d)
		->SetTagNum(UITag::UI_MAPTOOL_OBJECT_TAB);

	objTab->SetText("Object");
	objTab->SetFontFormat(DT_CENTER | DT_VCENTER);

	UITextBox* charTab = new UITextBox;
	charTab->AttachTo(this)->AttackToWindow(this)
		->SetBackground(TEXTURE("UI/ui_background_small.png"))
		->SetSize(155, 30)->SetPosition(WINSIZE_X - 160, 20)->SetDeligate(d)
		->SetTagNum(UITag::UI_MAPTOOL_CHARACTER_TAB);

	charTab->SetText("Character");
	charTab->SetFontFormat(DT_CENTER | DT_VCENTER);

	//오브젝트 탭
	UIWindow* objWin = new UIWindow;
	objWin->AttachTo(this)->AttackToWindow(this)
		->SetBackground(TEXTURE("UI/ui_background_small.png"))
		->SetSize(310, WINSIZE_Y - 70)->SetPosition(WINSIZE_X - 320, 60)->SetDeligate(d)
		->SetTagNum(UITag::UI_MAPTOOL_OBJECT_LIST);

	UITextBox* objs[50];

	for (int i = 0; i < 41; i++)
	{
		objs[i] = new UITextBox;
		objs[i]->AttachTo(objWin)->AttackToWindow(this)
			->SetBackground(TEXTURE("UI/ui_background_small.png"))
			->SetSize(90, 30)->SetPosition(10 + ((i % 3) * 100), 10 + ((i / 3) * 40))
			->SetDeligate(d)->SetTagNum(MAPTOOL_OBJ_TAG(i));
		objs[i]->SetFontFormat(DT_LEFT | DT_BOTTOM | DT_WORDBREAK);
		objs[i]->SetText(_scene->GetObjectName(i));
	}
	objWin->SetHiddenState(false);

	//캐릭터 탭
	UIWindow* charWin = new UIWindow;
	charWin->AttachTo(this)->AttackToWindow(this)
		->SetBackground(TEXTURE("UI/ui_background_small.png"))
		->SetSize(310, WINSIZE_Y - 70)->SetPosition(WINSIZE_X - 320, 60)->SetDeligate(d)
		->SetTagNum(UITag::UI_MAPTOOL_CHARACTER_LIST);

	UITextBox* chars[11];

	for (int i = 0; i < 11; i++)
	{
		chars[i] = new UITextBox;
		chars[i]->AttachTo(charWin)->AttackToWindow(this)
			->SetBackground(TEXTURE("UI/ui_background_small.png"))
			->SetSize(290, 50)->SetPosition(10, 10 + (i * 60))
			->SetDeligate(d)->SetTagNum(MAPTOOL_CHAR_TAG(i));
		chars[i]->SetFontFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	}
	charWin->SetHiddenState(true);


	chars[0]->SetText("Monster 1");
	chars[1]->SetText("Monster 2");
	chars[2]->SetText("Monster 3");
	chars[3]->SetText("Monster 4");
	chars[4]->SetText("Monster 5");
	chars[5]->SetText("Boss");
	chars[6]->SetText("NPC - 1");
	chars[7]->SetText("NPC - 2");
	chars[8]->SetText("NPC - 3");
	chars[9]->SetText("NPC - 4");
	chars[10]->SetText("NPC - 5");
}

void UIMapToolWIndow::Release()
{
	UIWindow::Release();
}

void UIMapToolWIndow::Update()
{
	UIWindow::Update();
}

void UIMapToolWIndow::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);
}

UINT UIMapToolDeligate::OnClickDown(UIWindow * caller, UIWindow * root)
{
	_scene->_selectObject = NULL;

	switch (caller->GetTagNum())
	{
	case UITag::UI_MAPTOOL_OBJECT_TAB:
		root->GetWindowByTag(UITag::UI_MAPTOOL_OBJECT_LIST)->SetHiddenState(false);
		root->GetWindowByTag(UITag::UI_MAPTOOL_CHARACTER_LIST)->SetHiddenState(true);
		return RETURN_OK;
	case UITag::UI_MAPTOOL_CHARACTER_TAB:
		root->GetWindowByTag(UITag::UI_MAPTOOL_OBJECT_LIST)->SetHiddenState(true);
		root->GetWindowByTag(UITag::UI_MAPTOOL_CHARACTER_LIST)->SetHiddenState(false);
		return RETURN_OK;
	}

	UINT tag = caller->GetTagNum();

	if (tag >= UITag::UI_MAPTOOL_OBJECT && tag < UITag::UI_MAPTOOL_OBJECT_END)
	{
		UINT objIdx = MAPTOOL_OBJ_IDX(tag);
		std::string objName = _scene->GetObjectName(objIdx);

		if (objName == "") return RETURN_NO;

		_scene->CreateObject(objName);
	}
	else if (tag >= UITag::UI_MAPTOOL_CHARACTER && tag < UITag::UI_MAPTOOL_CHARACTER_END)
	{
		UINT charIdx = MAPTOOL_CHAR_IDX(tag);

		if (charIdx < 6)
		{
			std::string objName = _scene->GetMonsterName(charIdx);
			if (objName == "") return RETURN_NO;
			_scene->CreateMonster(objName);
		}
		else
		{
			std::string objName = _scene->GetNPCName(charIdx - 6);
			if (objName == "") return RETURN_NO;
			_scene->CreateNPC(objName);
		}
	}

	return RETURN_OK;
}
