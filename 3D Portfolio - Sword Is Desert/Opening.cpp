#include "stdafx.h"
#include "Opening.h"
#include "UIWindow.h"
#include "UIButton.h"


UIButton* _beginBtn2;
UIWindow* _op_begin_btn;
RECT _op_rc;

Opening::Opening()
{
}

Opening::~Opening()
{
}

HRESULT Opening::Setup()
{
	//CAMERA->IsHiddenCursor = false;
	_ui = new UIWindow;
	_ui->SetBackground("../Resources/Textures/opening/Opening_01.jpg");
	_ui->SetHiddenState(false);
	_ui->SetSize(WINSIZE_X, WINSIZE_Y);
	
	OpeningDeligate* opdeli = new OpeningDeligate;
	_op_begin_btn = new UIWindow;
	_op_begin_btn->SetBackground("../Resources/Textures/opening/btn/op_start_btn.png");
	_op_begin_btn->SetHiddenState(true);
	_op_begin_btn->SetSize(110,25);
	_op_begin_btn->SetPosition(585, 540);
	//_op_begin_btn->SetDeligate(opdeli);

	/*
	left top: 585 540
	right bottom : 695 565
	*/
	_op_rc.left = 585;
	_op_rc.right = 695;
	_op_rc.top = 560;
	_op_rc.bottom = 585;

	time = 5;
	

	_beginBtn2 = new UIButton;
	_beginBtn2->Setup();
	_beginBtn2->SetButtonTexture(
		"../Resources/Textures/Icon/Button/begin.png",
		"../Resources/Textures/Icon/Button/begin.png",
		"../Resources/Textures/Icon/Button/begin_down.png");
	_beginBtn2
		->AttachTo(_ui)
		->AttackToWindow(_ui)
		->SetSize(151, 29)
		->SetPosition(100,100)
		->SetTagNum(UITag::UI_DIALOG_BEGIN);

	return S_OK;
}
void Opening::Release()
{
	_ui->Release();
	_op_begin_btn->Release();
}
void Opening::Update(float timeDelta)
{
	static bool s = false;
	if (!s)
	{
		if (KEY_MGR->IsOnceDown('1'))
		{
			SPRITE_MGR->FadeInBlack(3.0);
			s = true;
		}
		return;
	}

	if (time > 0)
	{
		time = time - timeDelta;

		/*
		if (KEY_MGR->IsOnceDown(VK_LBUTTON))
		{
		//	SCENE_MGR->Change("CharacterSelect");
			POINT pt;
			GetCursorPos(&pt);
			LOG_MGR->AddLog("%d:%d", pt.x, pt.y);
		}*/
	}
	if (time < 0)
	{
		_ui->SetBackground("../Resources/Textures/opening/Opening_02.jpg");
		
		SPRITE_MGR->FadeInBlack(5.0);
		time = INTMAX_MAX;

		_ui->SetSize(WINSIZE_X, WINSIZE_Y);
		_op_begin_btn->SetHiddenState(false);
		
	}

	POINT pt;
	GetCursorPos(&pt);
	if (PtInRect(&_op_rc, pt) && KEY_MGR->IsOnceDown(VK_LBUTTON)){
		//LOG_MGR->AddLog("Ãæµ¹");
		_op_begin_btn->SetBackground("../Resources/Textures/opening/btn/op_start_btn_click.png");
		SCENE_MGR->Change("CharacterSelect");
	}
	else{
		_op_begin_btn->SetBackground("../Resources/Textures/opening/btn/op_start_btn.png");
	}
}
void Opening::Render()
{
	//cXMesh_Skinned::SetCamera(CAMERA);
	_ui->Render(SPRITE);
	_op_begin_btn->Render(SPRITE);
//	_beginBtn2->Render(SPRITE);
}