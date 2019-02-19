#pragma once
#include <vector>

using namespace std;

class Container;

enum ITEM_TYPE
{
	ITEM_TYPE_ARMOR_HELMET, /**< ��� �� hp,		���� def */
	ITEM_TYPE_ARMOR_TOP,	/**< ���� �� def,	���� hp */
	ITEM_TYPE_ARMOR_PANTS,	/**< ���� �� def,	���� dex */
	ITEM_TYPE_ARMOR_SHOES,	/**< �Ź� �� dex,	���� moveSpeed */
	ITEM_TYPE_ARMOR_GLOVE,	/**< �尩 �� str,	���� atkSpeed */
	ITEM_TYPE_WEAPON,		/**< ���� �� atk,	���� str */
	ITEM_TYPE_POTION_HP,	/**< �ǹ� �� hp ȸ�� */
	ITEM_TYPE_POTION_MP	/**< ���� �� mp ȸ�� */
};

class Item
{
private:
	ITEM_TYPE _itemType;

	int _main;
	int _sub;

	char* _itemName;
	char* _brief;

public:
	Item(ITEM_TYPE type, char* name, int main, int sub, char* brief = "");
	~Item();

	void setType(ITEM_TYPE itemType){ _itemType = itemType; }
	ITEM_TYPE getType(){ return _itemType; }
	void setMain(int main){ _main = main; }
	int getMain(){ return _main; }
	void setSub(int sub){ _sub = sub; }
	int getSub(){ return _sub; }

	char* getName(){ return _itemName; }
	char* getBrief(){ return _brief; }

};

class ItemManager
{
private:
	vector<Item>* _items;
public:
	ItemManager();
	~ItemManager();

	Item randomItem(int = 1);
};