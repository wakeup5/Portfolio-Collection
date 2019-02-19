#include "stdafx.h"
#include "UIInventory.h"

UIInventory::UIInventory()
{
}


UIInventory::~UIInventory()
{
}

void UIInventory::Setup(Inventory* inven, Equipment* equiped)
{
	UIWindow::Setup();

	
	_inventory = inven;	
	_equiped = equiped;
	
	this->SetPosition(WINSIZE_X - 428, 100);
	this->SetSize(400, 650);

	defaulDeligate = new UIDeligateDefault;
	
	this->SetBackground("../Resources/Textures/ui/ui_background.png");
	this->SetDeligate(defaulDeligate);
	this->SetHiddenState(true);
	this->SetTagNum(UITag::UI_INVENTORY);
	
	UITextBox* name = new UITextBox;
	name->Setup();
	name->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 20)
		->SetPosition(0, 30);

	name->SetText("인 벤 토 리");
	name->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);


	content = new UIWindow;
	content->Setup();
	content
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/ui_background_small.png")
		->SetSize(350, 550)
		->SetPosition(25, 75);


	UIWindow*  moneyIcon = new UIWindow;
	moneyIcon->Setup();
	moneyIcon
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/Icon/emblem_money.png")
		->SetSize(32, 32)
		->SetPosition(50, 550);

	moneyQTY = new UITextBox;
	moneyQTY->Setup();
	moneyQTY->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(100, 50)
		->SetPosition(87, 560);

	moneyQTY->SetText(std::to_string(PLAYER_MGR->GetPlayer()->GetTotalStatus().Price));
	moneyQTY->SetFontFormat( DT_WORDBREAK);



	_inventorySocket = new UIInventorySocketDeligate;
	_inventorySocket->init();
	UISocket* socket;
	for (int i = 0; i < 64; i++)
	{
	

		socket = new UISocket;
		socket->Setup();
		socket
			->AttachTo(content)
			->AttackToWindow(this)
			->SetSize(38, 38)
			->SetPosition(16 + (i % 8) * 40, 20 + (i / 8) * 40)
			->SetBackground("../Resources/Textures/icon/socket/item_slot.png")			
			->SetDeligate(_inventorySocket)
			->SetTagNum(INVENTORY_SOCKET(i));		
		
	}
	
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
		->AttachTo(content)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(20, 370)
		->SetBackground("../Resources/Textures/icon/socket/item_slot.png")
		->SetDeligate(_inventorySocket)
		->SetTagNum(INVENTORY_SOCKET(555));
	itemInfoSocket->SetHiddenState(true);

	// 아이템 박스 컬러값 설정
	itemInfoBox->SetColor(D3DCOLOR_XRGB(127, 127, 127));	
	itemInfoDiscBox->SetColor(D3DCOLOR_XRGB(255, 221, 115));	
	itemInfoPrice1Box->SetColor(D3DCOLOR_XRGB(127, 127, 127));	
	itemInfoPrice2Box->SetColor(D3DCOLOR_XRGB(211, 198, 115));

	nowTime = (float)TIME_MGR->GetLastTime();
}

void UIInventory::Release()
{
	UIWindow::Release();
}



void UIInventory::Toggle(){

	UIWindow::Toggle();
	RemoveText();
	if (!_isOpenInventory){
		_isOpenInventory = true;
		_inventorySocket->init();
		_inventorySocket->IsInventoryOpen(true);
	}
	else{
		_isOpenInventory = false;
		_inventorySocket->IsInventoryOpen(false);
	}


}


void UIInventory::Update()
{
	UIWindow::Update();

	if (KEY_MGR->IsOnceDown('I'))
	{
		this->Toggle();
		SOUND_MGR->play("inventory_open", 0.2);
	}	

	// 돈 업데이트
	moneyQTY->SetText(std::to_string(PLAYER_MGR->GetPlayer()->GetTotalStatus().Price));

	UISocket* s;
	std::vector<int> tempVec;
	for (int i = 0; i < INVENTORY_LENGTH; i++)
	{
		

		s = (UISocket*)GetRoot()->GetWindowByTag(INVENTORY_SOCKET(i));
		
		s->SetSocketObject(_inventory->GetItem(i));		
		if (_inventory->GetItem(i) == NULL)
			tempVec.push_back(i + 350);
	}
	_inventorySocket->SetNullCheckVector(tempVec);
	
	if (_isOpenInventory && _inventory != NULL){
	
	//	InventorySellClick();
		InventorySwap();
		InventoryItemInfo();

		if (nowTime + 0.3f < TIME_MGR->GetLastTime()){
			InventoryEquiped();
		}
		
	}
	
}

void UIInventory::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);
}



void UIInventory::RemoveText(){
	itemInfoSocket->SetHiddenState(true);
	itemInfoBox->SetText("");
	itemInfoDiscBox->SetText("");
	itemInfoPrice1Box->SetText("");
	itemInfoPrice2Box->SetText("");
	itemInfoNameBox->SetText("");
}


void UIInventory::SetItemInfo(ItemInfo& info){
	
	if (info.Type == IT_NONE)return;

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

	itemInfoPrice1Box->SetText("판매금액 : ");

	itemInfoPrice2Box->SetText(std::to_string(info.Price/2));

	itemInfoSocket->SetBackground(info.IconPath);
}


// 인벤토리 판매 선택
void UIInventory::InventorySellClick(){
	if (!isSellMode){
		return;
	}

	if (_inventorySocket->IsClick() && _inventorySocket->GetIndex() != 10000){
		ItemInfo temp;
		UINT overIndex = _inventorySocket->GetIndex() - 350;
		if (_inventory->GetItem(overIndex) == NULL){
	
		}
		else{
			PLAYER_MGR->GetPlayer()->GetTotalStatus().Price += (_inventory->GetItem(overIndex)->GetInfo().Price/2);
			_inventory->NullItem(overIndex);
			isSellMode = false;
			_inventorySocket->init();
			RemoveText();
			SOUND_MGR->play("sell_buy_item", 0.2);
		}
	
		UIShopButton::GetInstance()->_isShowBtn = false;
	}
	
}


// 인벤토리 교체
void UIInventory::InventorySwap(){
	if (isSellMode)return;
	// 아이템 교체
	if (_inventorySocket->GetIndex() - 350 > 64)return;
//	Item* it = _inventory->GetItem(_inventorySocket->GetIndex() - 350);

	if (_inventorySocket->GetIndex() != 10000){

		UINT clickIndex = _inventorySocket->GetIndex();
		if (_inventorySocket->GetDragIndex() != 10000){
			UINT moveIndex = _inventorySocket->GetDragIndex();
			if (_inventory->GetItem(clickIndex - 350) != NULL){
				
				for (int i = 0; i < 10; i++){
					if (UIShopButton::GetInstance()->screenItemArray[i] == clickIndex){
						UIShopButton::GetInstance()->screenItemArray[i] = _inventorySocket->GetDragIndex();
						break;
					}
				}

				_inventory->Swap(clickIndex - 350, _inventorySocket->GetDragIndex() - 350);			 // 350 소켓 태그 기본번호를 뺴서 보정
			}
			_inventorySocket->init();


		}

	}

}


// 아이템 장착
void UIInventory::InventoryEquiped(){
	if (isSellMode)return;
	//if (nowTime + 0.3f < TIME_MGR->GetLastTime()){
		if (_inventorySocket->GetUseItemIndex() != 10000){

			UINT itemIndex = _inventorySocket->GetUseItemIndex();
			if (_inventory->GetItem(itemIndex) == NULL){
				int ds = 0;
			}
			else{
				Item* it;
				ItemInfo info;
				it = _inventory->GetItem(itemIndex);

				UISocket* s;
				Item* eqIt;
				if (it->GetInfo().Type == IT_POTION){
					CharacterInfo infoTest = PLAYER_MGR->GetPlayer()->GetCharacterInfo();
		
					if (it->GetInfo().Name == "hp 포션"){
						PLAYER_MGR->GetPlayer()->GetTotalStatus().Hp += it->GetInfo().Def;
					
					}
					else{
						PLAYER_MGR->GetPlayer()->SetMp(it->GetInfo().Def);
					}
					_inventory->NullItem(itemIndex);
					_inventorySocket->init();
					
					return;
				}
				eqIt = _equiped->Equip(it);

				if (it->GetType() == IT_SWORD){
					it->pTransform = PLAYER_MGR->GetPlayer()->GetSwordTransform();
				}
				if (it->GetType() == IT_SHEILD){
					it->pTransform = PLAYER_MGR->GetPlayer()->GetShieldTransform();
				}
				
				if (eqIt == NULL){
					switch (it->GetInfo().Type)
					{
						case IT_SWORD:
						case IT_BOW:
							SOUND_MGR->play("weapon", 0.2);
							break;
						case IT_BODY:
						case IT_HEAD:
						case IT_GLOVE:
						case IT_SHOES:
						case IT_SHEILD:
							SOUND_MGR->play("armor", 0.2);
							break;
						case IT_RING:
						case IT_NECKLESS:
						case IT_EARING:
							SOUND_MGR->play("ring", 0.2);
							break;
						case IT_POTION:
							SOUND_MGR->play("weapon", 0.2);
							break;


					default:
						break; 
					}
				
					s = (UISocket*)GetRoot()->GetWindowByTag(INVENTORY_SOCKET(itemIndex));
					// 장착한곳의 아이템소켓을 빈 아이템으로 변경
					_inventory->NullItem(itemIndex);
					_inventorySocket->init();

					s->removeS();
				}
				else{
					switch (it->GetInfo().Type)
					{
					case IT_SWORD:
					case IT_BOW:
						SOUND_MGR->play("weapon", 0.2);
						break;
					case IT_BODY:
					case IT_HEAD:
					case IT_GLOVE:
					case IT_SHOES:
					case IT_SHEILD:
						SOUND_MGR->play("armor", 0.2);
						break;
					case IT_RING:
					case IT_NECKLESS:
					case IT_EARING:
						SOUND_MGR->play("ring", 0.2);
						break;
					case IT_POTION:
						SOUND_MGR->play("weapon", 0.2);
						break;


					default:
						break;
					}
					_inventory->SetItem(eqIt, itemIndex);
					_inventorySocket->init();
				}
			//	SAFE_DELETE(it);
			//	SAFE_DELETE(eqIt);
			}


		}
		nowTime = TIME_MGR->GetLastTime();
	//}

}


// 인벤토리 마우스 오버 시 아이템 정보
void UIInventory::InventoryItemInfo(){

	// 아이템정보
	if (_inventorySocket->IsMouseOver() && _inventorySocket->GetMouseOverIndex() != 10000){
		ItemInfo temp;
		UINT overIndex = _inventorySocket->GetMouseOverIndex() - 350;
		if (_inventory->GetItem(overIndex) == NULL){
			RemoveText();
		}
		else{
			temp = _inventory->GetItem(overIndex)->GetInfo();
			
			itemInfoSocket->SetHiddenState(false);
			switch (temp.Type)
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

			case IT_POTION:
				SetItemInfo(temp);
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


void UIInventory::AddInventory(std::string itemName){
	LOG_MGR->AddLog("터지는위치00");
	if (itemName == "" || itemName.length() < 0 || itemName.empty())return;
	
	
	/*
	for (int i = 0; i<itemName.length(); i++){
		char c = itemName.charAt(i);
		//한글 ( 한글자 || 자음 , 모음 )
		if ((0xAC00 <= c && c <= 0xD7A3) || (0x3131 <= c && c <= 0x318E)){
			System.out.println("k" + c);
			k++;
		}
		else if ((0x61 <= c && c <= 0x7A) || (0x41 <= c && c <= 0x5A)){
			//영어
			System.out.println("e:" + c);
			e++;
		}
		else if (0x30 <= c && c <= 0x39){
			//숫자
			System.out.println("d" + c);
			d++;
		}
		else{
			System.out.println("z" + c);
			z = 0;
		}
	}*/
	//LOG_MGR->AddLog("터지는위치");
	int nullIndex = -1;
	int forLength = 0;
	for (int i = 0; i < 64; i++){
		LOG_MGR->AddLog("%d", _inventory->GetItem(i) == NULL ? 0 : 1);
		if (_inventory->GetItem(i) == NULL){
			nullIndex = i;
			break;
		}
		forLength++;
	}
	//LOG_MGR->AddLog("터지는위치1111");
	if (nullIndex == -1)return;
	if (itemName == "[null]")return;

	Item* addItem = new Item;
	SOUND_MGR->play("sell_buy_item", 0.2);
	tagItemInfo temp = ItemManager::GetInstance()->MapItem.find(itemName)->second;
	//LOG_MGR->AddLog("터지는위치222222 : %s", temp.Name);
	//LOG_MGR->AddLog("리소스명ff : %s", temp.ResourcePath);
	//LOG_MGR->AddLog("리소스명fffff : %s", ItemManager::GetInstance()->MapItem.find(itemName)->second.ResourcePath);
	if (temp.Name == "")return;
	addItem->Setup(ItemManager::GetInstance()->MapItem.find(itemName)->second);
	//LOG_MGR->AddLog("리소스명 : %s", addItem->GetInfo().ResourcePath);
	_inventory->SetItem(addItem, nullIndex);
	_inventorySocket->init();
}


void UIInventory::UseItem(int idx){
	if (isSellMode)return;
	if (UIShopButton::GetInstance()->screenItemArray[idx] == NULL)return;
	Item* it;
	ItemInfo info;
	it = _inventory->GetItem(UIShopButton::GetInstance()->screenItemArray[idx] - 350);

	UISocket* s;
	Item* eqIt;
	if (it->GetInfo().Type == IT_POTION){
		CharacterInfo infoTest = PLAYER_MGR->GetPlayer()->GetCharacterInfo();

		if (it->GetInfo().Name == "hp 포션"){
			PLAYER_MGR->GetPlayer()->GetTotalStatus().Hp += it->GetInfo().Def;

		}
		else{
			PLAYER_MGR->GetPlayer()->SetMp(it->GetInfo().Def);
		}
		_inventory->NullItem(UIShopButton::GetInstance()->screenItemArray[idx] - 350);
		_inventorySocket->init();
		SAFE_DELETE(it);
		UIShopButton::GetInstance()->screenItemArray[idx] = NULL;
		return;
	}
}