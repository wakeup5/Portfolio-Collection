#include "Item.h"
#include "Container.h"


Item::Item(ITEM_TYPE type, char* name, int main, int sub, char* brief)
{
	_itemType = type;
	_itemName = name;
	_main = main;
	_sub = sub;
	_brief = brief;
}


Item::~Item()
{
}

ItemManager::ItemManager()
{
	_items = new vector < Item > ;

	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº ±ÇÃÑ", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "±ÇÃÑ", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ1", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ2", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ3", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ4", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ5", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ6", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ7", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ8", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ9", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ10", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ11", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ12", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ13", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ14", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ15", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "³°Àº µüÃÑ16", 12, 10));
}

ItemManager::~ItemManager()
{
	delete _items;
}

Item ItemManager::randomItem(int level)
{
	int randNum = rand() % _items->size();

	return _items->at(randNum);
}