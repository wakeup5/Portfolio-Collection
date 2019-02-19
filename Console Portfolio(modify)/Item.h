#pragma once
#include <vector>
#include <conio.h>
#include "GameDisplay.h"

using namespace std;

class Container;

enum ITEM_TYPE
{
	ITEM_TYPE_ARMOR_HELMET, /**< 헬멧 주 hp,		서브 def */
	ITEM_TYPE_ARMOR_TOP,	/**< 상의 주 def,	서브 hp */
	ITEM_TYPE_ARMOR_PANTS,	/**< 하의 주 def,	서브 dex */
	ITEM_TYPE_ARMOR_SHOES,	/**< 신발 주 dex,	서브 moveSpeed */
	ITEM_TYPE_ARMOR_GLOVE,	/**< 장갑 주 str,	서브 atkSpeed */
	ITEM_TYPE_WEAPON,		/**< 무기 주 atk,	서브 str */
	ITEM_TYPE_POTION_HP,	/**< 피물 주 hp 회복 */
	ITEM_TYPE_POTION_MP	/**< 엠물 주 mp 회복 */
};

class Item
{
private:
	ITEM_TYPE _itemType;

	int _main;
	int _sub;

	int _price;

	char* _itemName;
	char* _brief;

public:
	Item(ITEM_TYPE type, char* name, int main, int sub, int price, char* brief = "");
	//Item(Item&);
	~Item();

	void setType(ITEM_TYPE itemType){ _itemType = itemType; }
	ITEM_TYPE getType(){ return _itemType; }
	void setMain(int main){ _main = main; }
	int getMain(){ return _main; }
	void setSub(int sub){ _sub = sub; }
	int getSub(){ return _sub; }

	int getPrice(){ return _price; }

	char* getName(){ return _itemName; }
	char* getBrief(){ return _brief; }

};

class ItemManager
{
private:
	vector<Item> _items;
public:
	ItemManager();
	~ItemManager();

	Item* randomItem(int = 1);
	vector<Item>* getItems();
	vector<Item>* getItems(ITEM_TYPE);

	static int showItem(vector<Item>);
};