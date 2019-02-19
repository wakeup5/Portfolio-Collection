#pragma once
#include "GameDisplay.h"
#include "GameImage.h"
#include "GameSound.h"
#include "Story.h"
#include "Item.h"
#include "Town.h"
#include "Unit.h"

class GameMain
{
private:
	Story* _story;
	ItemManager* _itemManager;
	UnitManager* _unitManager;
	Town* _town;

	void opening();
	void town();

public:
	GameMain();
	~GameMain();

	void startGame();
};

