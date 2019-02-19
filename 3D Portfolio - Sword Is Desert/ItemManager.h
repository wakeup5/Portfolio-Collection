#pragma once
#include "Singleton.h"
#include "Item.h"

class ItemManager : public Singleton < ItemManager >
{
public:

	
	static std::map<std::string, ItemInfo> MapItem;

private:
	std::vector<Item*> _items;

public:
	ItemManager();
	virtual ~ItemManager();

	Item* CreateItem(ItemInfo &info, Transform* trans = NULL);
	Item* CreateItem(std::string name, Transform* trans = NULL);

	void LoadItemInfo();

};

#define ITEM_MGR ItemManager::GetInstance()