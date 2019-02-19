#include "Dungeon.h"
//#include "Story.h"
#include "Container.h"

Dungeon::Dungeon()
{
	_dungeons = new tagDungeonInfo[16];
	
	int dungeon1[] = {
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
		9, 1, 1, 1, 41, 42, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 9,
		9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 1, 1, 9, 1, 1, 9, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 9, 1, 1, 9, 1, 1, 1, 1, 1, 1, 9, 1, 9, 9,
		9, 1, 1, 9, 1, 1, 9, 1, 1, 9, 1, 1, 9, 9, 9, 9, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9,
		9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1, 1, 9, 9, 9, 9,
		9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9,
		9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 40, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9
	};
	
	setDungeonInfo(&_dungeons[0], dungeon1, 20, 20, 1, 1, 18, 18);
}

Dungeon::~Dungeon()
{
	delete[] _dungeons;
}

//���� ���������� Ŭ������ ���� �����ڷ� ó���ϰ� �ؾ� �ϴµ� �����Ƽ�..
void Dungeon::setDungeonInfo(tagDungeonInfo* d, int tile[], int width, int height, int playerX, int playerY, int enemyX, int enemyY, int enemy)
{
	d->width = width;
	d->height = height;
	d->playerX = playerX;
	d->playerY = playerY;
	d->enemyX = enemyX;
	d->enemyY = enemyY;
	d->tile = new int[width * height];
	d->originalTile = new int[width * height];

	for (int i = 0; i < width * height; i++)
	{
		d->tile[i] = tile[i];
		
		if (tile[i] < TILE_DARK)
			d->originalTile[i] = tile[i];
		else
			d->originalTile[i] = TILE_MOVING_1;
	}

	d->tile[(playerY * width) + playerX] = TILE_PLAYER;
	d->tile[(enemyY * width) + enemyX] = enemy;
}

void Dungeon::dungeon(int num)
{
	switch (num)
	{
	case 0:
		setDungeonInfo(&_dungeons[0], _dungeons[0].tile, 20, 20, 1, 1, 18, 18, TILE_ENEMY_1);
		break;
	default:
		setDungeonInfo(&_dungeons[0], _dungeons[0].tile, 20, 20, 1, 1, 18, 18, TILE_ENEMY_1);
		break;
	}
	openDungeon(&_dungeons[num]);
}

void Dungeon::printDungeon(tagDungeonInfo* d)
{
	//tagDungeonInfo* d = &du;
	int x, y;
	float gradient;
	bool isLargeY = false;
	bool isDark = false;
	for (int i = 0; i < d->width*d->height; i++)
	{
		x = i % d->width;
		y = i / d->width;
		isDark = false;
		isLargeY = false;

		//0ó��
		
		if ((d->playerX - x) == 0)
		{
			gradient = 0;
			isLargeY = true;
		}
		else gradient = static_cast<float>(d->playerY - y) / static_cast<float>(d->playerX - x);

		if (gradient <= -1.0 || gradient >= 1.0)
		{
			gradient = 1 / gradient;
			isLargeY = true;
		}
		

		//�ΰ��� �� ���̿� ���� ������?
		//���⿡�� y�� ũ�� y�� ����. ������ y.
		if (isLargeY)
		{
			//y�� ũ��
			if (y > d->playerY)
			{
				double tx = static_cast<double>(d->playerX);
				for (int j = d->playerY + 1; j < y + 1; j++)//## x���� ������ �׸��� ǥ�ð� �ȵǴ� ���� ����. ã�ƺ���..
				{
					tx += gradient;
					int k = (j * d->width) + (static_cast<int>((d->playerX > x) ? ceil(tx) : floor(tx)));
					if (d->tile[i] == TILE_WALL_1)
					{
						isDark = false;
						break;
					}
					else if (d->tile[k] == TILE_WALL_1)
					{
						isDark = true;
						break;
					}
				}
			}
			else
			{
				double tx = static_cast<double>(d->playerX);
				for (int j = d->playerY - 1 ; j > y - 1; j--)
				{
					tx -= gradient;
					int k = (j * d->width) + (static_cast<int>((d->playerX > x) ? ceil(tx) : floor(tx)));
					if (d->tile[i] == TILE_WALL_1)
					{
						isDark = false;
						break;
					}
					else if (d->tile[k] == TILE_WALL_1)
					{
						isDark = true;
						break;
					}
				}
			}
		}
		else
		{
			//x�� ũ�� x�� ����.
			if (x > d->playerX)
			{
				double ty = static_cast<double>(d->playerY);
				for (int j = d->playerX + 1; j < x + 1; j++)
				{
					ty += gradient;
					int k = (static_cast<int>((d->playerY >= y) ? ceil(ty) : floor(ty)) * d->width) + j;
					if (d->tile[i] == TILE_WALL_1)
					{
						isDark = false;
						break;
					}
					else if (d->tile[k] == TILE_WALL_1)
					{
						isDark = true;
						break;
					}
				}
			}
			else
			{
				double ty = static_cast<double>(d->playerY);
				for (int j = d->playerX - 1; j > x - 1; j--)
				{
					ty -= gradient;
					int k = (static_cast<int>((d->playerY >= y) ? ceil(ty) : floor(ty)) * d->width) + j;
					if (d->tile[i] == TILE_WALL_1)
					{
						isDark = false;
						break;
					}
					else if (d->tile[k] == TILE_WALL_1)
					{
						isDark = true;
						break;
					}
				}
			}
		}

				
		if (isDark)
		{
			//Display::setColor(DISPLAY_COLOR_DARKWHITE);
			printTile(TILE_DARK);
		}
		else if (x == d->playerX && y == d->playerY)
		{
			printTile(TILE_PLAYER);
		}
		//else if (x == d->enemyX && y == d->enemyY)
		//{
			//Display::setColor(DISPLAY_COLOR_DARKRED);
			//printTile(TILE_ENEMY);
		//}
		else
		{
			printTile(d->tile[i]);
		}
		//printf("%6.3f", gradient);
		Display::setColor();
		
		if ((i % d->width) + 1 == d->width)
			cout << endl;
	}
}

void Dungeon::openDungeon(tagDungeonInfo* d/*, �� ����*/)
{
	int inputKey;
	int positionThing;
	int x, y;
	
	Display::image();
	printDungeon(d);

	while (true)
	{
		inputKey = -1;
		//Ű�޾Ƽ� ������.
		if (_getch() == 224)//ȭ��ǥ Ű�� ��������
		{
			x = d->playerX;
			y = d->playerY;

			inputKey = _getch();
			if (inputKey == 72)//��;
			{
				y--;
			}
			else if (inputKey == 80)//�Ʒ�;
			{
				y++;
			}
			else if (inputKey == 75)//����;
			{
				x--;
			}
			else if (inputKey == 77)//������;
			{
				x++;
			}

			positionThing = movePlayer(d, x, y);
		}
		else
		{
			continue;
		}
		//�� �����̴� �ΰ������ε�.. ���?

		
		//Display::dialogClear();
		//printf("%d", positionThing);
		if (positionThing >= TILE_ENEMY_1 && positionThing < TILE_ITEM_1) //�� ������ (�����ۺ��� ���� �켱 ó��)
		{
			//��Ʋ!
			//printf("���� ��Ÿ����.");
			Player* player = Container::getInstance()->getUnitManager()->getPlayer();
			Enemy* enemy = Container::getInstance()->getUnitManager()->getEnemy(ENEMY_1);

			Battle battle(player, enemy);
			battle.startBattle();

			break;
		}
		else if (positionThing >= TILE_ITEM_1 && positionThing < TILE_EVENT_1) //������ ������(���丮 ������)
		{
			//������ ó��!
			printf("�������̴�.");

		}
		else if (positionThing >= TILE_EVENT_1 && positionThing < TILE_NONE)
		{

		}

		Display::image();
		printDungeon(d);
	}
}

void Dungeon::printTile(int tile)
{
	char* t = "  ";
	switch (tile)
	{
	case TILE_MOVING_1:
		Display::setColor(DISPLAY_COLOR_WHITE, DISPLAY_COLOR_WHITE);
		//t = "  ";
		break;
	case TILE_WALL_1:
		Display::setColor(DISPLAY_COLOR_DARKBLUE, DISPLAY_COLOR_DARKBLUE);
		//t = "  ";
		break;
	case TILE_PLAYER:
		Display::setColor(DISPLAY_COLOR_BLUE, DISPLAY_COLOR_GRAY);
		t = "��";
		break;
	/*case TILE_ENEMY:*/ case TILE_ENEMY_1: case TILE_ENEMY_2: case TILE_ENEMY_3: case TILE_ENEMY_4: case TILE_ENEMY_5:
	case TILE_ENEMY_6: case TILE_ENEMY_7: case TILE_ENEMY_8: case TILE_ENEMY_9: case TILE_ENEMY_10: case TILE_ENEMY_11:
	case TILE_ENEMY_12: case TILE_ENEMY_13: case TILE_ENEMY_14: case TILE_ENEMY_15:
		Display::setColor(DISPLAY_COLOR_RED, DISPLAY_COLOR_GRAY);
		t = "��";
		break;
	case TILE_DARK:
		Display::setColor(DISPLAY_COLOR_RED, DISPLAY_COLOR_DARKWHITE);
		t = "  ";
		break;
	default:
		//t = "  ";
		break;
	}
	printf(t);
	Display::setColor();
}

/* �Ⱦ��� �͵� {*/
int Dungeon::moveUp(tagDungeonInfo* d)
{
	if (d->playerY <= 0) return 0;

	int tile = d->tile[(d->playerY - 1) * d->width + d->playerX];
	if (tile >= TILE_WALL_1 && tile < TILE_WALL_11) return 0;

	//d->playerY--;
	return movePlayer(d, d->playerY - 1, d->playerX);
}
int Dungeon::moveDown(tagDungeonInfo* d)
{
	if (d->playerY >= d->height - 1) return 0;

	int tile = d->tile[(d->playerY + 1) * d->width + d->playerX];
	if (tile >= TILE_WALL_1 && tile < TILE_WALL_11) return 0;

	//d->playerY++;
	return movePlayer(d, d->playerY + 1, d->playerX);
}
int Dungeon::moveLeft(tagDungeonInfo* d)
{
	if (d->playerX <= 0) return 0;
	int tile = d->tile[d->playerY * d->width + d->playerX - 1];
	if (tile >= TILE_WALL_1 && tile < TILE_WALL_11) return 0;

	//d->playerX--;
	return movePlayer(d, d->playerY, d->playerX - 1);
}
int Dungeon::moveRight(tagDungeonInfo* d)
{
	if (d->playerX >= d->height) return 0;
	int tile = d->tile[d->playerY * d->width + d->playerX + 1];
	if (tile >= TILE_WALL_1 && tile < TILE_WALL_11) return 0;

	//d->playerX++;
	return movePlayer(d, d->playerY, d->playerX + 1);
}
/*} �Ⱦ��� �͵� */

int Dungeon::movePlayer(tagDungeonInfo* d, int x, int y)
{
	if (d->playerX >= d->height) return 0;
	int tile = d->tile[y * d->width + x];
	if (tile >= TILE_WALL_1 && tile < TILE_WALL_11) return 0;

	int result = d->tile[y * d->width + x];

	d->tile[d->playerY * d->width + d->playerX] = d->originalTile[d->playerY * d->width + d->playerX];
	d->tile[y * d->width + x] = TILE_MOVING_1;

	d->playerX = x;
	d->playerY = y;

	return result;
}