#pragma once
#include "UITextBox.h"
#include "Singleton.h"
class UIShop;
class ShopManager : public UITextBox, public Singleton<ShopManager>
{
private:

	float _startTime = 0;
	UIDialog* _ui;
	UITextBox* _sellCount;
	int _sellNumber = 0;
public:
	ShopManager();
	virtual ~ShopManager();
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
};

