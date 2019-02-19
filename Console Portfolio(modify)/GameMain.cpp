#include "GameMain.h"

GameMain::GameMain()
{
	//_story = new Story();
	_itemManager = new ItemManager();
	_unitManager = new UnitManager();
	_town = new Town(_unitManager, _itemManager);

	GameDisplay::setDisplayInfo();
}


GameMain::~GameMain()
{
	//delete _story;
	delete _itemManager;
	delete _unitManager;
	delete _town;
}

void GameMain::startGame()
{
	opening();

	town();
}

void GameMain::opening()
{
	system("cls");
	Story story;
	story.opening();
	string dist[] = { "게임 시작", "게임 종료" };
	int gameStartNum = GameDisplay::choise("Watcher the console", dist, 2);
	if (gameStartNum != 0)
	{
		exit(0);
	}
	story.intro();
	story.hospital();
	system("cls");
}

void GameMain::town()
{
	_town->inTown();
}