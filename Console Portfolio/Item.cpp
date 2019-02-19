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

	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "����", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����1", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����2", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����3", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����4", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����5", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����6", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����7", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����8", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����9", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����10", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����11", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����12", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����13", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����14", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����15", 12, 10));
	_items->push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����16", 12, 10));
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