#include "Shop.h"


Shop::Shop(Player* player, ItemManager* itemManager)
{
	_player = player;
	_itemManager = itemManager;
}


Shop::~Shop()
{
}

void Shop::openShop()
{
	string dist[] = {"사기", "팔기"};
	while (true)
	{
		GameDisplay::image(GameImage::get(CG_MAEJUM));

		int shopNum = GameDisplay::choise("무엇을 도와드릴까요?", dist, 2);
		if (shopNum == 0)
		{
			buyShop();
		}
		else if (shopNum == 1)
		{
			Story story;
			story.getEvent(GAME_EVENT_SHOP_SELL);
			sellShop();
		}
		else
		{
			break;
		}
	}
}

void Shop::buyShop()
{
	int shopSelectNum = 0;

	//GameSound::playBGM(SOUND_BGM_LIFE2);

	while (true)
	{
		GameDisplay::image(GameImage::get(CG_MAEJUM));

		string dist[] = { "무기", "모자", "상의", "하의", "신발", "장갑", "포션" };
		shopSelectNum = GameDisplay::choise("어떤걸 보여드릴까요?", dist, 7);
		if (shopSelectNum == 0)
		{
			showItem(ITEM_TYPE_WEAPON);
		}
		else if (shopSelectNum == 1)
		{
			showItem(ITEM_TYPE_ARMOR_HELMET);
		}
		else if (shopSelectNum == 2)
		{
			showItem(ITEM_TYPE_ARMOR_TOP);
		}
		else if (shopSelectNum == 3)
		{
			showItem(ITEM_TYPE_ARMOR_PANTS);
		}
		else if (shopSelectNum == 4)
		{
			showItem(ITEM_TYPE_ARMOR_SHOES);
		}
		else if (shopSelectNum == 5)
		{
			showItem(ITEM_TYPE_ARMOR_GLOVE);
		}
		else if (shopSelectNum == 6)
		{
			int potionSelectNum = 0;
			string itemDist[] = { "HP", "MP" };
			potionSelectNum = GameDisplay::choise("어떤걸 보여드릴까요? 포션 - ", itemDist, 2);

			if (potionSelectNum == 0)
			{
				showItem(ITEM_TYPE_POTION_HP);
			}
			else if (potionSelectNum == 1)
			{
				showItem(ITEM_TYPE_POTION_MP);
			}
			else if (potionSelectNum == -1)
			{
				continue;
			}
			//showItem(ITEM_TYPE_ARMOR_HELMET);
		}
		else
		{
			break;
		}
	}
}

void Shop::showItem(ITEM_TYPE type)
{
	GameDisplay::dialogClear();
	printf("player fund - %d", _player->getFund());
	vector<Item> items = *_itemManager->getItems(type);
	int selectNum = ItemManager::showItem(items);

	if (selectNum == -1)
	{
		return;
	}

	int selectBuyNum = 0;

	string dist[] = {"예", "아니오"};
	selectBuyNum = GameDisplay::choise("구입하시겟습니까?", dist, 2);

	if (selectBuyNum == 0)
	{
		if (_player->getFund() >= items.at(selectNum).getPrice())
		{
			_player->addItem(&items.at(selectNum));
			_player->setFund(_player->getFund() - items.at(selectNum).getPrice());
			GameDisplay::dialogClear();
			printf("  %s 아이템을 구매하였습니다.", items.at(selectNum).getName());
			_getch();
		}
		else
		{
			GameDisplay::dialogClear();
			printf("  돈이 부족합니다.");
			_getch();
		}
	}
}

void Shop::sellShop()
{
	system("cls");

	if (_player->getInventory().size() == 0)
	{
		printf(" 인벤토리에 아이템이 없습니다.");
		_getch();
		return;
	}

	vector<Item> items = _player->getInventory();
	int selectNum = ItemManager::showItem(items);

	if (selectNum == -1)
	{
		return;
	}

	int selectBuyNum = 0;

	string dist[] = { "예", "아니오" };
	selectBuyNum = GameDisplay::choise("판매하시겟습니까?", dist, 2);

	if (selectBuyNum == 0)
	{
		_player->setFund(_player->getFund() + items.at(selectNum).getPrice() / 2);
		GameDisplay::dialogClear();
		printf("  %s 아이템을 판매하였습니다.", items.at(selectNum).getName());
		
		//items.erase(items.begin() + selectNum);
		_player->removeItem(selectNum);
		_getch();
	}

}