#include "HighLowSeven.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>

HighLowSeven::HighLowSeven(Player* player) :Gamble(player)
{
	_cardPosition = new CardPosition[5];
	_cardPosition[0] = { 0, 1 };
	_cardPosition[1] = { 8, 1 };
	_cardPosition[2] = { 16, 1 };
	_cardPosition[3] = { 24, 1 };
	_cardPosition[4] = { 32, 1 };

	_cardTurn = 0;
	_gameTurn = 1;

	//_fund = 10000;
	_minBatting = 100;

	_lineX = 10;
	_lineY = 10;
}


HighLowSeven::~HighLowSeven()
{
	delete _cardPosition;
}

void HighLowSeven::createCards()
{
	for (int i = 0; i < 52; i++)
	{
		_cards[i] = i;
	}
}

void HighLowSeven::suffleCards()
{
	int temp, dest, sour;

	for (int i = 0; i < 500; i++)
	{
		dest = rand() % 52;
		sour = rand() % 52;

		temp = _cards[dest];
		_cards[dest] = _cards[sour];
		_cards[sour] = temp;
	}

}

void HighLowSeven::printCard(int card, int y, int x)
{
	gotoxy(_lineX + x, _lineY + y);
	printf("��������\n");
	gotoxy(_lineX + x, _lineY + y + 1);
	printf("��    ��\n");
	gotoxy(_lineX + x, _lineY + y + 2);
	printf("��");
	switch (card / 13)
	{
	case 0:
		printf("��");
		break;
	case 1:
		printf("��");
		break;
	case 2:
		printf("��");
		break;
	case 3:
		printf("��");
		break;
	}
	switch (card % 13)
	{
	case 0:
		printf("%2c", 'A');
		break;
	case 10:
		printf("%2c", 'J');
		break;
	case 11:
		printf("%2c", 'Q');
		break;
	case 12:
		printf("%2c", 'K');
		break;
	default:
		printf("%2d", card % 13 + 1);
		break;
	}
	printf("��\n");
	gotoxy(_lineX + x, _lineY + y + 3);
	printf("��    ��\n");
	gotoxy(_lineX + x, _lineY + y + 4);
	printf("��������\n");
}

void HighLowSeven::printBackCard(int y, int x)
{
	gotoxy(_lineX + x, _lineY + y);
	printf("��������\n");
	gotoxy(_lineX + x, _lineY + y + 1);
	printf("��////��\n");
	gotoxy(_lineX + x, _lineY + y + 2);
	printf("��////��");
	gotoxy(_lineX + x, _lineY + y + 3);
	printf("��////��\n");
	gotoxy(_lineX + x, _lineY + y + 4);
	printf("��������\n");
}

int HighLowSeven::selectHighLowSeven()
{
	int select = 0;
	int input;

	while (true)
	{
		clearDisplayAndCursor();

		input = 0;
		printf("1: �ο�, 2: ����, 3: ���� > ");
		fflush(stdin);
		scanf_s("%d", &input);

		if (input < 0 || input > 3)
		{
			printf("�߸� �Է��Ͽ����ϴ�.\n");
			continue;
		}

		select = input;
		break;
	}

	return select;
}

HLS_GAME_RESULT checkCard(int select, int card)
{
	HLS_GAME_RESULT result = HLS_GAME_LOSE;
	int cardNum = card % 13 + 1;

	switch (select)
	{
	case 1:
		if (cardNum < 7) result = HLS_GAME_LOW_WIN;
		break;
	case 2:
		if (cardNum == 7) result = HLS_GAME_SEVEN_WIN;
		break;
	case 3:
		if (cardNum > 7) result = HLS_GAME_HIGH_WIN;
		break;
	}

	return result;
}

void HighLowSeven::printGameInfo()
{
	gotoxy(_lineX, _lineY);
	printf("������ : %d, ���� ī��� : %d     ", getFund(), 52 - _cardTurn - 5);
}

void HighLowSeven::startGame()
{
	int select;
	int battingFund;
	HLS_GAME_RESULT gameResult = HLS_GAME_LOSE;

	char gameEndInput[] = "n";

	printf("���̷ο켼�� ������...\n");
	Sleep(1000);

	createCards();
	suffleCards();
	while (true)
	{
		system("cls");
		//system("cls");
		printGameInfo();
		for (int i = 0; i < 4; i++)
		{
			printCard(_cards[_cardTurn + i], _cardPosition[i].y, _cardPosition[i].x);
		}
		printBackCard(_cardPosition[4].y, _cardPosition[4].x);

		//����
		battingFund = batting();

		printGameInfo();

		//����, �ο�, ���� ����
		select = selectHighLowSeven();

		//������ �� �����ֱ�
		printCard(_cards[_cardTurn + 4], _cardPosition[4].y, _cardPosition[4].x);

		//ī�� ��
		gameResult = checkCard(select, _cards[_cardTurn + 4]);

		clearDisplayAndCursor();
		switch (gameResult)
		{
		case HLS_GAME_HIGH_WIN: case HLS_GAME_LOW_WIN:
			setFund(getFund() + battingFund * 2);
			printf("�̰���ϴ�!");
			break;
		case HLS_GAME_SEVEN_WIN:
			setFund(getFund() + battingFund * 13);
			printf("13��!! �̰���ϴ�!");
			break;
		case HLS_GAME_LOSE:
			printf("�����ϴ�.");
			break;
		}
		printGameInfo();

		Sleep(1000);

		_cardTurn++;
		if (_cardTurn + 5 > 52)
		{
			clearDisplayAndCursor();
			printf("ī�尡 �����Ǿ����ϴ�.\nī�带 �ٽ� �����ϴ�.\n");
			Sleep(1000);
			suffleCards();
			_cardTurn = 0;
		}

		//�й�
		if (getFund() <= 0)
		{
			clearDisplayAndCursor();
			printf("��� ���� �Ҿ����ϴ�. ������ �����մϴ�.\n");
			gameOut();
			break;
		}

		//���� ���� ���� - ���� �����ؼ� ��Ʈ�������� ���

		if (gameOutInput())
		{
			gameOut();
			break;
		}

		/*
		clearDisplayAndCursor();
		printf("������ ����Ͻðٽ��ϱ�? > ");
		scanf_s("%s", &gameEndInput);

		if (strcmp(gameEndInput, "n") == 0 || strcmp(gameEndInput, "N") == 0)
		{
			clearDisplayAndCursor();
			printf("������ �����մϴ�.\n");
			break;
		}
		*/
		

	}
}
