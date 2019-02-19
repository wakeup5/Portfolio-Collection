#include "GameMain.h"
#include "Dungeon.h"

void main()
{

	//Dungeon d;
	
	//return;

	GameMain* game = new GameMain();
	game->startGame();

	delete game;

}