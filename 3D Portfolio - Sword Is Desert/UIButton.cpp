#include "stdafx.h"
#include "UIButton.h"


UIButton::UIButton()
	: _clickT(NULL)
	, _overT(NULL)
	, _normalT(NULL)
{
}


UIButton::~UIButton()
{
}

void UIButton::Release()
{
	UIWindow::Release();

	if (_normalT) _normalT->Release();
	if (_overT) _overT->Release();
	if (_clickT) _clickT->Release();
}

void UIButton::Update()
{
	UITextBox::Update();

	if (_isMouseClick)
	{
		if (_clickT) SetBackground(_clickT, _clickI);
	}
	else if (_isMouseOver)
	{
		if (_overT) SetBackground(_overT, _overI);
	}
	else
	{
		if (_normalT) SetBackground(_normalT, _normalI);
	}
}
void UIButton::Render(LPD3DXSPRITE sprite)
{
	UITextBox::Render(sprite);
}

void UIButton::SetButtonTexture(char* normal, char* over, char* click)
{
	if (_normalT) _normalT->Release();
	if (_overT) _overT->Release();
	if (_clickT) _clickT->Release();

	_normalT = TEXTURE_MGR->GetTexture(normal, &_normalI);
	_overT = TEXTURE_MGR->GetTexture(over, &_overI);
	_clickT = TEXTURE_MGR->GetTexture(click, &_clickI);

	_normalT->AddRef();
	_overT->AddRef();
	_clickT->AddRef();
}

void UIButton::SetClickTexture(){
	SetBackground(_clickT, _clickI);
}

void UIButton::SetNormalTexture(){
	SetBackground(_normalT, _normalI);
}

void UIButton::RemoveTexture(char* c){
	SetBackground(c);
}


