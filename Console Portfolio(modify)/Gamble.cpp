#include "Gamble.h"
#include <iostream>
#include <time.h>
#include <Windows.h>

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

Gamble::Gamble(Player* player)
{
	srand((unsigned int)time(NULL));
	_fund = 0;
	_minBatting = 1;
	_lineX = 0;
	_lineY = 0;

	_player = player;
	_fund = _player->getFund();
}


Gamble::~Gamble()
{
}

int Gamble::getFund()
{
	return _fund;
}

void Gamble::setFund(int fund)
{
	_fund = fund;
}


void Gamble::clearDisplayAndCursor()
{
	gotoxy(_lineX, _lineY + 6);
	printf("                                                ");
	gotoxy(_lineX, _lineY + 7);
	printf("                                                ");
	gotoxy(_lineX, _lineY + 6);
}

int Gamble::batting()
{
	int input;
	int fund = getFund();

	while (true)
	{
		clearDisplayAndCursor();

		input = 0;
		printf("베팅 (최소 : %d) > ", _minBatting);
		fflush(stdin);
		scanf_s("%d", &input);

		if (input < _minBatting || input > fund)
		{
			printf("잘못 입력하였습니다.\n");
			continue;
		}

		setFund(fund - input);
		break;
	}

	return input;
}

void Gamble::createCards()
{
}
void Gamble::suffleCards()
{
}
void Gamble::printCard(int card, int x, int y)
{
}
void Gamble::printBackCard(int x, int y)
{
}
void Gamble::printGameInfo()
{
}
void Gamble::startGame()
{
}

bool Gamble::gameOutInput()
{
	string dist[] = {"게임 계속", "나가기"};
	return GameDisplay::choise("계속 하시겠습니까?", dist, 2);
}

void Gamble::gameOut()
{
	GameDisplay::dialogClear();
	printf("미니 게임을 종료합니다. 플레이어가 획득한 금액은 %d 입니다.", _fund - _player->getFund());
	_getch();
	_player->setFund(_fund);
}