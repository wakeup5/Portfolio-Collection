#pragma once
class Item;
class UISocket;

#define INVENTORY_LENGTH 64
#define INVENTORY_WIDTH 50;
#define INVENTORY_HEIGHT 30;
#define INVENTORY_NEWLINE 60;
class Inventory
{
private:
	Item* _item[INVENTORY_LENGTH];

	UINT _clickIndex;
	bool _isHidden = true;
public:
	Inventory();
	virtual ~Inventory();

	Item* GetItem(UINT index) { return _item[index]; }

	bool Swap(UINT destIndex, UINT sourIndex);

	void SetClickIndex(UINT index) { _clickIndex = index; }
	void SetItem(Item* it, UINT index);
	void NullItem(UINT index);
	Item* GetClickIndex();

};

