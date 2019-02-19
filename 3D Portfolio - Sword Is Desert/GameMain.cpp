#include "stdafx.h"
#include "GameMain.h"
#include "TestScene.h"
#include "Opening.h"
#include "MapToolScene.h"
#include "LoadingScene.h"
#include "MainScene.h"
#include "CharacterSelect.h"

GameMain::GameMain()
{
	SetRandomSeed();
}


GameMain::~GameMain()
{
}

HRESULT GameMain::Setup()
{

	TIME_MGR->Setup();
	LOG_MGR->Setup(LOG_WINDOW, hWnd, "Dump");

	DEVICEMANAGER->Setup();
	DXFONT_MGR->Setup(DEVICE);
	GIZMO_MGR->Setup(DEVICE);

	TEXTURE_MGR->Setup(DEVICE);
	SPRITE_MGR->Setup(DEVICE);
	CAMERA;
	EFFECT_MGR->Setup();

	GAMECONFIG;

	SCENE_MGR->Add("Opening", new Opening());
	SCENE_MGR->Add("CharacterSelect", new CharacterSelect());
	SCENE_MGR->Add("loading", new LoadingScene("MainScene"));
	SCENE_MGR->Add("mapTool", new MapToolScene);
	SCENE_MGR->Add("MainScene", new MainScene);
	SCENE_MGR->Add("T", new TestScene);
	SCENE_MGR->Change("MainScene");

	return S_OK;
}

void GameMain::Release()
{
	//GIZMO_MGR->Release();
	GizmoManager::ReleaseInstance();
	//TIME_MGR->Release();
	TimeMgr::ReleaseInstance();
	//KEY_MGR->Release();
	KeyMgr::ReleaseInstance();
	//LOG_MGR->Release();
	LogMgr::ReleaseInstance();
	//DXFONT_MGR->Release();
	DXFontManager::ReleaseInstance();
	//DEVICEMANAGER->Release();
	Device::ReleaseInstance();

	SpriteManager::ReleaseInstance();

	SceneManager::ReleaseInstance();

	//옵매니저
	//MonsterManager::GetInstance()->Release();
	//MonsterManager::ReleaseInstance();
	//NPCManager::GetInstance()->Release();
	//NPCManager::ReleaseInstance();

	UIWindow::GetRoot()->Release();

	GameConfig::ReleaseInstance();
	EffectManager::ReleaseInstance();

	cResourceMgr_XStaticMesh::GetInstance()->ClearResource();
	cResourceMgr_XStaticMesh::ReleaseInstance();
	cResourceMgr_XSkinnedMesh::GetInstance()->ClearResource();
	cResourceMgr_XSkinnedMesh::ReleaseInstance();
	cResourceMgr_Texture::GetInstance()->ClearResource();
	cResourceMgr_Texture::ReleaseInstance();
	cResourceMgr_ShaderFX::GetInstance()->ClearResource();
	cResourceMgr_ShaderFX::ReleaseInstance();
}

void GameMain::Update()
{
	//타임매니져 업데이트
	TIME_MGR->UpdateTime(60.0f);

	//한프레임 갱신 시간
	double timeDelta = TIME_MGR->GetFrameDeltaSec();

	TIME_MGR->Update(timeDelta);
	CAMERA->Update(timeDelta);
	SPRITE_MGR->Update(timeDelta);
	SCENE_MGR->Update(timeDelta);
	//EFFECT_MGR->Update(timeDelta);

	//UIWindow::GetRoot()->Update();
	//CAMERA->DefaultControl(timeDelta);

	//if (KEY_MGR->IsOnceDown('N'))
	//{
	//	SCENE_MGR->Change("test");
	//}

	//if (KEY_MGR->IsOnceDown('M'))
	//{
	//	SCENE_MGR->Change("mapTool");
	//}

	GAMECONFIG->Update();
}

void GameMain::Render()
{
	cXMesh_Static::SetCamera(CAMERA);
	cXMesh_Skinned::SetCamera(CAMERA);
	cTerrain::SetCamera(CAMERA);

	cXMesh_Skinned::SetTechniqueName("Base");
	//버퍼를 청소한다
	HRESULT result = DEVICE->Clear(
		0,							//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,						//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,		//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		0xff202020,					//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,						//깊이버퍼를 청소할값 ( 0 ~ 1, 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
			0						//스텐실 버퍼를 채울값
		);

	//버퍼 청소가 성공했다면.......
	if (SUCCEEDED(result))
	{
		//디바이스 랜더링 시작 명령
		DEVICE->BeginScene();

		//랜더링 명령

		//씬랜더
		SCENE_MGR->Render();
		
		
		
//#ifdef _DEBUG		//디버그 모드에서만 실행
		//디바이스 랜더링 종료 명령
		//월드 그리드
		//GIZMO_MGR->WorldGrid(1, 10);

		//타임정보 출력
		//TIME_MGR->DrawTimeInfo();
//#endif
		//UIWindow::GetRoot()->Render(SPRITE);
		SPRITE_MGR->Render();

		DEVICE->EndScene();

		////////////////////////////////

		////////////////////////////////
		//DEVICE->StretchRect(_sur, NULL, defaultSur, NULL, D3DTEXF_NONE);
		
		//랜더링된 버퍼내용을 화면에 전송
		DEVICE->Present(0, 0, 0, 0);
	}
}

LRESULT GameMain::WndProc(HWND h, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	}
	
	CAMERA->WndProc(h, iMessage, wParam, lParam);

	if (SCENE_MGR->GetCurrent()) return SCENE_MGR->GetCurrent()->WndProc(h, iMessage, wParam, lParam);

	//

	return DefWindowProc(h, iMessage, wParam, lParam);
}
