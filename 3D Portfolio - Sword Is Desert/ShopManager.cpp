#include "stdafx.h"
#include "ShopManager.h"
#include "UITextBox.h"
#include "UIButton.h"
#include "UIShop.h"
// 각각의 ui 알림에 맞춰서 text와 position 값을 수정하면된다.
ShopManager::ShopManager()
{
	UIShopDeligate* sellDeli = new UIShopDeligate;
	this->Setup();
	this->SetAlphaBlend(255);
	this->SetBackground(
		"../Resources/Textures/ui/button.png");
	this->SetSize(100, 30)
		->SetDeligate(sellDeli)
		->SetTagNum(2000);
	this->SetDrawBoundingState(false);
	this->SetFontFormat(DT_CENTER | DT_WORDBREAK);
	this->SetHiddenState(false);
	this->SetText("");
	this->SetColor(D3DCOLOR_XRGB(255, 187, 0));

	this->SetPosition(50, 475);
	this->SetAlphaBlend(50);
	
	DXFONT_MGR->AddStyle("굴림", "굴림", 22);



	_sellCount = new UITextBox;
	_sellCount->Setup();
	_sellCount->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(40, 20)
		->SetPosition(10, 5);

	_sellCount->SetText("0");
	_sellCount->SetFontFormat(DT_CENTER | DT_WORDBREAK);


	UIButton* numberUpBtn = new UIButton;
	numberUpBtn->Setup();
	numberUpBtn
		->AttachTo(this)
		->AttackToWindow(this)
		//->SetBackground("../Resources/Textures/ui/button.png")
		->SetSize(20, 20)
		->SetDeligate(sellDeli)
		->SetTagNum(UITag::UI_SHOP_COUNT_PLUS)
		->SetPosition(55,5);
	numberUpBtn->SetDrawBoundingState(false);
	numberUpBtn->SetAlphaBlend(255);
	numberUpBtn->SetButtonTexture(
		"../Resources/Textures/Icon/button/updown/up.png",
		"../Resources/Textures/Icon/button/updown/up.png",
		"../Resources/Textures/Icon/button/updown/up_down.png"
		);

	UIButton* numberDownBtn = new UIButton;
	numberDownBtn->Setup();
	numberDownBtn
		->AttachTo(this)
		->AttackToWindow(this)
		//->SetBackground("../Resources/Textures/ui/button.png")
		->SetSize(20, 20)
		->SetDeligate(sellDeli)
		->SetTagNum(UITag::UI_SHOP_COUNT_MINUS)
		->SetPosition(75, 5);
	numberDownBtn->SetDrawBoundingState(false);
	numberDownBtn->SetAlphaBlend(255);
	numberDownBtn->SetButtonTexture(
		"../Resources/Textures/Icon/button/updown/down.png",
		"../Resources/Textures/Icon/button/updown/down.png",
		"../Resources/Textures/Icon/button/updown/down_down.png"
		);

}


ShopManager::~ShopManager()
{
}

void ShopManager::Release()
{
	UIWindow::Release();
}

void ShopManager::Update(float timeDelta)
{
	_startTime += timeDelta;
	if (_alphaBlend > 0){
		//_alphaBlend -= 3;
	}

	if (_alphaBlend < 0){
		_alphaBlend = 0;
		///	this->Release();
	}
	
}

void ShopManager::Render(LPD3DXSPRITE sprite)
{
	if (isSellMode){
		UIWindow::Render(sprite);
		POINT pos = GetFinalPosition();
		RECT rc;
		rc.left = pos.x;
		rc.top = pos.y;
		rc.right = pos.x + _size.width;
		rc.bottom = pos.y + _size.height;

		LPD3DXFONT font = DXFONT_MGR->GetStyle("굴림");

		font->DrawText(NULL, GetText().c_str(), GetText().size(), &rc, GetFontFormat(), D3DCOLOR_RGBA(255, 187, 0, _alphaBlend));
	}
}

