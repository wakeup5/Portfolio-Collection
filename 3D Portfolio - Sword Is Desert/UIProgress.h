#pragma once
#include "UIWindow.h"
class UIProgress :
	public UIWindow
{
private:
	LPDIRECT3DTEXTURE9 _barT, _backT;
	D3DXIMAGE_INFO _barI, _backI;

	float _max;
	float _current;
public:
	UIProgress();
	virtual ~UIProgress();

	virtual void Render(LPD3DXSPRITE sprite);

	void SetBarTexture(char* bar, char* back);
	void SetMax(float max) { _max = max; }
	void SetCurrent(float current) { _current = current; }
};

