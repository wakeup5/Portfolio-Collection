#include "stdafx.h"
#include "UILevel.h"
#include "UITextBox.h"

int playerLevel = 1;
// 각각의 ui 알림에 맞춰서 text와 position 값을 수정하면된다.
UILevel::UILevel()
{

}


UILevel::~UILevel()
{
}


void UILevel::Setup(){
	UIWindow::Setup();
	
	this->SetAlphaBlend(255);
	this->SetBackground("../Resources/Textures/Icon/Level.png");
	this->SetSize(250, 100)
		->SetDeligate(NULL)
		->SetTagNum(2000);
	this->SetDrawBoundingState(false);
	int player_level = PLAYER_MGR->GetPlayer()->GetCharacterInfo().Level;
	//퍼센테이지로 적용. 경험치 공식 (적 레벨 + 본인 레벨) / (본인 레벨 * 5)
	float player_exp = PLAYER_MGR->GetPlayer()->GetCharacterInfo().Exp;
	std::string levelInfo;
	levelInfo.append("Level " + std::to_string(player_level * 100) +"\n ");
	levelInfo.append(std::to_string(player_exp));
	levelInfo.append("%");
	this->SetText(levelInfo);
	this->SetFontFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	//this->SetPosition(WINSIZE_X / 2 - 220, -400);
	this->SetColor(D3DCOLOR_XRGB(255, 255, 255));

	this->SetPosition(0 , 0);
	DXFONT_MGR->AddStyle("NanumPen", "NanumPen", 32);
	//LPD3DXFONT font = DXFONT_MGR->GetStyle("");

	SetHiddenState(false);
	_isHidden = false;
}


void UILevel::Release()
{
	UIWindow::Release();
}

void UILevel::Update()
{
	UIWindow::Update();
	//_isHidden = false;
	/*
	if (KEY_MGR->IsOnceDown(VK_F9)){
		PLAYER_MGR->GetPlayer()->GetCharacterInfo().Exp += 10;
		if (PLAYER_MGR->GetPlayer()->GetCharacterInfo().Exp == PLAYER_MGR->GetPlayer()->GetCharacterInfo().Level * 100){
			PLAYER_MGR->GetPlayer()->GetCharacterInfo().Level += 1;
			PLAYER_MGR->GetPlayer()->GetCharacterInfo().Exp = 0;
		}
		std::string levelInfo;
		levelInfo.append("\n Level " + std::to_string(PLAYER_MGR->GetPlayer()->GetCharacterInfo().Level) + "\n ");
		levelInfo.append(std::to_string(PLAYER_MGR->GetPlayer()->GetCharacterInfo().Exp));
		levelInfo.append("%");
		this->SetText(levelInfo);
	}
	*/
	if (playerLevel != PLAYER_MGR->GetPlayer()->GetCharacterInfo().Level){
		playerLevel = PLAYER_MGR->GetPlayer()->GetCharacterInfo().Level;
		SOUND_MGR->isPlay("levelUp");
	}
	std::string levelInfo;
	levelInfo.append("Level " + std::to_string(PLAYER_MGR->GetPlayer()->GetCharacterInfo().Level) + "\n ");
	levelInfo.append(std::to_string(PLAYER_MGR->GetPlayer()->GetCharacterInfo().Exp * 100));
	levelInfo.append("%");
	this->SetText(levelInfo);
}

void UILevel::Render(LPD3DXSPRITE sprite)
{
	if (_isHidden) return;

	UIWindow::Render(sprite);

	POINT pos = GetFinalPosition();
	RECT rc;
	rc.left = pos.x;
	rc.top = pos.y;
	rc.right = pos.x + _size.width;
	rc.bottom = pos.y + _size.height;

	LPD3DXFONT font = DXFONT_MGR->GetStyle("NanumPen");

	font->DrawText(NULL, GetText().c_str(), GetText().size(), &rc, GetFontFormat(), D3DCOLOR_RGBA(254, 247, 221, _alphaBlend));
}

