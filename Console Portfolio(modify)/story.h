//#include "GameMain.h"
#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include "GameSound.h"
#include "GameImage.h"
#include "GameDisplay.h"

using namespace std;

enum GAME_EVENT
{
	GAME_EVENT_SHOP_SELL,
	GAME_EVENT_DUNGEON_1_1
};

class Story
{
private:
	string _name;

public:
	//static StoryItem* _storyItem;

	Story();
	~Story();

	void name(string = "");
	void dialogLine(string = "", int = 10, int = 0);
	void dialogLinePause(string = "", int = 10, int = 0);
	void pause();

	void opening();
	void intro();
	void hospital();

	void getEvent(GAME_EVENT);

	void shopSellEvent();
	void dungeon1Event();

	//StoryItem* getStoryItem(){ return _storyItem; }

};

class StoryItem
{
private:
	StoryItem(){}
	~StoryItem(){}
public:
	static bool isShopSellEvent;
};
