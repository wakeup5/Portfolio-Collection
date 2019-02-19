#pragma once
#include "Item.h"

class Equipment
{
private:
	Item* _item[EQUIPTYPE_LENGTH];
public:
	Equipment();
	virtual ~Equipment();

	Item* GetItem(ITEM_TYPE equipType);

	Item* Equip(Item* item);
	bool UnEquip(ITEM_TYPE type, OUT Item** out);
	bool Swap(IN Item* in, OUT Item** out);

	ItemInfo GetTotalStatus();

	void Render();
	void MainWeaponRender();
	void SubWeaponRender();
};

