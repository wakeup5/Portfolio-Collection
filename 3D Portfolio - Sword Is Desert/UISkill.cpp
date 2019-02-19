#include "stdafx.h"
#include "UISkill.h"
#include "Skill.h"


UISkill::UISkill()
{
}


UISkill::~UISkill()
{
}

void UISkill::Setup()
{
	UIWindow::Setup();

	UISkillSocketDeligate* d = new UISkillSocketDeligate;

	SetDeligate(new UIDeligateDefault);
	SetPosition(300, 100);
	SetSize(300, 400);
	SetBackground("../Resources/Textures/ui/ui_background.png");
	SetTagNum(UITag::UI_SKILL);

	//X버튼
	UIWindow* xButton = new UIWindow;
	xButton->Setup();
	xButton
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/close_icon.png")
		->SetSize(13, 15)
		->SetPosition(262, 30)
		->SetDeligate(new UICloseDeligate);

	//UI이름
	UITextBox* uiName = new UITextBox;
	uiName->Setup();
	uiName
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(300, 20)
		->SetPosition(0, 20);
	uiName->SetText("스    킬");
	uiName->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);

	disc = new UITextBox;
	disc->Setup();
	disc//->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/ui_background_small.png")
		->SetSize(250, 250)
		//->SetPosition(0, 400)
		->SetTagNum(UITag::UI_SKILL_DISC)
		->SetHiddenState(true);

	SetHiddenState(true);

	d->DiscWindow = disc;
	
	UIWindow* sockets = new UIWindow;
	sockets->Setup();
	sockets
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/ui_background_small.png")
		->SetSize(250, 300)
		->SetPosition(25, 75);

	// = new UISocket;
	/*
	socket->Setup();
	socket
		->AttachTo(sockets)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/Icon/socket/Item_Slot.png")
		->SetPosition(0, 0)
		->SetSize(43, 44)
		->SetDeligate(d);

	socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("RollingLeft"));
	*/
	/*
	std::vector<Skill*> skills = SkillManager::GetInstance()->GetSkills();

	for (int i = 0; i < skills.size(); i++)
	{
		socket = new UISocket;

		socket->Setup();
		socket
			->AttachTo(sockets)
			->AttackToWindow(this)
			->SetBackground("../Resources/Textures/Icon/socket/Item_Slot.png")
			->SetPosition(12 + (i % 10) * 60, 20 + (i / 10) * 60)
			->SetSize(43, 44)
			->SetDeligate(d)
			->SetTagNum(SKILL_SOCKET(i));

		socket->SetSocketObject(skills[i]);
	}
	*/
	{//스킬아이콘 위치 잡음
		UISocket* socket;
		Skill* skill;
		char* socIcon = "../Resources/Textures/Icon/socket/Item_Slot.png";
		UINT tag = 0;

		//좌 구르기
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(312 - 240, 20)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("RollingLeft"));

		//우 구르기
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(312 - 180, 20)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("RollingRight"));

		//뒤 구르기
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(312 - 120, 20)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("RollingBack"));

		//일반공격
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(12, 80)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("Attack"));

		//전진공격
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(72, 80)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("WalkAttack"));

		//좌 사이드 공격
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(312 - 180, 140)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("SideAttackLeft"));

		//우 사이드 공격
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(312 - 120, 140)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("SideAttackRight"));

		//방패 공격
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(12, 140)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("SheildAttack"));

		//필살기
		socket = new UISocket;
		socket->Setup();
		socket->AttachTo(sockets)->AttackToWindow(this)->SetBackground(socIcon)->SetSize(43, 44)->SetDeligate(d)
			->SetPosition(312 - 120, 260)->SetTagNum(SKILL_SOCKET(tag++));
		socket->SetSocketObject(SkillManager::GetInstance()->GetSkill("Ultimate"));
	}
}

void UISkill::Release()
{
	UIWindow::Release();
}

void UISkill::Update()
{
	UIWindow::Update();
	//SetHiddenState(false);
	if (KEY_MGR->IsOnceDown('K'))
	{
		Toggle();
		disc->SetHiddenState(true);
	}
	POINT p = GetMousePos();
	POINT tp = GetPosition();
	disc->SetPosition(p.x + 25, p.y + 25);

	disc->Update();
}

void UISkill::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);

	disc->Render(sprite);
}

inline UINT UISkillSocketDeligate::OnMouseOver(UIWindow * caller, UIWindow * root)
{
	UITextBox* t = (UITextBox*)root->GetWindowByTag(UITag::UI_SKILL_DISC);

	Skill* s = (Skill*)((UISocket*)caller)->GetSocketObject();

	if (!s)
	{
		t->SetText("");
		return 0;
	}

	SkillInfo info = s->GetInfo();

	std::string disc = "";

	disc.append(info.Name);
	if (info.Damage > 0)
	{
		disc.append("\n\n데미지  : ");
		disc.append(std::to_string((int)(info.Damage * 100)) + " %");
	}
	disc.append("\n\n");
	disc.append(info.Disc);

	t->SetText(disc);
	t->SetHiddenState(false);

	return UISocketDeligate::OnMouseOver(caller, root);
}

inline UINT UISkillSocketDeligate::OnMouseOut(UIWindow * caller, UIWindow * root)
{
	if (DiscWindow)
	{
		DiscWindow->SetHiddenState(true);
	}
	//UITextBox* t = (UITextBox*)caller->GetTop()->GetWindowByTag(UITag::UI_SKILL_DISC);
	//t->SetHiddenState(true);

	return 0;
}
