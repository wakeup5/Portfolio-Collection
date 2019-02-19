#pragma once
#include "UITextBox.h"
#include "Singleton.h"
class UILevel : public UITextBox
{
private:

	float _startTime = 0;
	UIDialog* _ui;

public:
	UILevel();
	virtual ~UILevel();
	void Release();
	void Setup();
	void Update();
	void Render(LPD3DXSPRITE sprite);
	void SetAlphaBlend(int alpha){
		_alphaBlend = alpha;
	}
};

