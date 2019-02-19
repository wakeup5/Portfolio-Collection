#include "stdafx.h"
#include "UIProgress.h"


UIProgress::UIProgress()
	: _max(1)
	, _current(1)
	, _barT(NULL)
	, _backT(NULL)
{

}


UIProgress::~UIProgress()
{
	SAFE_RELEASE(_barT);
	SAFE_RELEASE(_backT);
}

void UIProgress::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);

	if (_isHidden)
		return;

	//비율 계산
	float per = _current / _max;
	if (per > 1.0f) per = 1.0f;

	D3DXMATRIXA16 matPos, matSize, matTotal;

	//이동
	POINT pos = GetFinalPosition();

	D3DXMatrixTranslation(&matPos, pos.x, pos.y, 0.0f);

	D3DXMatrixIdentity(&matSize);
	D3DXMatrixScaling(&matSize, ((float)_size.width / (float)_barI.Width), (float)_size.height / (float)_barI.Height, 0);

	//합
	D3DXMatrixIdentity(&matTotal);
	matTotal = matSize * matPos;

	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	sprite->SetTransform(&matTotal);

	RECT rc;
	SetRect(&rc, 0, 0, _barI.Width * per, _barI.Height);
	sprite->Draw(_barT,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//Sprite->Draw(buttonTexture, NULL, NULL, &D3DXVECTOR3(-width2, -height2, 0.0), whitecol);

	sprite->End();
}

void UIProgress::SetBarTexture(char * bar, char * back)
{
	SAFE_RELEASE(_barT);
	SAFE_RELEASE(_backT);

	_barT = TEXTURE_MGR->GetTexture(bar, &_barI);
	_backT = TEXTURE_MGR->GetTexture(back, &_backI);

	SAFE_ADDREF(_barT);
	SAFE_ADDREF(_backT);

	SetBackground(_backT, _backI);
}
