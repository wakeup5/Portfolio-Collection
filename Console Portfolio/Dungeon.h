#pragma once
#include <iostream>
#include <conio.h>
#include "Display.h"
#include "Battle.h"

class Container;

enum DUNGEON_TILE
{
	TILE_MOVING_1 = 1,
	TILE_WALL_1 = 9,
	TILE_WALL_11 = 19,
	TILE_DARK = 20,
	TILE_PLAYER = 30,
	//TILE_ENEMY = 40,
	TILE_ENEMY_1 = 40, TILE_ENEMY_2, TILE_ENEMY_3, TILE_ENEMY_4, TILE_ENEMY_5,
	TILE_ENEMY_6, TILE_ENEMY_7, TILE_ENEMY_8, TILE_ENEMY_9, TILE_ENEMY_10,
	TILE_ENEMY_11, TILE_ENEMY_12, TILE_ENEMY_13, TILE_ENEMY_14, TILE_ENEMY_15,
	TILE_ITEM_1 = 100,
	TILE_EVENT_1 = 300,
	TILE_NONE = 999
};

enum DUNGEON_POSITION
{
	DUNGEON_1,
	DUNGEON_2
};

struct tagDungeonInfo
{
	int* tile;
	int* originalTile;
	int width;
	int height;

	int playerX;
	int playerY;
	int enemyX;
	int enemyY;
};

using namespace std;

class Dungeon
{
private:
	tagDungeonInfo* _dungeons;
	//int* _dungeonOrginalTile[20];
	//int _player;
	int _playerX;
	int _playerY;
	int _enemyX;
	int _enemyY;

	void setDungeonInfo(tagDungeonInfo*, int[], int, int, int, int, int, int, int = TILE_ENEMY_1);

	void openDungeon(tagDungeonInfo*);
	void printDungeon(tagDungeonInfo*);
	void printTile(int);

	int moveUp(tagDungeonInfo*);
	int moveDown(tagDungeonInfo*);
	int moveLeft(tagDungeonInfo*);
	int moveRight(tagDungeonInfo*);

	int movePlayer(tagDungeonInfo*, int, int);

public:
	Dungeon();
	~Dungeon();

	void openDungeon1();
	void dungeon(int);
};

