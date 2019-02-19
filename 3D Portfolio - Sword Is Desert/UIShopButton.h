#pragma once
#include "UITextBox.h"
#include "Singleton.h"
class UIShop;
class UIShopButton : public UITextBox, public Singleton<UIShopButton>
{
private:

	float _startTime = 0;
	UIDialog* _ui;
	UITextBox* _sellCount;
	int _sellNumber = 0;

public:
	bool _isShowBtn = false;
	bool _isAddInventory = false;
	bool _isSellInventory = false;
	UINT screenTagNum = 0;
	UINT screenItemArray[9];

	UIShopButton();
	virtual ~UIShopButton();
	void init();
	void Release();
	void Update(float timeDelta);
	void Render(LPD3DXSPRITE sprite);
	void SetAlphaBlend(int alpha){
		_alphaBlend = alpha;
	}
	void SetCount(int num){
		_sellNumber += num;
		if (_sellNumber < 0)_sellNumber = 0;
		_sellCount->SetText(std::to_string(_sellNumber));
	}

	void SetButtonPosition(POINT pt){
		this->SetPosition(  pt.x , pt.y);
	}
};

