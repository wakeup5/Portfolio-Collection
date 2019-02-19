#pragma once
#include "Scene.h"


class GameMain
{
private:
	Scene* _scene;

public:
	GameMain();
	~GameMain();

	HRESULT Setup();
	void Release();
	void Update();
	void Render();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

