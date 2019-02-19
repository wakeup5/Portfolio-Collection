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
		printf("���� (�ּ� : %d) > ", _minBatting);
		fflush(stdin);
		scanf_s("%d", &input);

		if (input < _minBatting || input > fund)
		{
			printf("�߸� �Է��Ͽ����ϴ�.\n");
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
	string dist[] = {"���� ���", "������"};
	return GameDisplay::choise("��� �Ͻðڽ��ϱ�?", dist, 2);
}

void Gamble::gameOut()
{
	GameDisplay::dialogClear();
	printf("�̴� ������ �����մϴ�. �÷��̾ ȹ���� �ݾ��� %d �Դϴ�.", _fund - _player->getFund());
	_getch();
	_player->setFund(_fund);
}