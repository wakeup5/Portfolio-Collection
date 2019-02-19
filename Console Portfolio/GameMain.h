#pragma once
#include <iostream>
//#include "Story.h"
#include "Display.h"
//#include "Dungeon.h"
#include "Map.h"

using namespace std;

class GameMain
{
private:
	//Sound* _sound;
	//Story* _story;
	//Display* _display;

public:
	GameMain();
	~GameMain();
	
	void startGame();

	void opening();
	void map();
	//Sound* getSound();
	//Story* getStory();

};

