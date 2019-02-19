#include "stdafx.h"
#include "UIAlram.h"
#include "UITextBox.h"

// 각각의 ui 알림에 맞춰서 text와 position 값을 수정하면된다.
UIAlram::UIAlram()
{
	
	this->Setup();
	this->SetAlphaBlend(0);
	this->SetBackground(
		"../Resources/Textures/Icon/퀘스트_알림.png");
	this->SetSize(450, 70)
		->SetDeligate(NULL)
		->SetTagNum(2000);
	this->SetDrawBoundingState(false);
	this->SetText("\n퀘스트를 수락하였습니다.");
	this->SetFontFormat(DT_CENTER | DT_WORDBREAK);
	this->SetHiddenState(true);
	this->SetPosition(WINSIZE_X / 2 - 220, -400);
	this->SetColor(D3DCOLOR_XRGB(255, 187, 0));

	this->SetPosition(WINSIZE_X / 2 - 220, -400);
	DXFONT_MGR->AddStyle("NanumPen1", "NanumPen1", 24);
	//LPD3DXFONT font = DXFONT_MGR->GetStyle("");

	
}


UIAlram::~UIAlram()
{
}

void UIAlram::Release()
{
	UIWindow::Release();
}

void UIAlram::Update(float timeDelta)
{
	_startTime += timeDelta;
	if (_alphaBlend > 0){
		_alphaBlend -= 3;
	}

	if (_alphaBlend < 0){
		_alphaBlend = 0;
	///	this->Release();
	}
}

void UIAlram::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);

	POINT pos = GetFinalPosition();
	RECT rc;
	rc.left = pos.x;
	rc.top = pos.y;
	rc.right = pos.x + _size.width;
	rc.bottom = pos.y + _size.height;

	LPD3DXFONT font = DXFONT_MGR->GetStyle("NanumPen1");

	font->DrawText(NULL, GetText().c_str(), GetText().size(), &rc, GetFontFormat(), D3DCOLOR_RGBA(255, 187, 0, _alphaBlend));

}

