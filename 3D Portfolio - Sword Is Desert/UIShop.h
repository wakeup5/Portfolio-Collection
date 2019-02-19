#pragma once
#include "UIWindow.h"
#include "UISocket.h"
#include "ShopManager.h"
#include "UIShopButton.h"
#include "UIInventory.h"
class UIShop : public UIWindow
{
private:

	int _clickQuestIndex = -1;
	bool _isOpenQuest = false;
	bool _isCursor = false;
	bool _isBuyMode = true;
	bool _isShopFirstClick = false;
	UITextBox* itemInfoBox;
	UITextBox* itemInfoNameBox;
	UITextBox* itemInfoDiscBox;
	UITextBox* itemInfoPrice1Box;
	UITextBox* itemInfoPrice2Box;
	UISocket* itemInfoSocket;
	UIShopSocketDeligate* _shopSocektDeligate;
	UIInventory* _inven;

public:
	int _testInt = 0;
	UIShop();
	virtual ~UIShop();

	void Setup();
	void Release();
	void Update();
	void Render(LPD3DXSPRITE sprite);
	
	void Buy_Item();
	void Sell_Item();
	void Buy_Tab(); // 구입탭
	void Sell_Tab(); // 팔기 탭으로 전환

	void SetItemInfo(tagItemInfo& info);
	void SetCursor(bool c){ _isCursor = c; }
	
	void InventoryItemInfo();
	// 아이템정보창 초기화
	void RemoveText();

	void OpenButton();

	void SetInventory(UIInventory* inven){
		_inven = inven;
	}

	UIInventory* GetInventory(){
		return _inven;
	}

	void AddInventory(std::string itemName){
		_inven->AddInventory(itemName);
	}

	void SellInventory(){
		_inven->InventorySellClick();
	}

	void InitShopInventory();

};




class UIShopDeligate : public UIDeligate
{
public:
	inline virtual UINT OnClickDown(UIWindow* caller, UIWindow* root) override
	{
		UINT t = caller->GetTagNum();
		int sdf = 0;
		HCURSOR hCursor;
		
	//	wcex.hCursor = customCursor;
		//SetSystemCursor(customCursor,	32512);
	//	RegisterClassEx(&wcex);
		switch (caller->GetTagNum())
		{
		case UITag::UI_SHOP_BUY:
			//((UIDialog*)(caller->GetTop()))->SetNPC(NULL);
			((UIShop*)(caller->GetTop()))->SetCursor(true);
			((UIShop*)(caller->GetTop()))->Buy_Tab();

	//		HCURSOR hCursor;
		//	customCursor = LoadCursorFromFile("use.cur");
		//	SetCursor(customCursor);
			//hCursor = LoadCursorFromFile("stand_mouse.cur");
			//HCURSOR hCursor;
			break;

		case UITag::UI_SHOP_SELL:
			((UIShop*)(caller->GetTop()))->Sell_Tab();
			break;


		case UITag::UI_SHOP_COUNT_PLUS:
			((ShopManager*)(caller->GetTop()))->SetCount(+1);
			break;

		case UITag::UI_SHOP_COUNT_MINUS:
			((ShopManager*)(caller->GetTop()))->SetCount(-1);
			break;

		case UITag::UI_SHOP_BTN_OK:
			((UIShopButton*)(caller->GetTop()))->_isShowBtn = false;
			if (!isSellMode){
				((UIShopButton*)(caller->GetTop()))->_isAddInventory = true;
			}
			else{
				((UIShopButton*)(caller->GetTop()))->_isSellInventory = true;
			}
		//	((UIShop*)(caller->GetTop()))->AddInventory();
			//((UIInventory*)(caller->GetTop()))->AddInventory("dd");
			break;

		case UITag::UI_SHOP_BTN_CANCLE:
			((UIShopButton*)(caller->GetTop()))->_isShowBtn = false;
			break;

		default:
			break;
		}

		return RETURN_DEFAULT;
	}

	inline virtual UINT OnMouseOver(UIWindow* caller, UIWindow* root) override
	{


		return RETURN_DEFAULT;
	}
};