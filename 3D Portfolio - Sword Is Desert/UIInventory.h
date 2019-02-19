#pragma once
#include "UIWindow.h"
#include "UITextBox.h"
#include "UIButton.h"
#include "Inventory.h"
#include "Equipment.h"
#include "UISocket.h"

class UIInventory : public UIWindow
{
private:
	UITextBox* _textBox;
	UIButton* _buttonOk;
	Inventory* _inventory;
	
	UIDeligateDefault* defaulDeligate;
	
	UIWindow* content;

	UIInventorySocketDeligate* _inventorySocket;

	UITextBox* itemInfoBox;
	UITextBox* itemInfoNameBox;
	UITextBox* itemInfoDiscBox;
	UITextBox* itemInfoPrice1Box;
	UITextBox* itemInfoPrice2Box;
	UISocket* itemInfoSocket;
	UISocket* moneySocket;
	UITextBox* moneyQTY;

	Equipment* _equiped;

	bool _isOpenInventory = false;	

	float fTime = 0;
	float nowTime = 0;
public:
	UIInventory();
	virtual ~UIInventory();

	virtual void Setup(Inventory* inven, Equipment* equiped);
	virtual void Release();
	virtual void Update();
	virtual void Render(LPD3DXSPRITE sprite);
	// 아이템정보창 초기화
	void RemoveText();
	// 아이템 정보
	void SetItemInfo(ItemInfo& info);
	// 인벤토리 오픈 여부(인벤토리 창 열릴때 장비창 제어용)
	bool IsInventoryOpen(){
		return _isOpenInventory;
	}
	
	void InventorySellClick();
	void InventorySwap();
	void InventoryEquiped();
	void InventoryItemInfo();

	void AddInventory(std::string itemName);
	Inventory* GetInventory(){
		return _inventory;
	}

	void UseItem(int idx);

	void Toggle();
};

