#include "Wolnamppong.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>

Wolnamppong::Wolnamppong(Player* player) :Gamble(player)
{
	_cardPosition = new CardPosition[5];
	_cardPosition[0] = { 0, 1 };
	_cardPosition[1] = { 16, 1 };
	_cardPosition[2] = { 8, 1 };

	_cardTurn = 0;

	//_fund = 10000;
	_minBatting = 100;
	_lineX = 10;
	_lineY = 10;
}

Wolnamppong::~Wolnamppong()
{
}

void Wolnamppong::createCards()
{
	for (int i = 0; i < 52; i++)
	{
		_cards[i] = i;
	}
}

void Wolnamppong::suffleCards()
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

void Wolnamppong::printCard(int card, int y, int x)
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

void Wolnamppong::printBackCard(int y, int x)
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

void Wolnamppong::printGameInfo()
{
	gotoxy(_lineX, _lineY);
	printf("������ : %d, ���� ī��� : %d     ", getFund(), 52 - _cardTurn);
}

void Wolnamppong::startGame()
{
	int battingFund;
	int viewCards[3] = {};
	//WNP_GAME_RESULT gameResult = WNP_GAME_LOSE;

	char gameEndInput[] = "n";

	printf("������ ������....\n");
	Sleep(1000);

	createCards();
	suffleCards();
	while (true)
	{
		system("cls");
		//ī�� ������.
		for (int i = 0; i < 3; i++)
		{
			viewCards[i] = _cards[_cardTurn++];
		}
		printCard(viewCards[0], _cardPosition[0].y, _cardPosition[0].x); //ù���� ī��
		printBackCard(_cardPosition[2].y, _cardPosition[2].x);//����° ������ ī��
		printCard(viewCards[1], _cardPosition[1].y, _cardPosition[1].x); //�ι��� ī��
		
		printGameInfo();

		//���� ����
		battingFund = batting();

		printGameInfo();

		//����° ī�� ����
		printCard(viewCards[2], _cardPosition[2].y, _cardPosition[2].x);

		if (viewCards[2] % 13 < viewCards[0] % 13 && viewCards[2] % 13 > viewCards[1] % 13 || viewCards[2] % 13 > viewCards[0] % 13 && viewCards[2] % 13 < viewCards[1] % 13)
		{
			clearDisplayAndCursor();
			printf("�̰���ϴ�.!");
			setFund(getFund() + battingFund * 2);
		}
		else
		{
			clearDisplayAndCursor();
			printf("�����ϴ�.!");
		}
		Sleep(1000);
		printGameInfo();

		//_cardTurn += 3;
		if (_cardTurn >= 51)//52���� ������ 51�� 3���� ������ �������� ����..
		{
			clearDisplayAndCursor();
			printf("ī�尡 �����Ǽ� �ٽ� �����ϴ�.\n");
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

		if (gameOutInput())
		{
			gameOut();
			break;
		}
		
	}
}