#include "GameMain.h"

GameMain::GameMain()
	//: _story(new Story())
	//, _display(new Display())
{
	system("mode CON COLS=128 LINES=66");
	system("title Watcher the console");
}

GameMain::~GameMain()
{
	//delete _story;
	//delete _display;
}

void GameMain::startGame()
{	
	opening();
	
	Container::getInstance()->getStory()->intro();
	
	map();
}

void GameMain::opening()
{
	Container::getInstance()->getStory()->opening();

	int selectNum = Display::choise("Watcher ~the console", new char*[]{ "게임 시작", "게임 종료" }, 2, " > ");

	if (selectNum != 0)
	{
		exit(0);
	}
}

void GameMain::map()
{
	Map map;
	map.openMap();
}