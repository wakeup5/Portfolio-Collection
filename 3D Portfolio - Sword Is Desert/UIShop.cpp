#include "stdafx.h"
#include "UIShop.h"
#include "UIButton.h"
#include "ShopManager.h"
#include "ItemManager.h"

UIWindow* _shopContent;
UIShopDeligate*  _shopDeligate;
tagItemInfo sellItemInfoArray[56];

UISocket* socketArray[56];
UISocket* sell_Item_Socket[56];

bool isSellMode;

UIButton* sellBtn;
UIButton* buyBtn;
UIShop::UIShop()
{
}


UIShop::~UIShop()
{
}

void UIShop::Setup()
{
	UIWindow::Setup();

	PLAYER_MGR->GetPlayer()->GetTotalStatus().Price = 100000;

	isSellMode = false;

	SetPosition((WINSIZE_X/2)-200, 100);
	SetSize(400, 600);

	_shopDeligate = new UIShopDeligate;

	SetBackground("../Resources/Textures/ui/ui_background.png");
	SetDeligate(_shopDeligate);
	SetHiddenState(true);
	SetTagNum(UITag::UI_SHOP_OPEN);

	UITextBox* name = new UITextBox;
	name->Setup();
	name->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 20)
		->SetPosition(0, 30);

	name->SetText("상    점");
	name->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);	
	
	sellBtn = new UIButton;

	sellBtn->Setup();
	sellBtn->SetButtonTexture(
		"../Resources/Textures/Icon/button/shop/sell.png",
		"../Resources/Textures/Icon/button/shop/sell.png",
		"../Resources/Textures/Icon/button/shop/sell_click.png");
	sellBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(151, 29)
		->SetPosition(40, 85)
		->SetDeligate(_shopDeligate)
		->SetTagNum(UITag::UI_SHOP_SELL);

	buyBtn = new UIButton;

	buyBtn->Setup();
	buyBtn->SetButtonTexture(
		"../Resources/Textures/Icon/button/shop/buy.png",
		"../Resources/Textures/Icon/button/shop/buy.png",
		"../Resources/Textures/Icon/button/shop/buy_click.png");
	buyBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(151, 29)
		->SetPosition(210, 85)
		->SetDeligate(_shopDeligate)
		->SetTagNum(UITag::UI_SHOP_BUY);

	//X버튼
	UIWindow* xButton = new UIWindow;
	xButton->Setup();
	xButton
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/close_icon.png")
		->SetSize(13, 15)
		->SetPosition(350, 40)
		->SetDeligate(new UICloseDeligate);


	_shopContent  = new UIWindow;
	_shopContent->Setup();
	_shopContent
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/ui_background_small.png")
		->SetSize(350, 350)
		->SetDeligate(_shopDeligate)
		->SetPosition(25, 120);

	/*
	ShopManager* alram = ShopManager::GetInstance();
	alram->Setup();
	alram->AttachTo(this);
	alram->AttackToWindow(this);
	alram->SetAlphaBlend(255);
	*/
	


	itemInfoBox = new UITextBox;

	itemInfoBox->Setup();
	itemInfoBox->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 100)
		->SetPosition(100, 450);
	itemInfoBox->SetFontFormat(DT_TOP | DT_WORDBREAK);

	itemInfoNameBox = new UITextBox;

	itemInfoNameBox->Setup();
	itemInfoNameBox->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(200, 100)
		->SetPosition(100, 450);
	itemInfoBox->SetFontFormat(DT_TOP | DT_WORDBREAK);

	itemInfoDiscBox = new UITextBox;
	itemInfoDiscBox->Setup();
	itemInfoDiscBox->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 100)
		->SetPosition(100, 500);
	itemInfoDiscBox->SetFontFormat(DT_TOP | DT_WORDBREAK);

	itemInfoPrice1Box = new UITextBox;
	itemInfoPrice1Box->Setup();
	itemInfoPrice1Box->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 100)
		->SetPosition(180, 530);
	itemInfoPrice1Box->SetFontFormat(DT_TOP | DT_WORDBREAK);

	itemInfoPrice2Box = new UITextBox;
	itemInfoPrice2Box->Setup();
	itemInfoPrice2Box->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 100)
		->SetPosition(270, 530);
	itemInfoPrice2Box->SetFontFormat(DT_TOP | DT_WORDBREAK);

	itemInfoSocket = new UISocket;
	itemInfoSocket->Setup();
	itemInfoSocket
		->AttachTo(_shopContent)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(20, 330)
		->SetBackground("../Resources/Textures/icon/socket/item_slot.png")
		->SetDeligate(_shopSocektDeligate)
		->SetTagNum(INVENTORY_SOCKET(555));
	itemInfoSocket->SetHiddenState(true);
	itemInfoSocket->SetPosition(1500, 330);

	// 아이템 박스 컬러값 설정
	itemInfoBox->SetColor(D3DCOLOR_XRGB(127, 127, 127));
	itemInfoDiscBox->SetColor(D3DCOLOR_XRGB(255, 221, 115));
	itemInfoPrice1Box->SetColor(D3DCOLOR_XRGB(127, 127, 127));
	itemInfoPrice2Box->SetColor(D3DCOLOR_XRGB(211, 198, 115));

	itemInfoBox->SetDrawBoundingState(false);
	itemInfoNameBox->SetDrawBoundingState(false);
	itemInfoDiscBox->SetDrawBoundingState(false);
	itemInfoPrice1Box->SetDrawBoundingState(false);
	itemInfoPrice2Box->SetDrawBoundingState(false);
	itemInfoSocket->SetDrawBoundingState(false);


	_shopSocektDeligate = new UIShopSocketDeligate;
	_shopSocektDeligate->init();
	
	//OpenButton();
	

}

void UIShop::Release()
{
	UIWindow::Release();
}

void UIShop::Update()
{
	UIWindow::Update();
	

	if (KEY_MGR->IsOnceDown('O'))
	{
		//questInfoBox->SetText("");
		
		Toggle();
		RemoveText();
	}


	if (_isBuyMode){
		InventoryItemInfo(); 
	}
	
	
	if (UIShopButton::GetInstance()->_isAddInventory && _shopSocektDeligate->GetMouseOverIndex() - 1730 >= 0 && _shopSocektDeligate->GetUseItemIndex()!=10000){
		
		UINT indexCount = _shopSocektDeligate->GetMouseOverIndex() - 1730;
		_inven->GetInventory()->GetItem(indexCount);
		int mapIndex = 0;
		std::string keyName;
		keyName.clear();
		int te = _shopSocektDeligate->GetUseItemIndex() + 350 - 1730;
		for (auto i = ItemManager::GetInstance()->MapItem.begin(); i != ItemManager::GetInstance()->MapItem.end(); ++i){
			if (mapIndex == te){
				keyName = i->second.Name;
				break;
			}
			else{
				keyName = "";
			}
			mapIndex++;
		}
		
		UINT playerPirce = PLAYER_MGR->GetPlayer()->GetTotalStatus().Price;
		UINT itemPrice = ItemManager::GetInstance()->MapItem.find(keyName)->second.Price;
		if (playerPirce - itemPrice >= 0){
			PLAYER_MGR->GetPlayer()->GetTotalStatus().Price -= itemPrice;
			AddInventory(keyName);
			_shopSocektDeligate->init();
		}
		else{
			//UIShopButton::GetInstance()->_isAddInventory = false;
			LOG_MGR->AddLog("소지금 부족");
		}
		UIShopButton::GetInstance()->_isAddInventory = false;
		
	}


	if (UIShopButton::GetInstance()->_isSellInventory && _shopSocektDeligate->GetMouseOverIndex() - 1730 >= 0){
		LOG_MGR->AddLog("셀버튼");
		SellInventory();
		UIShopButton::GetInstance()->_isSellInventory = false;

	}
	
}

void UIShop::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);
	//ShopManager::GetInstance()->Render(sprite);
//	UIShopButton::GetInstance()->Render(sprite);
}


void UIShop::OpenButton(){
	
	UIShopButton* alram2 = UIShopButton::GetInstance();
	alram2->AttachTo(this);
	alram2->AttackToWindow(this);
	alram2->init();
	
//	alram2->SetAlphaBlend(255);
//	alram2->SetPosition(_shopSocektDeligate->GetMousePt().x, _shopSocektDeligate->GetMousePt().y);
	
}


void UIShop::Buy_Tab(){
	InitShopInventory();
	RemoveText();
	_isBuyMode = true;
	isSellMode = false;
	for (int i = 0; i < 56; i++){
		socketArray[i]->SetHiddenState(false);
	}

	for (int i = 0; i < 56; i++){
		sell_Item_Socket[i]->SetHiddenState(false);
	}

}


void UIShop::Sell_Tab(){
	isSellMode = true;
	_isBuyMode = false;

	InitShopInventory();
	RemoveText();
	
	for (int i = 0; i < 56; i++){
		socketArray[i]->SetHiddenState(true);
	}

	for (int i = 0; i < 56; i++){
		sell_Item_Socket[i]->SetHiddenState(true);
	}

}



// 인벤토리 마우스 오버 시 아이템 정보
void UIShop::InventoryItemInfo(){
	
	
	// 아이템정보
	if (_shopSocektDeligate->IsMouseOver() && _shopSocektDeligate->GetMouseOverIndex() != 10000){
		ItemInfo temp;
		UINT overIndex = _shopSocektDeligate->GetMouseOverIndex() - 1730;
		int itemLength = ItemManager::GetInstance()->MapItem.size();
	
		// 1   ,	2
		if (itemLength <= overIndex){
			RemoveText();
			
		}
		else{
		//	temp = _inventory->GetItem(overIndex)->GetInfo();
			
			itemInfoSocket->SetHiddenState(false);
	//		sellItemInfoArray[overIndex].Type
		switch (sellItemInfoArray[overIndex].Type)
			{
			case IT_SWORD:

			case IT_BOW:

			case IT_BODY:

			case IT_HEAD:

			case IT_GLOVE:

			case IT_SHOES:

			case IT_RING:

			case IT_NECKLESS:

			case IT_EARING:

			case IT_SHEILD:

			case IT_WAIST:

			case IT_POTION:
				
				SetItemInfo(sellItemInfoArray[overIndex]);
				break;

			case IT_NONE:
				RemoveText();
				break;
			default:
				break;
			}
		}



	}
	
}


void UIShop::SetItemInfo(tagItemInfo& info){
	if (info.Type == IT_NONE)return;

	itemInfoSocket->SetPosition(20, 330);

	std::string textArea, status;
	std::string disc;
	if (info.Type == IT_SWORD || info.Type == IT_BOW){
		status = "공격력 : " + std::to_string(info.Atk) + "\n";
	}
	else{
		status = "방어력 : " + std::to_string(info.Def) + "\n";
	}

	switch (info.RATING)
	{
	case ITR_COMMON:
		itemInfoNameBox->SetColor(D3DCOLOR_XRGB(127, 127, 127));
		break;
	case ITR_UNCOMMON:
		itemInfoNameBox->SetColor(D3DCOLOR_XRGB(47, 237, 40));
		break;
	case ITR_RARE:
		itemInfoNameBox->SetColor(D3DCOLOR_XRGB(0, 162, 201));
		break;
	case ITR_EPIC:
		itemInfoNameBox->SetColor(D3DCOLOR_XRGB(255, 187, 0));
		break;
	default:
		break;
	}

	itemInfoNameBox->SetText(info.Name);
	textArea = "\n";
	textArea += status;
	itemInfoBox->SetText(textArea.c_str());

	disc = info.Disc;
	itemInfoDiscBox->SetText(disc.c_str());

	if(itemInfoPrice1Box) itemInfoPrice1Box->SetText("구매금액 : ");

	if(itemInfoPrice2Box) itemInfoPrice2Box->SetText(std::to_string(info.Price));

	itemInfoSocket->SetBackground(info.IconPath);
}



void UIShop::RemoveText(){
	itemInfoSocket->SetHiddenState(true);
	itemInfoBox->SetText("");
	itemInfoDiscBox->SetText("");
	itemInfoPrice1Box->SetText("");
	itemInfoPrice2Box->SetText("");
	itemInfoNameBox->SetText("");

	
}


void UIShop::InitShopInventory(){

	if (!_isShopFirstClick){
		OpenButton();
		ZeroMemory(socketArray, sizeof(UISocket*) * 56);
		ZeroMemory(sellItemInfoArray, sizeof(tagItemInfo*) * 56);
		ZeroMemory(sell_Item_Socket, sizeof(UISocket*) * 56);



		UISocket* socket;
		_shopContent->SetSize(350, 450);
		int itemLength = ItemManager::GetInstance()->MapItem.size();
		int socketIndex = 0;

		for (int i = 0; i < 56; i++)
		{

			socket = new UISocket;
			socket->Setup();
			socket
				->AttachTo(_shopContent)
				->AttackToWindow(this)
				->SetSize(38, 38)
				->SetPosition(16 + (i % 8) * 40, 20 + (i / 8) * 40)
				->SetBackground("../Resources/Textures/icon/socket/item_slot.png")
				->SetDeligate(_shopSocektDeligate);
			//	->SetTagNum(SHOP_BUY_SOCKET(i));
			socket->SetDrawBoundingState(false);

			if (i < itemLength)socket->SetTagNum(SHOP_BUY_SOCKET(i));

			socketArray[i] = socket;
			sell_Item_Socket[i] = new UISocket;

			//SAFE_DELETE(socket);
		}


		for (auto i = ItemManager::GetInstance()->MapItem.begin(); i != ItemManager::GetInstance()->MapItem.end(); ++i){
			socket = new UISocket;
			socket->Setup();
			socket
				->AttachTo(_shopContent)
				->AttackToWindow(this)
				->SetSize(38, 38)
				->SetPosition(16 + (socketIndex % 8) * 40, 20 + (socketIndex / 8) * 40)
				->SetBackground("../Resources/Textures/icon/socket/item_slot.png")
				->SetDeligate(_shopSocektDeligate)
				->SetTagNum(SHOP_BUY_SOCKET(socketIndex));
			socket->SetDrawBoundingState(false);

			socket->SetBackground(i->second.IconPath);


			ItemInfo sellInfo;
			sellInfo.Atk = 10;
			sellItemInfoArray[socketIndex] = i->second;
			sell_Item_Socket[socketIndex] = socket;
			socketIndex++;
			//SAFE_DELETE(socket);
		}

		for (int i = 0; i < 56; i++){
			socketArray[i]->SetHiddenState(true);
		}

		for (int i = 0; i < 56; i++){
			sell_Item_Socket[i]->SetHiddenState(true);
		}
		_isShopFirstClick = true;
	}
}