#include "Dungeon.h"

int Dungeon::objectNum = 100;

Dungeon::Dungeon(Player* player)
{
	_player = player;
}

Dungeon::Dungeon()
{

}

Dungeon::~Dungeon()
{
}

void Dungeon::setPlayer(Player* player)
{
	_player = player;
}
void Dungeon::setTile(int width, int height, int tile[20][20])
{
	_tile = new int*[height];
	for (int i = 0; i < height; i++)
	{
		_tile[i] = new int[width];
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_tile[i][j] = tile[i][j];
			if (tile[i][j] == TILE_PLAYER)
			{
				_playerX = j;
				_playerY = i;
			}
		}
	}
	_width = width;
	_height = height;
}

void Dungeon::addEnemy(int x, int y, Enemy* enemy)
{
	_tile[y][x] = Dungeon::objectNum;
	_objects.insert(map<int, tagTileObject>::value_type(Dungeon::objectNum++, tagTileObject{ TILE_ENEMY, enemy }));
}

void Dungeon::addEvent(int x, int y, GAME_EVENT e)
{
	int* en = new int(e);
	_tile[y][x] = Dungeon::objectNum;
	_objects.insert(map<int, tagTileObject>::value_type(Dungeon::objectNum++, tagTileObject{ TILE_EVENT, en }));
}

void Dungeon::open()
{
	DUNGEON_TILE tile = TILE_MOVE;
	system("cls");
	printDuneon();
	while (true)
	{
		GameDisplay::image();
		tile = movePlayer();
		printDuneon();
		if (tile >= 100)
		{
			map<int, tagTileObject>::iterator i = _objects.find(tile);
			if (i->second.tile == TILE_ENEMY)
			{
				GameDisplay::dialog();
				printf("적이다!");
				Battle battle(_player, (Enemy*)i->second.objectFunction);
				battle.startBattle();
				//GameDisplay::dialogClear();
				//GameDisplay::imageClear();
				system("cls");
				//break;
			}
			else if (i->second.tile == TILE_EVENT)
			{
				Story story;
				story.getEvent((GAME_EVENT)*(int*)i->second.objectFunction);
				system("cls");
			}
		}
		else if (tile == TILE_OUT)
		{
			string dist[] = { "계속 있기", "나가기" };
			int selectNum = GameDisplay::choise("나가시겟습니까?", dist, 2);
			if (selectNum == 1)
			{
				break;
			}
		}
	}
}

DUNGEON_TILE Dungeon::movePlayer()
{
	DUNGEON_TILE positionTile;

	if (_tile[_playerY][_playerX] != TILE_OUT)
	{
		_tile[_playerY][_playerX] = TILE_MOVE;
	}

	int inputKey = _getch();
	if (inputKey == 224)
	{
		inputKey = _getch();
		if (inputKey == 72)//위;
		{
			if (_tile[_playerY - 1][_playerX] != TILE_WALL && _playerY > 0) _playerY--;
		}
		else if (inputKey == 80)//아래;
		{
			if (_tile[_playerY + 1][_playerX] != TILE_WALL && _playerY < _height) _playerY++;
		}
		else if (inputKey == 75)//왼쪽;
		{
			if (_tile[_playerY][_playerX - 1] != TILE_WALL && _playerX > 0) _playerX--;
		}
		else if (inputKey == 77)//오른쪽;
		{
			if (_tile[_playerY][_playerX + 1] != TILE_WALL && _playerX < _width) _playerX++;
		}
		//printDuneon();
	}

	return (DUNGEON_TILE)_tile[_playerY][_playerX];
}

void Dungeon::printDuneon()
{
	//int x, y;
	float gradient;
	bool isLargeY = false;
	bool isDark = false;
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			isDark = false;
			isLargeY = false;

			if ((_playerX - x) == 0)
			{
				gradient = 0;
				isLargeY = true;
			}
			else gradient = static_cast<float>(_playerY - y) / static_cast<float>(_playerX - x);

			if (gradient <= -1.0 || gradient >= 1.0)
			{
				gradient = 1 / gradient;
				isLargeY = true;
			}
			//printf("%6.3f", gradient);

			if (_tile[y][x] == TILE_WALL)
			{
				isDark = false;
			}
			else
			{
				if (isLargeY)
				{
					//y가 크면
					if (y > _playerY)
					{
						double tx = static_cast<double>(_playerX);
						for (int j = _playerY + 1; j < y + 1; j++)//## x값이 같을때 그림자 표시가 안되는 버그 있음. 찾아보길..
						{
							tx += gradient;
							int k = (static_cast<int>((_playerX > x) ? ceil(tx) : floor(tx)));
							if (_tile[j][k] == TILE_WALL)
							{
								isDark = true;
								break;
							}
						}
					}
					else
					{
						double tx = static_cast<double>(_playerX);
						for (int j = _playerY - 1; j > y - 1; j--)
						{
							tx -= gradient;
							int k = (static_cast<int>((_playerX > x) ? ceil(tx) : floor(tx)));
							if (_tile[j][k] == TILE_WALL)
							{
								isDark = true;
								break;
							}
						}
					}
				}
				else
				{
					//x가 크면 x를 증감.
					if (x > _playerX)
					{
						double ty = static_cast<double>(_playerY);
						for (int j = _playerX + 1; j < x + 1; j++)
						{
							ty += gradient;
							int k = (static_cast<int>((_playerY >= y) ? ceil(ty) : floor(ty)));
							if (_tile[k][j] == TILE_WALL)
							{
								isDark = true;
								break;
							}
						}
					}
					else
					{
						double ty = static_cast<double>(_playerY);
						for (int j = _playerX - 1; j > x - 1; j--)
						{
							ty -= gradient;
							int k = (static_cast<int>((_playerY >= y) ? ceil(ty) : floor(ty)));
							if (_tile[k][j] == TILE_WALL)
							{
								isDark = true;
								break;
							}
						}
					}
				}
			}

			if (isDark)
			{
				//Display::setColor(DISPLAY_COLOR_DARKWHITE);
				printTile(TILE_DARK);
			}
			else if (x == _playerX && y == _playerY)
			{
				printTile(TILE_PLAYER);
			}
			else
			{
				if (_tile[y][x] >= 100)
				{
					map<int, tagTileObject>::iterator i = _objects.find(_tile[y][x]);
					printTile(i->second.tile);
				}
				else
				{
					printTile(_tile[y][x]);
				}
			}
			//printf("%d", _tile[y][x]);
		}
		printf("\n");
	}
	GameDisplay::setColor();
}

void Dungeon::printTile(int tile)
{
	char* t = "  ";
	switch (tile)
	{
	case TILE_MOVE:default:
		GameDisplay::setColor(DISPLAY_COLOR_WHITE, DISPLAY_COLOR_WHITE);
		t = "  ";
		break;
	case TILE_WALL:
		GameDisplay::setColor(DISPLAY_COLOR_DARKBLUE, DISPLAY_COLOR_DARKBLUE);
		t = "  ";
		break;
	case TILE_PLAYER:
		GameDisplay::setColor(DISPLAY_COLOR_BLUE, DISPLAY_COLOR_GRAY);
		t = "★";
		break;
	case TILE_ENEMY:
		GameDisplay::setColor(DISPLAY_COLOR_RED, DISPLAY_COLOR_GRAY);
		t = "★";
		break;
	case TILE_DARK:
		GameDisplay::setColor(DISPLAY_COLOR_RED, DISPLAY_COLOR_DARKWHITE);
		t = "  ";
		break;
	case TILE_ITEM:
		GameDisplay::setColor(DISPLAY_COLOR_BLUE, DISPLAY_COLOR_DARKBLUE);
		t = "＊";
		break;
	case TILE_OUT:
		GameDisplay::setColor(DISPLAY_COLOR_DARKSKY, DISPLAY_COLOR_DARKSKY);
		t = "＊";
		break;
	}
	printf(t);
	//GameDisplay::setColor();
}


DungeonManager::DungeonManager(UnitManager* unitManager)
{
	_unitManager = unitManager;

	for (int i = 0; i < 17; i++)
	{
		_dungeons[i] = new Dungeon(_unitManager->getPlayer());
	}

	int d1[20][20] =
	{
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 0, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7 },
		{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 7, 7, 7, 7, 4, 4, 7 },
		{ 7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7, 4, 4, 7 },
		{ 7, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 7, 7, 7, 7, 7, 7, 4, 4, 7, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 7, 4, 7, 4, 4, 7, 4, 4, 7, 7, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
	};
	_dungeons[0]->setTile(20, 20, d1);
	_dungeons[0]->addEnemy(18, 14, _unitManager->getEnemy(0));
	_dungeons[0]->addEvent(3, 1, GAME_EVENT_DUNGEON_1_1);
	_dungeons[0]->addEvent(3, 2, GAME_EVENT_DUNGEON_1_1);
}

DungeonManager::~DungeonManager()
{
	delete[] _dungeons;
}

void DungeonManager::openDungeon(int num)
{
	_dungeons[num]->open();
}