#include "stdafx.h"
#include "UIEquippedItem.h"
#include "Equipment.h"
#include "SoundManager.h"
void UIEquippedItem::Setup(Equipment* equipmentItem)
{
	UIWindow::Setup();

	_equipment = equipmentItem;
	//_inventory = inven;
	//	soundManager::GetInstance()->addSound("ring", "../Resources/bgm/inventory_sound_effects/ring_inventory.wav", 0.3, 1, false);
	//soundManager::GetInstance()->Playsound("ring");
	
	this->SetPosition(100, 100);
	this->SetSize(400, 600);

	defaulDeligate = new UIDeligateDefault;
	//this->SetBackground("../Resources/Textures/I_on/Copy of df1_coral_1.jpg");
	this->SetBackground("../Resources/Textures/ui/ui_background.png");
	this->SetDeligate(defaulDeligate);
	this->SetHiddenState(true);
	this->SetTagNum(UITag::UI_EQUIPPEDITEM);
	
	UITextBox* name = new UITextBox;
	name->Setup();
	name->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 20)
		->SetPosition(0, 30);

	name->SetText("장     비");
	name->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);


	content = new UIWindow;
	content->Setup();
	content
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/아이템원.png")
		->SetSize(380, 380)
		->SetPosition(10, 100);

	_atk = 0;
	_def = 0;
	_statusAtkBox = new UITextBox;
	_statusAtkBox->Setup();
	_statusAtkBox->AttachTo(content)
		->AttackToWindow(this)
		->SetSize(400, 20)
		->SetPosition(0, 150);
	_statusAtkBox->SetColor(D3DCOLOR_XRGB(211, 198, 115));
	_statusAtkBox->SetText("공 격 력 : " + std::to_string(_atk));
	_statusAtkBox->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);

	_statusDefBox = new UITextBox;
	_statusDefBox->Setup();
	_statusDefBox->AttachTo(content)
		->AttackToWindow(this)
		->SetSize(400, 20)
		->SetPosition(0, 200);
	_statusDefBox->SetColor(D3DCOLOR_XRGB(211, 198, 115));
	_statusDefBox->SetText("방 어 력 : " + std::to_string(_def));
	_statusDefBox->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);



	sd = new UIInventorySocketDeligate;
	sd->init();
	/*
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
	*/
	_headSocket = new UISocket;
	_headSocket->Setup();
	_headSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(110, 100)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/투구.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_HEAD);
	
	_bodySocket = new UISocket;
	_bodySocket->Setup();
	_bodySocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(260, 100)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/갑옷.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_BODY);

	_earingSocket = new UISocket;
	_earingSocket->Setup();
	_earingSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(20, 41)
		->SetPosition(30, 170)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/귀걸이.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_EARING);

	_ringSocket = new UISocket;
	_ringSocket->Setup();
	_ringSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(340, 170)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/반지.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_RING);


	_handSocket = new UISocket;
	_handSocket->Setup();
	_handSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(370, 250)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/장갑.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_GLOVE);

	_shoesSocket = new UISocket;
	_shoesSocket->Setup();
	_shoesSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(0, 250)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/신발.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_SHOES);

	_waistSocket = new UISocket;
	_waistSocket->Setup();
	_waistSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(20, 360)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/허리띠.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_WAIST);

	_necklaceSocket = new UISocket;
	_necklaceSocket->Setup();
	_necklaceSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(350, 360)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/목걸이.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_NECKLESS);

	_weaphonSocket = new UISocket;
	_weaphonSocket->Setup();
	_weaphonSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(80, 430)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/무기.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_WEAPON);

	_shieldSocket = new UISocket;
	_shieldSocket->Setup();
	_shieldSocket
		->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(38, 38)
		->SetPosition(270, 430)
		->SetBackground("../Resources/검사_아이콘_작업/장착UI/방패.png")
		->SetDeligate(sd)
		->SetTagNum(UITag::UI_EQUIPPEDITEM_SHEILD);

	
}

void UIEquippedItem::Release()
{
	UIWindow::Release();
}


void UIEquippedItem::Update()
{
	UIWindow::Update();
	_equipment = PLAYER_MGR->GetEquipment();
	if (KEY_MGR->IsOnceDown('B'))
	{
		this->Toggle();
	}

	if (_isOpenInventory && _isFirst){
		this->Toggle();
		_isFirst = false;
	}
	else if (!_isOpenInventory && !_isFirst){
		this->Toggle();
		_isFirst = true;
	}

	ItemUpdate();


	UISocket* s;
	for (int i = 0; i < INVENTORY_LENGTH; i++)
	{
	//	s = (UISocket*)GetRoot()->GetWindowByTag(INVENTORY_SOCKET(i));

	//	s->SetSocketObject(_inventory->GetItem(i));
		//	s->SetBackground(_inventory->GetItem(i)->GetInfo().IconPath);
	}



}


bool isSound = false;
void UIEquippedItem::ItemUpdate(){

	CharacterInfo pi = PLAYER_MGR->GetPlayerTotalStatus();

	_statusAtkBox->SetText("공격력 : " + std::to_string((UINT)pi.Atk));
	_statusDefBox->SetText("방어력 : " + std::to_string((UINT)pi.Def));
	
	if (_equipment->GetItem(IT_SWORD) != NULL){
		_weaphonSocket->SetBackground(_equipment->GetItem(IT_SWORD)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_BOW) != NULL){
		_weaphonSocket->SetBackground(_equipment->GetItem(IT_BOW)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_BODY) != NULL){
		_bodySocket->SetBackground(_equipment->GetItem(IT_BODY)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_HEAD) != NULL){
		_headSocket->SetBackground(_equipment->GetItem(IT_HEAD)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_GLOVE) != NULL){
		_handSocket->SetBackground(_equipment->GetItem(IT_GLOVE)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_SHOES) != NULL){
		_shoesSocket->SetBackground(_equipment->GetItem(IT_SHOES)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_RING) != NULL){
		_ringSocket->SetBackground(_equipment->GetItem(IT_RING)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_NECKLESS) != NULL){
		_necklaceSocket->SetBackground(_equipment->GetItem(IT_NECKLESS)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_EARING) != NULL){
		_earingSocket->SetBackground(_equipment->GetItem(IT_EARING)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_SHEILD) != NULL){
		_shieldSocket->SetBackground(_equipment->GetItem(IT_SHEILD)->GetInfo().IconPath);
	}
	if (_equipment->GetItem(IT_WAIST) != NULL){
		_waistSocket->SetBackground(_equipment->GetItem(IT_WAIST)->GetInfo().IconPath);
	}


}


void UIEquippedItem::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);
}



void UIEquippedItem::RemoveText(){
	
}


void UIEquippedItem::SetItemInfo(ItemInfo& info){
	
}