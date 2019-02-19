#include "stdafx.h"
#include "ItemManager.h"

std::map<std::string, tagItemInfo> ItemManager::MapItem;

ItemManager::ItemManager()
{
}


ItemManager::~ItemManager()
{
}

Item * ItemManager::CreateItem(ItemInfo &info, Transform* trans)
{
//	if (MapItem.find(info.Name)->second.Name.size() > 0)return;

	Item* result = new Item;
	result->Setup(info);

	_items.push_back(result);
	


	if (trans != NULL)
	{
		SAFE_DELETE(result->pTransform);
		result->pTransform = trans;
	}

	return result;
}

Item * ItemManager::CreateItem(std::string name, Transform* trans)
{
	auto iter = MapItem.find(name);
	if (iter == MapItem.end()) return NULL;

	return CreateItem(iter->second, trans);
}

void ItemManager::LoadItemInfo()
{
	ItemInfo info;
	info = ItemInfo("이름", "설명", RESOURCE("Textures/Icon/머시기.png"), RESOURCE("Weapon/머시기.X"), IT_NONE, ITR_COMMON, 0, 0, 0, 0, 0, false);
	//MapItem.insert(std::make_pair(info.Name, info));
	//string testdat = "../Resources/Weapon/Sword_01.X";
	// 검 아이템 셋팅
	info = ItemInfo("민병대 장검", "초보 모험자를 위한 장검", "../Resources/검사_아이콘_작업/아이콘/장검/민병대_장검.png",
		RESOURCE("Weapon/Sword_01.X"), IT_SWORD, ITR_COMMON, 1000, 0, 0, 10, 0, true);
	info.SetMatrix(0.022, 0.022, 0.022, 0, 0, 0, 0.1, 0, 0.1);
	//info.ResourcePath = "../Resources/Weapon/Sword_01.X";
	MapItem.insert({ info.Name, info });

	info = ItemInfo("엘쉬 장검", "숙련된 모험자를 위한 장검", "../Resources/검사_아이콘_작업/아이콘/장검/엘쉬_장검.png",
		RESOURCE("Weapon/Sword_02.X"), IT_SWORD, ITR_UNCOMMON, 2000, 0, 0, 20, 0, true);
	info.SetMatrix(0.022, 0.022, 0.022, 0, 0, 0, 0.1, 0, 0.1);
	//info.ResourcePath = "../Resources/Weapon/Sword_02.X";
	MapItem.insert({ info.Name, info });

	info = ItemInfo("리베르토 장검", "고급 모험자를 위한 장검", "../Resources/검사_아이콘_작업/아이콘/장검/리베르토_장검.png",
		RESOURCE("Weapon/Sword_03.X"), IT_SWORD, ITR_RARE, 3000, 0, 0, 30, 0, true);
	info.SetMatrix(0.05, 0.05, 0.05, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 로사르 장검", "영웅적인 모험자를 위한 장검", "../Resources/검사_아이콘_작업/아이콘/장검/극_로사르_장검.png",
		RESOURCE("Weapon/Sword_04.X"), IT_SWORD, ITR_EPIC, 4000, 0, 0, 40, 0, true);
	info.SetMatrix(0.03, 0.03, 0.05, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });


	info = ItemInfo("엘쉬 활", "영웅적인 모험자를 위한 활", "../Resources/검사_아이콘_작업/아이콘/활/엘쉬_장궁.png",
		RESOURCE("Weapon/Bow.X"), IT_BOW, ITR_EPIC, 4000, 0, 0, 40, 0, true);
	info.SetMatrix(0.1, 0.1, 0.1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	//ITEM_MGR->CreateItem("민병대 장검");
	//ITEM_MGR->CreateItem("엘쉬 장검");
	//ITEM_MGR->CreateItem("리베르토 장검");
	//ITEM_MGR->CreateItem("극 로사르 장검");
	//ITEM_MGR->CreateItem("엘쉬 활");
	// 갑옷 아이템 셋팅
	info = ItemInfo("레플라스 갑옷", "초보 모험자를 위한 갑옷", "../Resources/검사_아이콘_작업/아이콘/방어구/갑옷/레플라스_갑옷.png",
		RESOURCE(".."), IT_BODY, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("강철 타타리스 갑옷", "숙련된 모험자를 위한 갑옷", "../Resources/검사_아이콘_작업/아이콘/방어구/갑옷/강철_타리타스_갑옷.png",
		RESOURCE(".."), IT_BODY, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 강철 타타리스 갑옷", "고급 모험자를 위한 갑옷", "../Resources/검사_아이콘_작업/아이콘/방어구/갑옷/극_강철_타리타스_갑옷.png",
		RESOURCE(".."), IT_BODY, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 바인더 갑옷", "영웅적인 모험자를 위한 갑옷", "../Resources/검사_아이콘_작업/아이콘/방어구/갑옷/극_바인더_갑옷.png",
		RESOURCE(".."), IT_BODY, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	// 귀걸이 아이템 셋팅
	info = ItemInfo("바인더 귀걸이", "초보 모험자를 위한 귀걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/귀걸이/바인더_귀걸이.png",
		RESOURCE(".."), IT_EARING, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("앨리샤 귀걸이", "숙련된 모험자를 위한 귀걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/귀걸이/앨리샤_귀걸이.png",
		RESOURCE(".."), IT_EARING, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("타락의 루비 귀걸이", "고급 모험자를 위한 귀걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/귀걸이/타락의_루비_귀걸이.png",
		RESOURCE(".."), IT_EARING, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("마녀의 귀장식", "영웅적인 모험자를 위한 귀걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/귀걸이/마녀의_귀장식.png",
		RESOURCE(".."), IT_EARING, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	// 목걸이 아이템 셋팅
	info = ItemInfo("바인더 목걸이", "초보 모험자를 위한 목걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/목걸이/바인더_목걸이.png",
		RESOURCE(".."), IT_NECKLESS, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("바레스 목걸이", "숙련된 모험자를 위한 목걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/목걸이/바레스_목걸이.png",
		RESOURCE(".."), IT_NECKLESS, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("불굴의 다이아몬드 목걸이", "고급 모험자를 위한 목걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/목걸이/불굴의_다이아몬드_목걸이.png",
		RESOURCE(".."), IT_NECKLESS, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("고대 수호자의 인장", "영웅적인 모험자를 위한 목걸이", "../Resources/검사_아이콘_작업/아이콘/방어구/목걸이/고대_수호자의_인장.png",
		RESOURCE(".."), IT_NECKLESS, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	
	// 반지 아이템 셋팅
	info = ItemInfo("바인더 반지", "초보 모험자를 위한 반지", "../Resources/검사_아이콘_작업/아이콘/방어구/반지/바인더_반지.png",
		RESOURCE(".."), IT_RING, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("류릭의 반지", "숙련된 모험자를 위한 반지", "../Resources/검사_아이콘_작업/아이콘/방어구/반지/류릭의_반지.png",
		RESOURCE(".."), IT_RING, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("네르트 반지", "고급 모험자를 위한 반지", "../Resources/검사_아이콘_작업/아이콘/방어구/반지/네르트_반지.png",
		RESOURCE(".."), IT_RING, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("초승달 수호자의 인장", "영웅적인 모험자를 위한 반지", "../Resources/검사_아이콘_작업/아이콘/방어구/반지/초승달_수호자의_반지.png",
		RESOURCE(".."), IT_RING, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	// 신발 아이템 셋팅
	info = ItemInfo("레플라스 신발", "초보 모험자를 위한 신발", "../Resources/검사_아이콘_작업/아이콘/방어구/신발/레플라스_신발.png",
		RESOURCE(".."), IT_SHOES, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("강철 타리타스 신발", "숙련된 모험자를 위한 신발", "../Resources/검사_아이콘_작업/아이콘/방어구/신발/강철_타리타스_신발.png",
		RESOURCE(".."), IT_SHOES, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 강철 타리타스 신발", "고급 모험자를 위한 신발", "../Resources/검사_아이콘_작업/아이콘/방어구/신발/극_강철_타리타스_신발.png",
		RESOURCE(".."), IT_SHOES, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 바인더 신발", "영웅적인 모험자를 위한 신발", "../Resources/검사_아이콘_작업/아이콘/방어구/신발/극_바인더_신발.png",
		RESOURCE(".."), IT_SHOES, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });
	
	
	// 장갑 아이템 셋팅
	info = ItemInfo("레플라스 신발", "초보 모험자를 위한 장갑", "../Resources/검사_아이콘_작업/아이콘/방어구/장갑/레플라스_장갑.png",
		RESOURCE(".."), IT_GLOVE, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("강철 타리타스 장갑", "숙련된 모험자를 위한 장갑", "../Resources/검사_아이콘_작업/아이콘/방어구/장갑/강철_타리타스_장갑.png",
		RESOURCE(".."), IT_GLOVE, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 강철 타리타스 장갑", "고급 모험자를 위한 장갑", "../Resources/검사_아이콘_작업/아이콘/방어구/장갑/극_강철_타리타스_장갑.png",
		RESOURCE(".."), IT_GLOVE, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 바인더 장갑", "영웅적인 모험자를 위한 장갑", "../Resources/검사_아이콘_작업/아이콘/방어구/장갑/극_바인더_장갑.png",
		RESOURCE(".."), IT_GLOVE, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	// 투구 아이템 셋팅
	info = ItemInfo("레플라스 신발", "초보 모험자를 위한 투구", "../Resources/검사_아이콘_작업/아이콘/방어구/투구/레플라스_투구.png",
		RESOURCE(".."), IT_HEAD, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("도바르트 투구", "숙련된 모험자를 위한 투구", "../Resources/검사_아이콘_작업/아이콘/방어구/투구/도바르트_투구.png",
		RESOURCE(".."), IT_HEAD, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 강철 타리타스 투구", "고급 모험자를 위한 투구", "../Resources/검사_아이콘_작업/아이콘/방어구/투구/극_강철_타리타스_투구.png",
		RESOURCE(".."), IT_HEAD, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 바인더 투구", "영웅적인 모험자를 위한 투구", "../Resources/검사_아이콘_작업/아이콘/방어구/투구/극_바인더_투구.png",
		RESOURCE(".."), IT_HEAD, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });
	

	// 허리띠 아이템 셋팅
	info = ItemInfo("바인더 허리띠", "초보 모험자를 위한 허리띠", "../Resources/검사_아이콘_작업/아이콘/방어구/허리띠/바인더_허리띠.png",
		RESOURCE(".."), IT_WAIST, ITR_COMMON, 1000, 0, 0, 0, 1, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("가르트너 허리띠", "숙련된 모험자를 위한 허리띠", "../Resources/검사_아이콘_작업/아이콘/방어구/허리띠/가르트너_허리띠.png",
		RESOURCE(".."), IT_WAIST, ITR_UNCOMMON, 2000, 0, 0, 0, 2, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("바실리스크의 허리띠", "고급 모험자를 위한 허리띠", "../Resources/검사_아이콘_작업/아이콘/방어구/허리띠/바실리스크의_허리띠.png",
		RESOURCE(".."), IT_WAIST, ITR_RARE, 3000, 0, 0, 0, 3, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("비밀이 담긴 허리띠", "영웅적인 모험자를 위한 허리띠", "../Resources/검사_아이콘_작업/아이콘/방어구/허리띠/비밀이_담긴_허리띠.png",
		RESOURCE(".."), IT_WAIST, ITR_EPIC, 4000, 0, 0, 0, 4, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	// 방패 셋팅
	info = ItemInfo("허름한 원형 방패", "초보 모험자를 위한 방패", "../Resources/검사_아이콘_작업/아이콘/방어구/방패/허름한 원형 방패.png", 
		RESOURCE("Weapon/Shiled01.X"), IT_SHEILD, ITR_COMMON, 1000, 0, 0, 0, 1, true);
	info.SetMatrix(0.02f, 0.02f, 0.02f, 0, 0, 0, 0, 0, 0);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("로사르 방패", "숙련된 모험자를 위한 방패", "../Resources/검사_아이콘_작업/아이콘/방어구/방패/로사르 방패.png",
		RESOURCE("Weapon/Shiled02.X"), IT_SHEILD, ITR_UNCOMMON, 2000, 0, 0, 0, 2, true);
	info.SetMatrix(0.1f, 0.1f, 0.1f, 0, 0, 0, 0, 0, 0);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("극 로사르 방패", "고급 모험자를 위한 방패", "../Resources/검사_아이콘_작업/아이콘/방어구/방패/극 로사르 방패.png",
		RESOURCE("Weapon/Shiled03.X"), IT_SHEILD, ITR_RARE, 3000, 0, 0, 0, 3, true);
	info.SetMatrix(0.2f, 0.2f, 0.2f, 0, 0, 0, 0, 0, 0);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("파괴의 로사르 방패", "영웅적인 모험자를 위한 방패", "../Resources/검사_아이콘_작업/아이콘/방어구/방패/파괴의 로사르 방패.png",
		RESOURCE("Weapon/Shiled04.X"), IT_SHEILD, ITR_EPIC, 4000, 0, 0, 0, 4, true);
	info.SetMatrix(0.1f, 0.1f, 0.1f, 0, 0, 0, 0, 0, 0);
	MapItem.insert({ info.Name, info });

	//ITEM_MGR->CreateItem("허름한 원형 방패");
	//ITEM_MGR->CreateItem("로사르 방패");
	//ITEM_MGR->CreateItem("극 로사르 방패");
	//ITEM_MGR->CreateItem("파괴의 로사르 방패");

	// 포션
	info = ItemInfo("hp 포션", "체력회복을위한 아이템", "../Resources/검사_아이콘_작업/아이콘/포션/hp포션.png",
		RESOURCE(""), IT_POTION, ITR_COMMON, 5000, 0, 0, 0, 100, false); // 너무 늦게 생각해서 포션의 경우 def로 회복 수치 결정
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });

	info = ItemInfo("mp 포션", "마나회복을위한 아이템", "../Resources/검사_아이콘_작업/아이콘/포션/mp포션.png",
		RESOURCE(""), IT_POTION, ITR_COMMON, 5000, 0, 0, 0, 100, false);
	info.SetMatrix(1, 1, 1, 0, 0, 0, 0.1, 0, 0.1);
	MapItem.insert({ info.Name, info });


}
