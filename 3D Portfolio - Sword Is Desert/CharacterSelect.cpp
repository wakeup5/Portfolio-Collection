#include "stdafx.h"
#include "CharacterSelect.h"


CharacterSelect::CharacterSelect()
{
}


CharacterSelect::~CharacterSelect()
{
}
HRESULT CharacterSelect::Setup()
{

	this->_pMesh = RESOURCE_STATICXMESH->GetResource("../Resources/Textures/Ch_Select/Ch_Background.x");
	
	pTransform = new Transform;
	pTransform->SetWorldPosition(0, 0, 0);
	pTransform->SetScale(10, 10, 10);

	_ui = new UIWindow;
	_ui->SetBackground("../Resources/Textures/opening/CHSELRECT.png");
	_ui->SetHiddenState(false);
	_ui->SetSize(WINSIZE_X, WINSIZE_Y);
	_ui->SetPosition(500, 0);
	SPRITE_MGR->FadeInBlack(3.0);
	//time = 8;

	PLAYER_MGR->Setup();
	_player = PLAYER_MGR->GetPlayer();

	//CAMERA->IsBoundPlayer = false;
	CAMERA_MGR->SetNPC();
	CAMERA->SetPos(D3DXVECTOR3(1.5, 2, -5));
	CAMERA->SetLook(D3DXVECTOR3(1.5, 2, 1));
	CAMERA->SetFov(RAD(85));

	cLight_Direction light;
	light.Intensity = 5;
	light.pTransform->LookDirection(D3DXVECTOR3(0, -1, -1));
	light.Color = 0xffffffff;

	cXMesh_Skinned::SetBaseLight(&light);
		
	//pTransform->LookPosition(-CAMERA->GetWorldPosition());

	return S_OK;
}
void CharacterSelect::Release()
{
	_ui->Release();
	//_player->Release();
	//delete _player;

	//PLAYER_MGR->Release();
	//PlayerManager::ReleaseInstance();
}
void CharacterSelect::Update(float timeDelta)
{
	_player->animation->Update(timeDelta);

	_ui->Update();

	if (KEY_MGR->IsOnceDown(VK_LBUTTON))
	{
		SCENE_MGR->Change("loading");
	}
}
void CharacterSelect::Render()
{
	//cXMesh_Skinned::SetCamera(CAMERA);

	_player->Render();
	this->_pMesh->Render(pTransform);

	_ui->Render(SPRITE);
}