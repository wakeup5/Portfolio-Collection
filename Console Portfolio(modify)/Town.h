#pragma once
#include <string>
#include "GameDisplay.h"
#include "GameImage.h"
#include "GameSound.h"
#include "Item.h"
#include "Unit.h"
#include "Shop.h"
#include "HighLowSeven.h"
#include "Wolnamppong.h"
#include "Sutda.h"
#include "Map.h"
#include "Dungeon.h"

using namespace std;

class Town
{
private:
	ItemManager* _itemManager;
	UnitManager* _unitManager;
	DungeonManager* _dungeonManager;

	Shop* _shop;
	//Gamble* _gamble;

public:
	Town(UnitManager*, ItemManager*);
	~Town();

	int inTown();
	void playerInfo();
};