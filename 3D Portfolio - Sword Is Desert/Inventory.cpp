#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "UISocket.h"

Inventory::Inventory()
{
	ZeroMemory(_item, sizeof(Item*) * INVENTORY_LENGTH);
}


Inventory::~Inventory()
{

}


bool Inventory::Swap(UINT destIndex, UINT sourIndex)
{
	Item* temp = _item[sourIndex];
	if (temp == NULL) {
		_item[sourIndex] = _item[destIndex];
		_item[destIndex] = NULL;
		UISocket* s;

		s = (UISocket*)UIWindow::GetRoot()->GetWindowByTag(INVENTORY_SOCKET(destIndex));

		s->removeS();
	}
	else {
		_item[sourIndex] = _item[destIndex];
		_item[destIndex] = temp;
	}
	return true;
}

void Inventory::SetItem(Item* it, UINT index) {

	_item[index] = it;

}

void Inventory::NullItem(UINT index)
{
	//_item[index]->RemoveIcon();
	_item[index] = NULL;
	int sdfs = 0;
}

Item * Inventory::GetClickIndex() { if (_clickIndex == -1) return NULL; return _item[_clickIndex]; }