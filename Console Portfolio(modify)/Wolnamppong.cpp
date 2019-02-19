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
	printf("┌──┐\n");
	gotoxy(_lineX + x, _lineY + y + 1);
	printf("│    │\n");
	gotoxy(_lineX + x, _lineY + y + 2);
	printf("│");
	switch (card / 13)
	{
	case 0:
		printf("♤");
		break;
	case 1:
		printf("◆");
		break;
	case 2:
		printf("♥");
		break;
	case 3:
		printf("♧");
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
	printf("│\n");
	gotoxy(_lineX + x, _lineY + y + 3);
	printf("│    │\n");
	gotoxy(_lineX + x, _lineY + y + 4);
	printf("└──┘\n");
}

void Wolnamppong::printBackCard(int y, int x)
{
	gotoxy(_lineX + x, _lineY + y);
	printf("┌──┐\n");
	gotoxy(_lineX + x, _lineY + y + 1);
	printf("│////│\n");
	gotoxy(_lineX + x, _lineY + y + 2);
	printf("│////│");
	gotoxy(_lineX + x, _lineY + y + 3);
	printf("│////│\n");
	gotoxy(_lineX + x, _lineY + y + 4);
	printf("└──┘\n");
}

void Wolnamppong::printGameInfo()
{
	gotoxy(_lineX, _lineY);
	printf("소지금 : %d, 남은 카드수 : %d     ", getFund(), 52 - _cardTurn);
}

void Wolnamppong::startGame()
{
	int battingFund;
	int viewCards[3] = {};
	//WNP_GAME_RESULT gameResult = WNP_GAME_LOSE;

	char gameEndInput[] = "n";

	printf("월남뽕 실행중....\n");
	Sleep(1000);

	createCards();
	suffleCards();
	while (true)
	{
		system("cls");
		//카드 보여줌.
		for (int i = 0; i < 3; i++)
		{
			viewCards[i] = _cards[_cardTurn++];
		}
		printCard(viewCards[0], _cardPosition[0].y, _cardPosition[0].x); //첫번때 카드
		printBackCard(_cardPosition[2].y, _cardPosition[2].x);//세번째 가려진 카드
		printCard(viewCards[1], _cardPosition[1].y, _cardPosition[1].x); //두번때 카드
		
		printGameInfo();

		//베팅 시작
		battingFund = batting();

		printGameInfo();

		//세번째 카드 오픈
		printCard(viewCards[2], _cardPosition[2].y, _cardPosition[2].x);

		if (viewCards[2] % 13 < viewCards[0] % 13 && viewCards[2] % 13 > viewCards[1] % 13 || viewCards[2] % 13 > viewCards[0] % 13 && viewCards[2] % 13 < viewCards[1] % 13)
		{
			clearDisplayAndCursor();
			printf("이겼습니다.!");
			setFund(getFund() + battingFund * 2);
		}
		else
		{
			clearDisplayAndCursor();
			printf("졌습니다.!");
		}
		Sleep(1000);
		printGameInfo();

		//_cardTurn += 3;
		if (_cardTurn >= 51)//52까지 있으나 51이 3으로 나누어 떨어지는 수라..
		{
			clearDisplayAndCursor();
			printf("카드가 소진되서 다시 섞습니다.\n");
			Sleep(1000);
			suffleCards();
			_cardTurn = 0;
		}

		//패배
		if (getFund() <= 0)
		{
			clearDisplayAndCursor();
			printf("모든 돈을 잃었습니다. 게임을 종료합니다.\n");
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