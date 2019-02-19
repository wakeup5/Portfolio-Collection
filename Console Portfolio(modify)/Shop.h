#pragma once
#include "GameDisplay.h"
#include "GameImage.h"
#include "GameSound.h"
#include "Unit.h"
#include "Story.h"

class Shop
{
private:
	Player* _player;
	ItemManager* _itemManager;
public:
	Shop(Player*, ItemManager*);
	~Shop();

	void openShop();
	void buyShop();
	void showItem(ITEM_TYPE);
	void sellShop();
};

