//#include "GameMain.h"
#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Sound.h"
#include "Image.h"
#include "Display.h"

class Story
{
private:
	char* _name;
public:
	Story();
	~Story();

	void name(char*);
	void dialogLine(char* = "", int = 10, int = 0);
	void dialogLinePause(char* = "", int = 10, int = 0);
	void pause();

	void opening();
	void intro();
};


