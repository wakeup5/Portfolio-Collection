#pragma once
#include "UIWindow.h"
#include "UITextBox.h"
#include "UIButton.h"
#include "Inventory.h"
#include "UISocket.h"

class Equipment;

class UIEquippedItem : public UIWindow
{
private:
	UITextBox* _textBox;
	UIButton* _buttonOk;
	Inventory* _inventory;
	Equipment* _equipment;

	UIDeligateDefault* defaulDeligate;
	UIInventorySocketDeligate* sd;
	UIWindow* content;

	UISocket* _headSocket;
	UISocket* _bodySocket;
	UISocket* _handSocket;
	UISocket* _shoesSocket;
	UISocket* _ringSocket;
	UISocket* _necklaceSocket;
	UISocket* _weaphonSocket;
	UISocket* _shieldSocket;
	UISocket* _waistSocket;
	UISocket* _earingSocket;

	UITextBox* _statusAtkBox;
	UITextBox* _statusDefBox;

	int _atk;
	int _def;

	bool _isOpenInventory = false;
	bool _isFirst = true;
public:
	UIEquippedItem(){}
	virtual ~UIEquippedItem(){}

	virtual void Setup(Equipment* equipmentItem);
	virtual void Release();
	virtual void Update();
	virtual void Render(LPD3DXSPRITE sprite);
	// 아이템정보창 초기화
	void RemoveText();
	// 아이템 정보
	void SetItemInfo(ItemInfo& info);
	void SetEquipment(Equipment* equipment){
		_equipment = equipment;
	}
	void ItemUpdate();

	void SetInventoryOpen(bool open)
	{
		_isOpenInventory = open;
	}

	bool IsInventoryOpen(){ return _isOpenInventory; }

};
