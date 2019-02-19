#include "stdafx.h"
#include "UIMouseSocket.h"


UIMouseSocket::UIMouseSocket()
{
}


UIMouseSocket::~UIMouseSocket()
{
}

void UIMouseSocket::Setup()
{
	_socket = new UISocket;

	_socket->Setup();
	_socket->SetSize(44, 44);
	_socket->SetTagNum(UITag::UI_MOUSE_SOCKET);
	_socket->AttachTo(this);
	_socket->AttackToWindow(this);
}

void UIMouseSocket::Update()
{
	POINT mousePt = GetMousePos();
	_socket->SetPosition(mousePt.x + 5, mousePt.y + 5);

	if (KEY_MGR->IsOnceDown(VK_LBUTTON) && !GetRoot()->IsMouseOver())
	{
		if(_socket->GetSocketObject() != NULL) _socket->SetSocketObject(NULL);
	}
}

