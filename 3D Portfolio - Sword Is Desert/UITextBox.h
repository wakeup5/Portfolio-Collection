#pragma once
#include "UIWindow.h"

class UITextBox :
	public UIWindow
{
private:
	//const char* _text;
	std::string _text;
	DWORD _fontFormat;
	D3DCOLOR _color;
public:
	UITextBox();
	virtual ~UITextBox();

	virtual void Setup();
	//virtual void Release();
	virtual void Update();
	virtual void Render(LPD3DXSPRITE sprite);

	void SetText(const char* t) { _text = t; }
	void SetText(std::string &t) { _text = t; }
	void SetFontFormat(DWORD format) { _fontFormat = format; }
	void SetColor(D3DCOLOR color) { _color = color; }


	std::string GetText(){ return _text; }
	DWORD GetFontFormat(){ return _fontFormat; }
	
};