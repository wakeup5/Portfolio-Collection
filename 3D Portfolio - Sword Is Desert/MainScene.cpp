#include "stdafx.h"
#include "MainScene.h"
#include "UIScreen.h"
#include "UISkill.h"
#include "UIInventory.h"
#include "UIMouseSocket.h"
#include "UIDialog.h"
#include "UIEquippedItem.h"
#include "UIShop.h"
#include "UIQuest.h"
#include "Day.h"
#include "MapDataSaveLoad.h"
#include "PostEffect.h"
#include "MapView.h"
#include "cQuadParticleEmitter.h"

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

HRESULT MainScene::Setup()
{
	//*
	MonsterManager::LoadMonsters();
	NPCManager::LoadNPC();

	MONSTER_MGR->Setup();
	NPC_MGR->Setup();
	TERRAIN_MGR->Setup();

	PLAYER_MGR->Setup();
	
	LOG_MGR->AddLog("MainScene - 매니저 셋업");

	MONSTER_MGR->SetPlayer(PLAYER_MGR->GetPlayer());

	//Terrain
	
	TerrainManager::GetInstance()->Create(
		"worldMap",
		"../Resources/Map/asd.bmp",
		"../Resources/map/road.jpg",
		"../Resources/map/grass2.jpg",
		"../Resources/map/sahara.jpg",
		"../Resources/map/grass1.jpg",
		"../Resources/Map/split1.png",
		3, 350, 10, 200
		);
	
	LOG_MGR->AddLog("MainScene - UI");
	//*/

	MONSTER_MGR->SetPlayer(PLAYER_MGR->GetPlayer());

	//UI 셋팅
	_ui = new UIScreen; _ui->Setup(PLAYER_MGR->GetPlayer());
	_uiInven = new UIInventory;
	_uiInven->Setup(PLAYER_MGR->GetInventory(), PLAYER_MGR->GetEquipment());
	_ui->SetInventory(_uiInven);
	_uiSkill = new UISkill; _uiSkill->Setup();
	_uiEquip = new UIEquippedItem; _uiEquip->Setup(PLAYER_MGR->GetEquipment());
	_uiShop = new UIShop; _uiShop->Setup();
	_uiShop->SetInventory(_uiInven);
	_uiQuest = new UIQuest; _uiQuest->Setup();
	_uiMouseSocket = new UIMouseSocket; _uiMouseSocket->Setup();
	_uiDialog = new UIDialog; _uiDialog->Setup();
	_uiDialog->SetShop(_uiShop);
	_uiDialog->SetInventory(_uiInven);

	UIWindow::GetRoot()->SetDrawBoundingState(false);
	UIWindow::GetRoot()->SetHiddenState(true);
	_ui->SetHiddenState(false);

	//NPC 매니저 셋팅
	LOG_MGR->AddLog("MainScene - NPC");

	NPC_MGR->SetPlayer(PLAYER_MGR->GetPlayer());
	NPC_MGR->SetTerrain(TERRAIN_MGR->GetTerrain("worldMap"));
	NPC_MGR->SetUI(_uiDialog);

	//맵 저장 정보 로딩
	LOG_MGR->AddLog("MainScene - 맵 로딩");
	MapDataSaveLoad mdsl;
	mdsl.LoadData("test.xml");
	LOG_MGR->AddLog("MainScene - 플레이어 정보 셋팅");
	mdsl.GetPlayerData(PLAYER_MGR->GetPlayer()); //플레이어 정보
	LOG_MGR->AddLog("MainScene - 맵 오브젝트 셋팅");
	mdsl.GetObjectData(&_mapObj); //오브젝트 정보
	LOG_MGR->AddLog("MainScene - 몬스터 생성");
	mdsl.CreateMonster(); //몬스터 생성
	LOG_MGR->AddLog("MainScene - NPC 생성");
	mdsl.CreateNpc(); 	//NPC 생성
	//PLAYER_MGR->GetPlayer()->pTransform->SetWorldPosition(0, 0, 0);

	//Day Time
	LOG_MGR->AddLog("MainScene - 날씨");
	
	_day = new Day;
	_day->Setup();

	//카메라 기본 셋팅
	LOG_MGR->AddLog("MainScene - 카메라 셋팅");

	//포스트 이펙트
	LOG_MGR->AddLog("MainScene - 포스트 이펙트 생성");

	_postEffect = new PostEffect;

	//월드맵
	LOG_MGR->AddLog("MainScene - 월드맵 셋팅");

	_map = new MapView;
	_map->Setup();
	_map->SetDay(_day);

	//마지막으로 화면 페이드 인
	SPRITE_MGR->FadeInBlack(5);

	CAMERA_MGR->SetPlayer();

	LOG_MGR->AddLog("MainScene - 셋업 완료");

	SOUND_MGR->initialize();
	SOUND_MGR->loadDefaultSound();
	SOUND_MGR->play("bgm",0.4);

	_lightCamera = new Camera;
	_lightCamera->OrthoToggle();
	_lightCamera->ReadyShadowTexture(2048);

	SEParam param;
	//param.MinSpeed = D3DXVECTOR3(0, 1, 0);
	//param.MaxSpeed = D3DXVECTOR3(0, 1, 0);
	param.MinRot = D3DXVECTOR3(0, RAD(90), 0);
	param.MaxRot = D3DXVECTOR3(0, RAD(90), 0);
	
	return S_OK;
}

void MainScene::Release()
{
	PLAYER_MGR->ReleaseInstance();
	MONSTER_MGR->ReleaseInstance();
	NPC_MGR->ReleaseInstance();
	SOUND_MGR->release();
	_map->Release();
}

void MainScene::Update(float timeDelta)
{
	_map->Update();

	PLAYER_MGR->Update(timeDelta);
	//Day Time
	_day->Update();
	
	if (_map->IsOpen()) return;

	
	MONSTER_MGR->Update(timeDelta);
	NPC_MGR->Update(timeDelta);

	//몬스터 플레이어 충돌처리
	MONSTER_MGR->PlayerCollision(PLAYER_MGR->GetPlayer());
	//몬스터 지형 충돌
	MONSTER_MGR->TerrainCollision(TERRAIN_MGR->GetTerrain("worldMap"));

	//맵 높이
	//플레이어 높이
	TERRAIN_MGR->SetHeight("worldMap", PLAYER_MGR->GetPlayer());
	D3DXVECTOR3 frontPos = PLAYER_MGR->GetPlayer()->pTransform->GetWorldPosition() + PLAYER_MGR->GetPlayer()->pTransform->GetForward();
	TERRAIN_MGR->SetHeight("worldMap", &frontPos, &PLAYER_MGR->GetPlayer()->_frontHeight);
	//펫 높이
	TERRAIN_MGR->SetHeight("worldMap", PLAYER_MGR->GetPet());
	//용병 높이
	TERRAIN_MGR->SetHeight("worldMap", (Pet*)PLAYER_MGR->GetMercenary());

	//UI
	if(_ui != NULL) _ui->Update();
	if (_uiMouseSocket != NULL)_uiMouseSocket->Update();
	if (_uiSkill != NULL)_uiSkill->Update();
	if (_uiInven != NULL)_uiInven->Update();
	if (_uiShop != NULL)_uiShop->Update();
	if (_uiQuest != NULL)_uiQuest->Update();
	if (_uiEquip != NULL)_uiEquip->Update();
	if (_uiDialog != NULL)_uiDialog->Update();

	UIShopButton::GetInstance()->Update(timeDelta);
	UIAlram::GetInstance()->Update(timeDelta);
	ShopManager::GetInstance()->Update(timeDelta);

	if (isInitDialog){
		_uiDialog->Release();
		SAFE_DELETE(_uiDialog);
		_uiDialog = new UIDialog;
		_uiDialog->Setup();
		_uiDialog->SetHiddenState(true);
		_uiDialog->SetDrawBoundingState(false);
		isInitDialog = false;
		NPCManager::GetInstance()->SetUI(_uiDialog);
		_uiDialog->SetShop(_uiShop);
		_uiDialog->SetInventory(_uiInven);

		_uiQuest->Release();
		SAFE_DELETE(_uiQuest);
		_uiQuest = new UIQuest;
		_uiQuest->Setup();
		_uiQuest->SetDrawBoundingState(false);
	}
	else{
		if (_uiDialog != NULL)_uiDialog->Update();
	}

	_map->Update();

	if (KEY_MGR->IsOnceDown('O'))
	{
		CAMERA->SetPos(D3DXVECTOR3(0, 100, 100), D3DXVECTOR3(0, 100, -100), 1);
		//CAMERA->SetLook(D3DXVECTOR3(10, 100, 10), D3DXVECTOR3(0, 100, -101), 1);
	}

	SOUND_MGR->update();

	//화면 맵 정보 셋팅
	_ui->SetMapInfo(_map->GetNearBaseName(PLAYER_MGR->GetPlayer()->pTransform->GetWorldPosition()), _day->GetTime());
}

void MainScene::Render()
{
	if (_map->IsOpen())
	{
		_day->Render();

		TERRAIN_MGR->GetTerrain("worldMap")->Render(CAMERA, _day->GetDayLight(), _day->GetNightLight());
		
		for (int i = 0; i < _mapObj.size(); i++)
		{
			_mapObj[i]->Render();
		}

		_map->Render();

		return;
	}

	//블러 & DOF 출력
	if (GAMECONFIG->GetIsDof())
	{
		_postEffect->BeginBase();
		CoreRender();
		_postEffect->EndBase();

		_postEffect->RenderDof();
		//_postEffect->RenderMotionBlur();
	}
	else
	{
		//_postEffect->BeginBase();
		CoreRender();
		//_postEffect->EndBase();

		//_postEffect->RenderBase();
	}
	_map->EffectRender();

	//이펙트
	//태양
	_postEffect->BeginSunlight();
	_day->SunlightRender();
	_postEffect->EndSunlight();
	_postEffect->RenderSunlightBlur();

	//비
	_postEffect->BeginRain();
	_day->_rain->Render();
	_postEffect->EndRain();
	_postEffect->RenderRain();

	//_postEffect->ShowTexture(_postEffect->_rainNormal);
	//이펙트 end
	//UI
	UIRender();

}

void MainScene::CoreRender()
{
	_day->Render();
	//그림자!
	if (_day->GetDayLightAngle() < RAD(180))
	{
		_lightCamera->SetWorldPosition(PLAYER_MGR->GetPlayer()->pTransform->GetWorldPosition() - _day->GetDayLight()->pTransform->GetForward() * 20);
		_lightCamera->SetFov(RAD(60));
		_lightCamera->LookDirection(_day->GetDayLight()->pTransform->GetForward());

		_lightCamera->UpdateMatrix();

		cXMesh_Skinned::SetTechniqueName("CreateShadow");
		cXMesh_Skinned::SetCamera(_lightCamera);
		cXMesh_Static::sStaticMeshEffect->SetTechnique("CreateShadow");
		cXMesh_Static::SetCamera(_lightCamera);
		cTerrain::m_pTerrainEffect->SetTechnique("CreateShadow");
		cTerrain::SetCamera(_lightCamera);


		_lightCamera->RenderTextureBegin(0xffffffff);
		DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//TERRAIN_MGR->GetTerrain("worldMap")->RenderShadow(_lightCamera);
		//for (int i = 0; i < _mapObj.size(); i++)
		//{
		//	_mapObj[i]->Render();
		//}
		PLAYER_MGR->Render();
		MONSTER_MGR->Render();
		NPC_MGR->Render();
		DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		_lightCamera->RenderTextureEnd();


		cXMesh_Skinned::SetTechniqueName("ReciveShadow");
		cXMesh_Skinned::SetCamera(CAMERA);
		cXMesh_Static::sStaticMeshEffect->SetTechnique("ReciveShadow");
		cXMesh_Static::SetCamera(CAMERA);
		cTerrain::m_pTerrainEffect->SetTechnique("ReciveShadow");
		cTerrain::SetCamera(CAMERA);
		TERRAIN_MGR->GetTerrain("worldMap")->Render(CAMERA, _day->GetDayLight(), _day->GetNightLight(), _lightCamera);
	}
	else
	{
		TERRAIN_MGR->GetTerrain("worldMap")->Render(CAMERA, _day->GetDayLight(), _day->GetNightLight());
	}
	

	NPC_MGR->Render();

	for (int i = 0; i < _mapObj.size(); i++)
	{
		_mapObj[i]->Render();
	}

	NPC_MGR->RenderNameTag();

	if (!_uiDialog->IsExistNPC())
	{
		MONSTER_MGR->Render();
		PLAYER_MGR->Render();
	}

	//_day->_rain->Render();
	//EFFECT_MGR->Render();
}

void MainScene::UIRender()
{
	if (!_uiDialog->IsExistNPC())
	{
		if(_ui != NULL)_ui->Render(SPRITE);
		if (_uiSkill != NULL)_uiSkill->Render(SPRITE);
		if (_uiQuest != NULL)_uiQuest->Render(SPRITE);
		if (_uiEquip != NULL)_uiEquip->Render(SPRITE);
		
		MONSTER_MGR->NameTagRender();
	}

	if (_uiDialog != NULL)_uiDialog->Render(SPRITE);
	if (_uiShop != NULL)_uiShop->Render(SPRITE);
	if (_uiInven != NULL)_uiInven->Render(SPRITE);
	if (UIShopButton::GetInstance()!=NULL)UIShopButton::GetInstance()->Render(SPRITE);
	if (_uiMouseSocket != NULL)_uiMouseSocket->Render(SPRITE);
	//_uiLevel->Render(SPRITE);
	UIAlram::GetInstance()->Render(SPRITE);
}
