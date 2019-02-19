#include "stdafx.h"
#include "UISocket.h"

UISocket::UISocket()
	: _obj(NULL)
{
}


UISocket::~UISocket()
{
}

void UISocket::Render(LPD3DXSPRITE sprite)
{
	UITextBox::Render(sprite);

	if (_obj)
	{
		FillTextureRender(sprite, _obj->GetIcon(), _obj->GetIconInfo());
	}
}
