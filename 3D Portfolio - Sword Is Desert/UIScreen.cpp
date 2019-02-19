#include "stdafx.h"
#include "UIScreen.h"
#include "Skill.h"
#include "Item.h"
#include "UIInventory.h"
#include "UILevel.h"

UIScreen::UIScreen()
{
}


UIScreen::~UIScreen()
{

}

void UIScreen::Setup(Player* player)
{
	UIWindow::Setup();

	_player = player;

	//UICommon.h에 각 UI태그 넘버 셋팅 되어있음.
	//화면 UI 셋팅
	this->SetTagNum(UITag::UI_SCREEN);

	//우측 하단 셋팅
	_uiIconDeligate = new UIScreenIconDeligate;

	_uiIconInventory = new UIButton;
	_uiIconInventory->Setup();
	_uiIconInventory->SetButtonTexture(
		"../Resources/Textures/Icon/Invectory_up.png", 
		"../Resources/Textures/Icon/Invectory_over.png", 
		"../Resources/Textures/Icon/Invectory_down.png");
	_uiIconInventory
		->AttachTo(this)
		->AttackToWindow(this)
		->SetTagNum(UITag::UI_SCREEN_ICON_INVENTORY)
		->SetDeligate(_uiIconDeligate)
		->SetSize(45, 45)
		->SetPosition(WINSIZE_X - 200, WINSIZE_Y - 50);
	

	_uiIconQuest = new UIButton;
	_uiIconQuest->Setup();
	_uiIconQuest->SetButtonTexture(
		"../Resources/Textures/Icon/quest_up.png",
		"../Resources/Textures/Icon/quest_over.png",
		"../Resources/Textures/Icon/quest_down.png");
	_uiIconQuest
		->AttachTo(this)
		->AttackToWindow(this)
		->SetTagNum(UITag::UI_SCREEN_ICON_QUEST)
		->SetDeligate(_uiIconDeligate)
		->SetSize(45, 45)
		->SetPosition(WINSIZE_X - 150, WINSIZE_Y - 50);

	_uiIconSkill = new UIButton;
	_uiIconSkill->Setup();
	_uiIconSkill->SetButtonTexture(
		"../Resources/Textures/Icon/skill_up.png",
		"../Resources/Textures/Icon/skill_over.png",
		"../Resources/Textures/Icon/skill_down.png");
	_uiIconSkill
		->AttachTo(this)
		->AttackToWindow(this)
		->SetTagNum(UITag::UI_SCREEN_ICON_SKILL)
		->SetDeligate(_uiIconDeligate)
		->SetSize(45, 45)
		->SetPosition(WINSIZE_X - 100, WINSIZE_Y - 50);

	_uiIconExit = new UIButton;
	_uiIconExit->Setup();
	_uiIconExit->SetButtonTexture(
		"../Resources/Textures/Icon/exit_up.png",
		"../Resources/Textures/Icon/exit_over.png",
		"../Resources/Textures/Icon/exit_down.png");
	_uiIconExit
		->AttachTo(this)
		->AttackToWindow(this)
		->SetTagNum(UITag::UI_SCREEN_ICON_EXIT)
		->SetDeligate(_uiIconDeligate)
		->SetSize(45, 45)
		->SetPosition(WINSIZE_X - 50, WINSIZE_Y - 50);

	//좌측 상단
	_defualtDeligate = new UIDeligate;

	_uiExp = new UIProgress;
	_uiExp->Setup();
	_uiExp->SetBarTexture(
		"../Resources/Textures/bar/exp/exp_bar.png",
		"../Resources/Textures/bar/exp/exp_bar_back.png");
	_uiExp
		->AttachTo(this)
		->AttackToWindow(this)
		->SetTagNum(UITag::UI_SCREEN_EXP)
		->SetDeligate(_defualtDeligate)
		->SetSize(WINSIZE_X, 5)
		->SetPosition(0, 2);

	_uiExp->SetMax(1);
	_uiExp->SetCurrent(0);

	_uiHpBar = new UIProgress;
	_uiHpBar->Setup();
	_uiHpBar->SetBarTexture(
		"../Resources/Textures/bar/hp_bar.png",
		"../Resources/Textures/bar/hp_back.png");
	_uiHpBar
		->AttachTo(this)
		->AttackToWindow(this)
		->SetTagNum(UITag::UI_SCREEN_HPBAR)
		->SetDeligate(_defualtDeligate)
		->SetSize(300, 15)
		->SetPosition((WINSIZE_X / 2) - 150, WINSIZE_Y - 180);

	_uiMpBar = new UIProgress;
	_uiMpBar->Setup();
	_uiMpBar->SetBarTexture(
		"../Resources/Textures/bar/exp_bar.png",
		"../Resources/Textures/bar/exp_back.png");
	_uiMpBar
		->AttachTo(this)
		->AttackToWindow(this)
		->SetTagNum(UITag::UI_SCREEN_MPBAR)
		->SetDeligate(_defualtDeligate)
		->SetSize(296, 10)
		->SetPosition((WINSIZE_X / 2) - 148, WINSIZE_Y - 150);

	UIScreenSocketDeligate* screenSocketDeligate = new UIScreenSocketDeligate;

	for (int i = 0; i < 10; i++)
	{
		_skillSocket[i] = new UISocket;
		_skillSocket[i]->Setup();
		_skillSocket[i]
			->AttachTo(this)
			->AttackToWindow(this)
			->SetBackground("../Resources/Textures/Icon/socket/item_slot.png")
			->SetTagNum(SCREEN_SOCKET(i))
			->SetDeligate(screenSocketDeligate)
			->SetSize(40, 40)
			->SetPosition((WINSIZE_X / 2) + (4 - i) * 50, WINSIZE_Y - 75);
	}


	_uiLevel = new UILevel;
	_uiLevel->Setup();
	_uiLevel->SetHiddenState(false);
	_uiLevel->AttachTo(this);
	_uiLevel->AttackToWindow(this);

	_miniMapInfo = new UITextBox;
	_miniMapInfo->Setup();
	_miniMapInfo
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground(RESOURCE("Textures/Icon/MapInfo.png"))
		->SetSize(300, 50)->SetPosition(WINSIZE_X - 300, 0);
	_miniMapInfo->SetFontFormat(DT_CENTER | DT_VCENTER);

	this->SetDrawBoundingState(false);
	
	UIWindow* icon = new UIWindow;
	icon->Setup();
	icon->AttachTo(this)->AttackToWindow(this)
		->SetBackground(RESOURCE("Textures/Icon/Merc.png"))
		->SetPosition(10, 110)
		->SetSize(80, 80);

	icon = new UIWindow;
	icon->Setup();
	icon->AttachTo(this)->AttackToWindow(this)
		->SetBackground(RESOURCE("Textures/Icon/Pet.png"))
		->SetPosition(90, 110)
		->SetSize(80, 80);

	icon = new UIWindow;
	icon->Setup();
	icon->AttachTo(this)->AttackToWindow(this)
		->SetBackground(RESOURCE("Textures/Icon/ride.png"))
		->SetPosition(170, 110)
		->SetSize(80, 80);

	//미니맵
	_minimap = new UIMiniMap;
	_minimap->Setup();
	_minimap->SetBackground(RESOURCE("Textures/Minimap/minimapButton.png"));
	_minimap->AttachTo(this)->AttackToWindow(this)
		->SetSize(300, 300)->SetPosition(WINSIZE_X - 300, 50);

	_minimap->SetMapTexture(
		RESOURCE("Textures/Minimap/minimap.png"), 
		RESOURCE("Textures/Minimap/sight.png"), 
		RESOURCE("Textures/Minimap/minimap_monster.png"),
		RESOURCE("Textures/Icon/npc.png"));
}


void UIScreen::SetInventory(UIInventory* inven){
	_uiInven = inven;
}


void UIScreen::Render(LPD3DXSPRITE sprite){
	//if (_uiLevel != NULL) _uiLevel->Render(sprite);
	UIWindow::Render(sprite);
}

void UIScreen::SetMapInfo(std::string areaName, float time)
{
	std::string info;
	info.append(areaName + "  ");
	info.append(" 검은사막 싱글 플레이 ");

	UINT hour = 6;
	UINT minite;

	//time이 0 ~ 2사이로 오는데 1 이상이면 밤, 아니면 낮
	hour += (time / 2) * 24;
	if (hour > 24) hour -= 24;

	int minTime = time * 720;
	minite = minTime % 60;

	if (hour > 12)
	{
		hour -= 12;
		info.append("  PM " + std::to_string(hour) + " : " + std::to_string(minite));
	}
	else
	{
		info.append("  AM " + std::to_string(hour) + " : " + std::to_string(minite));
	}

	_miniMapInfo->SetText(info);
}


void UIScreen::Update()
{
	UIWindow::Update();

	CharacterInfo info = _player->GetTotalStatus();

	_uiExp->SetCurrent(info.Exp);

	_uiHpBar->SetMax(info.MaxHp);
	_uiHpBar->SetCurrent(info.Hp);
	_uiMpBar->SetMax(info.MaxMp);
	_uiMpBar->SetCurrent(info.Mp);

	//소켓 스킬 사용

	if (KEY_MGR->IsOnceDown('1')) ActiveSocket(9);
	if (KEY_MGR->IsOnceDown('2')) ActiveSocket(8);
	if (KEY_MGR->IsOnceDown('3')) ActiveSocket(7);
	if (KEY_MGR->IsOnceDown('4')) ActiveSocket(6);
	if (KEY_MGR->IsOnceDown('5')) ActiveSocket(5);
	if (KEY_MGR->IsOnceDown('6')) ActiveSocket(4);
	if (KEY_MGR->IsOnceDown('7')) ActiveSocket(3);
	if (KEY_MGR->IsOnceDown('8')) ActiveSocket(2);
	if (KEY_MGR->IsOnceDown('9')) ActiveSocket(1);

	//if (_uiLevel != NULL)_uiLevel->Update();

	if (KEY_MGR->IsOnceDown(VK_NUMPAD0))
	{
		Toggle();
		_uiLevel->Toggle();
	}
	if (KEY_MGR->IsOnceDown(VK_NUMPAD1))
	{
		SetHiddenState(false);
		_uiLevel->SetHiddenState(false);
	}
}

void UIScreen::ActiveSocket(UINT num)
{
	if (num > 9) return;

	SocketObject* obj = _skillSocket[num]->GetSocketObject();

	if (!obj) return;

	if (obj->GetType() == ST_SKILL)
	{
		((Skill*)obj)->Active(_player);
	}
	else
	{
		//((Item*)obj); //아이템 사용
		LOG_MGR->AddLog("백그라운드체크 : %d", num);
	//	_skillSocket[num]->SetBackground("../Resources/Textures/Icon/socket/item_slot.png");
		_skillSocket[num]->SetSocketObject(NULL);
		_uiInven->UseItem(num);
		
	}
}
