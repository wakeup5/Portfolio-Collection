#pragma once
#include "UITextBox.h"

class UIButton : public UITextBox
{
private:
	LPDIRECT3DTEXTURE9 _overT, _normalT, _clickT;
	D3DXIMAGE_INFO _overI, _normalI, _clickI;
public:
	UIButton();
	virtual ~UIButton();

	virtual void Release();
	virtual void Update();
	virtual void Render(LPD3DXSPRITE sprite);

	void SetButtonTexture(char* normal, char* over, char* click);
	bool IsMouseClick(){
		return _isMouseClick;
	}


	void SetClickTexture();
	void SetNormalTexture();
	void RemoveTexture(char* c);
};