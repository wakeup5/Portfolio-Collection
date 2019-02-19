#pragma once
#include "UITextBox.h"
#include "Singleton.h"
class UIAlram : public UITextBox, public Singleton<UIAlram>
{
private:
	
	float _startTime = 0;
	UIDialog* _ui;

public:
	UIAlram();
	virtual ~UIAlram();
	void Release();
	void Update(float timeDelta);
	void Render(LPD3DXSPRITE sprite);
	void SetAlphaBlend(int alpha){
		_alphaBlend = alpha;
	}
};

