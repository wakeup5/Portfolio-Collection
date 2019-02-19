#include "stdafx.h"
#include "UIShopButton.h"
#include "UITextBox.h"
#include "UIButton.h"
#include "UIShop.h"
// 각각의 ui 알림에 맞춰서 text와 position 값을 수정하면된다.




UIShopButton::UIShopButton()
{
	

}


UIShopButton::~UIShopButton()
{
}


void UIShopButton::init(){
	UIShopDeligate* sellDeli = new UIShopDeligate;
	this->Setup();
	//this->SetAlphaBlend(255);
	this->SetBackground(
		"../Resources/Textures/ui/button.png");
	this->SetSize(80, 40)
		->SetDeligate(sellDeli)
		->SetTagNum(2000);
	this->SetDrawBoundingState(false);
	this->SetFontFormat(DT_TOP | DT_CENTER | DT_WORDBREAK);
	this->SetHiddenState(false);
	this->SetText("");
	this->SetColor(D3DCOLOR_XRGB(255, 187, 0));

	this->SetPosition(10, 30);
	this->SetAlphaBlend(150);

	UIButton* okBtn = new UIButton;
	okBtn->Setup();
	okBtn
		->AttachTo(this)
		->AttackToWindow(this)
		//->SetBackground("../Resources/Textures/ui/button.png")
		->SetSize(40, 40)
		->SetDeligate(sellDeli)
		->SetTagNum(UITag::UI_SHOP_BTN_OK)
		->SetPosition(0, 0);
	okBtn->SetDrawBoundingState(false);
	okBtn->SetAlphaBlend(255);
	okBtn->SetButtonTexture(
		"../Resources/Textures/Icon/button/shop/ok.png",
		"../Resources/Textures/Icon/button/shop/ok.png",
		"../Resources/Textures/Icon/button/shop/ok_down.png"
		);

	UIButton* cancleBtn = new UIButton;
	cancleBtn->Setup();
	cancleBtn
		->AttachTo(this)
		->AttackToWindow(this)
		//->SetBackground("../Resources/Textures/ui/button.png")
		->SetSize(40, 40)
		->SetDeligate(sellDeli)
		->SetTagNum(UITag::UI_SHOP_BTN_CANCLE)
		->SetPosition(40, 0);
	cancleBtn->SetDrawBoundingState(false);
	cancleBtn->SetAlphaBlend(255);
	cancleBtn->SetButtonTexture(
		"../Resources/Textures/Icon/button/shop/cancle.png",
		"../Resources/Textures/Icon/button/shop/cancle.png",
		"../Resources/Textures/Icon/button/shop/cancle_down.png"
		);


	ZeroMemory(screenItemArray, sizeof(UINT*) * 9);
}


void UIShopButton::Release()
{
	UIWindow::Release();
}

void UIShopButton::Update(float timeDelta)
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

void UIShopButton::Render(LPD3DXSPRITE sprite)
{
	if (_isShowBtn){
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

