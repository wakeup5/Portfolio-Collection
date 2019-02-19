#include "stdafx.h"
#include "Equipment.h"


Equipment::Equipment()
{
	ZeroMemory(_item, sizeof(Item*) * EQUIPTYPE_LENGTH);
	for (int i = 0; i < EQUIPTYPE_LENGTH; i++)
	{
		_item[i] = NULL;
	}
}


Equipment::~Equipment()
{

}

Item* Equipment::GetItem(ITEM_TYPE equipType)
{
	return _item[equipType];
}

Item* Equipment::Equip(Item* item)
{
//	if (_item[item->GetType()] != NULL) return 
	Item* temp = NULL;
	try{
		
		//temp = new Item;
	
		if (_item[item->GetInfo().Type] != NULL){
			if (item->GetInfo().Type == _item[item->GetInfo().Type]->GetInfo().Type){
				temp = _item[item->GetInfo().Type];
			}
			
		}
		else{
			temp = NULL;
		}

		if (item->GetInfo().Type == IT_SWORD){
			if (_item[IT_BOW]){
				temp = _item[IT_BOW];
				_item[IT_BOW] = NULL;
				//_item[item->GetInfo().Type] = new Item;
				_item[item->GetInfo().Type] = item;
			}
			else{
				//_item[item->GetInfo().Type] = new Item;
				_item[item->GetInfo().Type] = item;
			}
	
		}
		else if (item->GetInfo().Type == IT_BOW){
			if (_item[IT_SWORD]){
				temp = _item[IT_SWORD];
				_item[IT_SWORD] = NULL;
				//_item[item->GetInfo().Type] = new Item;
				_item[item->GetInfo().Type] = item;
			}
			else{
				//_item[item->GetInfo().Type] = new Item;
				_item[item->GetInfo().Type] = item;
			}
		
		}
		else{
			//_item[item->GetInfo().Type] = new Item;
			_item[item->GetInfo().Type] = item;
		}
		

		int sdf = 0;
		//_item[item->GetType()] = item;
	}
	catch (...){
		int a = 0;
	}
	return temp;
}

bool Equipment::UnEquip(ITEM_TYPE type, OUT Item** out)
{
	if (_item[type] == NULL) return false;

	*out = _item[type];

	return true;
}

bool Equipment::Swap(IN Item* in, OUT Item** out)
{
	//인 : 장착하려는 아이템
	//아웃 : 장착된 아이템
	if (in == NULL) return false;

	UnEquip(in->GetType(), out);
	Equip(in);

	return true;
}

ItemInfo Equipment::GetTotalStatus()
{
	ItemInfo result, temp;

	for (int i = 0; i < EQUIPTYPE_LENGTH; i++)
	{
		if (_item[i] == NULL) continue;

		temp = _item[i]->GetInfo();

		result.Atk += temp.Atk;
		result.Def += temp.Def;
		result.MaxHp += temp.MaxHp;
		result.MaxMp += temp.MaxMp;
	}

	return result;
}

void Equipment::Render()
{
	for (int i = 0; i < EQUIPTYPE_LENGTH; i++)
	{
		if (_item[i] != NULL)
		{
			_item[i]->Render();
		}
	}
}

void Equipment::MainWeaponRender()
{
	if (_item[IT_SWORD]) _item[IT_SWORD]->Render();
	if (_item[IT_SHEILD]) _item[IT_SHEILD]->Render();
}

void Equipment::SubWeaponRender()
{
	if (_item[IT_BOW]) _item[IT_BOW]->Render();
}
