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
	string dist[] = {"���", "�ȱ�"};
	while (true)
	{
		GameDisplay::image(GameImage::get(CG_MAEJUM));

		int shopNum = GameDisplay::choise("������ ���͵帱���?", dist, 2);
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

		string dist[] = { "����", "����", "����", "����", "�Ź�", "�尩", "����" };
		shopSelectNum = GameDisplay::choise("��� �����帱���?", dist, 7);
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
			potionSelectNum = GameDisplay::choise("��� �����帱���? ���� - ", itemDist, 2);

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

	string dist[] = {"��", "�ƴϿ�"};
	selectBuyNum = GameDisplay::choise("�����Ͻðٽ��ϱ�?", dist, 2);

	if (selectBuyNum == 0)
	{
		if (_player->getFund() >= items.at(selectNum).getPrice())
		{
			_player->addItem(&items.at(selectNum));
			_player->setFund(_player->getFund() - items.at(selectNum).getPrice());
			GameDisplay::dialogClear();
			printf("  %s �������� �����Ͽ����ϴ�.", items.at(selectNum).getName());
			_getch();
		}
		else
		{
			GameDisplay::dialogClear();
			printf("  ���� �����մϴ�.");
			_getch();
		}
	}
}

void Shop::sellShop()
{
	system("cls");

	if (_player->getInventory().size() == 0)
	{
		printf(" �κ��丮�� �������� �����ϴ�.");
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

	string dist[] = { "��", "�ƴϿ�" };
	selectBuyNum = GameDisplay::choise("�Ǹ��Ͻðٽ��ϱ�?", dist, 2);

	if (selectBuyNum == 0)
	{
		_player->setFund(_player->getFund() + items.at(selectNum).getPrice() / 2);
		GameDisplay::dialogClear();
		printf("  %s �������� �Ǹ��Ͽ����ϴ�.", items.at(selectNum).getName());
		
		//items.erase(items.begin() + selectNum);
		_player->removeItem(selectNum);
		_getch();
	}

}