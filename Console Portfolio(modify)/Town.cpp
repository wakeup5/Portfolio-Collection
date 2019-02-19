#include "Town.h"


Town::Town(UnitManager* unitManager, ItemManager* itemManager)
{
	_unitManager = unitManager;
	_itemManager = itemManager;
	_shop = new Shop(_unitManager->getPlayer(), _itemManager);
	_dungeonManager = new DungeonManager(unitManager);
}


Town::~Town()
{
	delete _shop, _dungeonManager;//, _gamble;
}

int Town::inTown()
{
	int townSelectNum = 0;

	GameSound::playBGM(SOUND_BGM_LIFE1);
	system("cls");
	while (true)
	{
		GameDisplay::image(GameImage::get(CG_TOWN));

		string dist[] = { "플레이어 정보", "상점", "지하 도박장", "외출", "맵" };
		townSelectNum = GameDisplay::choise("영인정신병원", dist, 5);

		if (townSelectNum == 0)
		{
			playerInfo();
		}
		else if (townSelectNum == 1)
		{
			_shop->openShop();
		}
		else if (townSelectNum == 2)
		{
			string gameDist[] = { "하이로우세븐", "섯다", "월남뽕" };
			int miniGameSelectNum = GameDisplay::choise("무엇을 하시겟습니까?", gameDist, 3);
			if (miniGameSelectNum == 0)
			{
				HighLowSeven game(_unitManager->getPlayer());
				game.startGame();
			}
			else if (miniGameSelectNum == 1)
			{
				Sutda game(_unitManager->getPlayer());
				game.startGame();
			}
			else if (miniGameSelectNum == 2)
			{
				Wolnamppong game(_unitManager->getPlayer());
				game.startGame();
			}
			else if (miniGameSelectNum == -1)
			{
				continue;
			}
		}
		else if (townSelectNum == 3)
		{
			Map map(_unitManager);
			int mapSelectNum = map.openMap();
			if (mapSelectNum != -1)
				_dungeonManager->openDungeon(mapSelectNum);
		}
	}

	return 0;
}

void Town::playerInfo()
{
	int playerInfoSelectNum = 0;
	Player* player = _unitManager->getPlayer();
	while (true)
	{
		GameDisplay::image(GameImage::get(CG_TOWN));

		string dist[] = { "능력치", "착용정보", "인벤토리", "스킬"};
		playerInfoSelectNum = GameDisplay::choise("플레이어 정보", dist, 4);

		if (playerInfoSelectNum == 0)
		{
			//player status
			system("cls");
			printf("Player status\n\n");
			printf("\n %10s \t\t %5d", "Level", player->getLevel());
			printf("\n %10s \t\t %5d", "exp", player->getExp());
			printf("\n\n %10s \t\t %5d / %5d", " HP / MP", player->getMaxHp() / player->getMaxHp());
			printf("\n\n %10s \t\t %5d", "Strength", player->getStr());
			printf("\n %10s \t\t %5d", "Dexterity", player->getDex());
			printf("\n %10s \t\t %5d", "Luck", player->getLuck());
			printf("\n\n %10s \t\t %5d", "Agility", player->getTotalAgi());
			printf("\n\n %10s \t\t %5d", "Atk Speed", player->getTotalAtkSpeed());
			printf("\n %10s \t\t %5d", "Move Speed", player->getTotalMoveSpeed());
			printf("\n\n %10s \t\t %5d", "Attack", player->getTotalAtk());
			printf("\n %10s \t\t %5d", "Defence", player->getTotalDef());
			printf("\n\n %10s \t\t %d", "소지금", player->getFund());
			_getch();
		}
		else if (playerInfoSelectNum == 1)
		{
			int equipSelectNum = 0;
			int input = 0;
			int itemSelectNum = 0;
			//Item* item;
			while (true)
			{
				system("cls");

				printf("Player Equipment\n\n");
				printf((equipSelectNum == ITEM_TYPE_ARMOR_HELMET)? " > ":"   ");
				printf(" %10s \t %20s\tMain : %5d, Sub : %5d\n\t\t%s\n", "Helmet"
					, player->getHelmet()->getName(), player->getHelmet()->getMain(), player->getHelmet()->getSub(), player->getHelmet()->getBrief());
				printf((equipSelectNum == ITEM_TYPE_ARMOR_TOP) ? " > " : "   ");
				printf(" %10s \t %20s\tMain : %5d, Sub : %5d\n\t\t%s\n", "Top"
					, player->getTop()->getName(), player->getTop()->getMain(), player->getTop()->getSub(), player->getTop()->getBrief());
				printf((equipSelectNum == ITEM_TYPE_ARMOR_PANTS) ? " > " : "   ");
				printf(" %10s \t %20s\tMain : %5d, Sub : %5d\n\t\t%s\n", "Pants"
					, player->getPants()->getName(), player->getPants()->getMain(), player->getPants()->getSub(), player->getPants()->getBrief());
				printf((equipSelectNum == ITEM_TYPE_ARMOR_SHOES) ? " > " : "   ");
				printf(" %10s \t %20s\tMain : %5d, Sub : %5d\n\t\t%s\n", "Shoes"
					, player->getShoes()->getName(), player->getShoes()->getMain(), player->getShoes()->getSub(), player->getShoes()->getBrief());
				printf((equipSelectNum == ITEM_TYPE_ARMOR_GLOVE) ? " > " : "   ");
				printf(" %10s \t %20s\tMain : %5d, Sub : %5d\n\t\t%s\n", "Glove"
					, player->getGlove()->getName(), player->getGlove()->getMain(), player->getGlove()->getSub(), player->getGlove()->getBrief());
				printf((equipSelectNum == ITEM_TYPE_WEAPON) ? " > " : "   ");
				printf(" %10s \t %20s\tMain : %5d, Sub : %5d\n\t\t%s\n", "Weapon"
					, player->getWeapon()->getName(), player->getWeapon()->getMain(), player->getWeapon()->getSub(), player->getWeapon()->getBrief());

				input = _getch();
				if (input == 224)
				{
					input = _getch();
					if (input == 72)
					{
						if (equipSelectNum > 0) equipSelectNum--;
					}
					else if (input == 80)
					{
						if (equipSelectNum < 5) equipSelectNum++;
					}
				}
				else if (input == 13)
				{
					vector<Item> inventory = _unitManager->getPlayer()->getInventory();
					if (inventory.size() == 0)
					{
						system("cls");
						printf("인벤토리에 아이템이 없습니다.");
						_getch();
						continue;
					}
					itemSelectNum = ItemManager::showItem(inventory);

					if (itemSelectNum == -1)
					{
						continue;
					}

					if (inventory.at(itemSelectNum).getType() == player->getEquipItem(ITEM_TYPE(equipSelectNum))->getType())
					{
						player->addItem(player->getEquipItem(ITEM_TYPE(equipSelectNum)));
						Item* item = new Item(inventory.at(itemSelectNum));
						player->setEquipItem(item);
						player->removeItem(itemSelectNum);
					}
					else
					{
						GameDisplay::dialogClear();
						printf("아이템 타입이 맞지 않습니다.");
						_getch();
					}
				}
				else if (input == 27)
				{
					//return -1;
					break;
				}
			}
		}
		else if (playerInfoSelectNum == 2)
		{
			system("cls");
			vector<Item> inventory = _unitManager->getPlayer()->getInventory();
			ItemManager::showItem(inventory);
		}
		else if (playerInfoSelectNum == 3)
		{
			//player = _unitManager->getPlayer();
			system("cls");
			printf(" Player Skills\n\n");
			vector<Skill> skills = player->getSkills();
			for (int i = 0; i < skills.size(); i++)
			{
				printf("\n\n %d", i + 1);
				printf("%20s", skills.at(i).getName());
				printf("\t %.0f%% + %d", skills.at(i).getPower() * 100, skills.at(i).getFixedPower());
				printf("\t CoolTime %.1fs", (float)skills.at(i).getCooltime() / 1000);
				printf("\n   - %s", skills.at(i).getBrief());
				
				//delete skill;
			}
			_getch();
		}
		else
		{
			break;
		}
	}

	return;

	
}