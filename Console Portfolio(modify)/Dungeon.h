#pragma once
#include <map>
#include "Unit.h"
#include "Story.h"
#include "Battle.h"

enum DUNGEON_TILE
{
	TILE_OUT,
	TILE_EVENT,
	TILE_ITEM,
	TILE_ENEMY,
	TILE_MOVE,
	TILE_PLAYER,
	TILE_DARK,
	TILE_WALL
};

struct tagTileObject
{
	DUNGEON_TILE tile;
	void* objectFunction;
};

class Dungeon
{
private:
	Player* _player;

	int _width, _height;
	int _playerX, _playerY;

	int** _tile;
	
	static int objectNum;
	map<int, tagTileObject> _objects;

	void printDuneon();
	void printTile(int);
	DUNGEON_TILE movePlayer();
public:
	Dungeon(Player* player);
	Dungeon();
	~Dungeon();

	void setPlayer(Player*);
	void setTile(int, int, int[20][20]);
	void addEnemy(int, int, Enemy*);
	void addEvent(int, int, GAME_EVENT);

	void open();
};


class DungeonManager
{
private:
	UnitManager* _unitManager;
	Dungeon* _dungeons[17];

public:
	DungeonManager(UnitManager*);
	~DungeonManager();

	void openDungeon(int);
};
