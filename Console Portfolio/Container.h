#pragma once
#include "Story.h"
#include "Unit.h"
#include "Item.h"
#include "Dungeon.h"

class UnitManager;
class Dungeon;
class Container
{
private:
	static Container* _instance;

	Dungeon* _dungeon;
	Story* _story;

	UnitManager* _unitManager;
	ItemManager* _itemManager;

	Container();

public:
	static Container* getInstance();

	Dungeon* getDungeon();
	Story* getStory();

	UnitManager* getUnitManager();
	ItemManager* getItemManager();

	~Container();
};

