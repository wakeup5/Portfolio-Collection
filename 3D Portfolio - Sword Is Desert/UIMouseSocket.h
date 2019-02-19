#pragma once
#include "UIWindow.h"
#include "UISocket.h";

class UIMouseSocket :
	public UIWindow
{
private:
	UISocket* _socket;
public:
	UIMouseSocket();
	virtual ~UIMouseSocket();

	void Setup();
	void Update();
};

