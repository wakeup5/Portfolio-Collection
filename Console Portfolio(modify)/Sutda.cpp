#include "Sutda.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>

Sutda::Sutda(Player* player) : Gamble(player)
{
	_lineX = 10;
	_lineY = 10;
	//_fund = 10000;
	_minBatting = 100;

	_cardPosition = new CardPosition[16];
	_cardPosition[0] = { 0, 2 };
	_cardPosition[1] = { 8, 2 };
	_cardPosition[2] = { 0, 7 };
	_cardPosition[3] = { 8, 7 };
	_cardPosition[4] = { 0, 12 };
	_cardPosition[5] = { 8, 12 };
	_cardPosition[6] = { 0, 17 };
	_cardPosition[7] = { 8, 17 };
	_cardPosition[8] = { 0, 22 };
	_cardPosition[9] = { 8, 22 };
	_cardPosition[10] = { 0, 27 };
	_cardPosition[11] = { 8, 27 };
	_cardPosition[12] = { 0, 32 };
	_cardPosition[13] = { 8, 32 };
	_cardPosition[14] = { 0, 37 };
	_cardPosition[15] = { 8, 37 };
}


Sutda::~Sutda()
{
}

void Sutda::createCards()
{
	for (int i = 0; i < 20; i++)
	{
		_cards[i] = SUTDA_JOKBO(i);
	}
}

void Sutda::suffleCards()
{
	int temp, dest, sour;

	for (int i = 0; i < 500; i++)
	{
		dest = rand() % 20;
		sour = rand() % 20;

		temp = _cards[dest];
		_cards[dest] = _cards[sour];
		_cards[sour] = temp;
	}

}

void Sutda::printCard(int card, int y, int x)
{
	gotoxy(_lineX + x, _lineY + y);
	printf("┌──┐\n");
	gotoxy(_lineX + x, _lineY + y + 1);
	printf("│");
	cardName(card);
	printf("│\n");
	gotoxy(_lineX + x, _lineY + y + 2);
	printf("└──┘\n");
}

void Sutda::printBackCard(int y, int x)
{
	gotoxy(_lineX + x, _lineY + y);
	printf("┌──┐\n");
	gotoxy(_lineX + x, _lineY + y + 1);
	printf("│////│");
	gotoxy(_lineX + x, _lineY + y + 2);
	printf("└──┘\n");
}

void Sutda::clearDisplayAndCursor()
{
	gotoxy(_lineX + 30, _lineY + 1);
	printf("                                                ");
	gotoxy(_lineX + 30, _lineY + 2);
	printf("                                                ");
	gotoxy(_lineX + 30, _lineY + 1);
}

void Sutda::cardName(int card)
{
	char* name;
	if (card % 2 == 0 && ((card / 2) + 1 == 1 || (card / 2) + 1 == 3 || (card / 2) + 1 == 8))
		name = "光";
	else if (card % 2 == 0)
		name = "알";
	else
		name = "띠";
	printf("%2d%s", (card / 2) + 1, name);
}


void Sutda::inputGamePlayers()
{
	int input;
	char inputStr[20];

	while (true)
	{
		clearDisplayAndCursor();
		printf("시작할 인원 수 입력.(3~8) > ");
		scanf_s("%d", &input);

		if (input < 3 || input > 8)
		{
			printf("\n다시 입력하시오.\n");
			continue;
		}

		_personsNumber = input;
		_players = new tagPlayer[_personsNumber];
		break;
	}
	//입원 이름 입력
	strcpy_s(_players[0].name, "player");
	for (int i = 1; i < _personsNumber; i++)
	{
		clearDisplayAndCursor();
		printf("%d번째 이름을 입력하시오. > ", i + 1);
		scanf_s("%s", &inputStr, 20);
		strcpy_s(_players[i].name, inputStr);
	}
}

void Sutda::giveOutCard()
{
	for (int i = 0; i < _personsNumber; i++)
	{
		_players[i].cards[0] = _cards[_cardTurn++];
		_players[i].cards[1] = _cards[_cardTurn++];

		_players[i].pa = getCardPa(_players[i].cards[0], _players[i].cards[1]);
	}
}

int Sutda::batting()
{
	int input;
	char inputStr[100];
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
			clearDisplayAndCursor();
			printf("잘못 입력하였습니다.\n");
			continue;
		}

		setFund(fund - input);
		break;
	}

	clearDisplayAndCursor();
	printf("담보 입력 > ");

	scanf_s("%s", &inputStr, 100);
	strcpy_s(_players[0].dambo, inputStr);

	return input;
}

void Sutda::printGameInfo()
{
	gotoxy(_lineX, _lineY);
	printf("소지금 : %d           ", getFund());
}

void Sutda::startGame()
{
	int winnerNum = 0;
	int battingFund = 0;
	bool isDraw = false;

	printf("섯다 게임 실행 중..");

	system("cls");
	inputGamePlayers();
	createCards();
	while (true)
	{
		system("cls");
		_cardTurn = 0;
		suffleCards();//패 섞기
		giveOutCard();//패 나눠주기

		printGameInfo();

		//뒤집은 패 출력
		gotoxy(_lineX, _lineY + _cardPosition[0].y - 1);
		printf("%s 의 패 - %s     ", _players[0].name, cardPaName(_players[0].pa));
		printCard(_players[0].cards[0], _cardPosition[0].y, _cardPosition[0].x);
		printCard(_players[0].cards[1], _cardPosition[1].y, _cardPosition[1].x);
		for (int i = 1; i < _personsNumber; i++)
		{
			gotoxy(_lineX, _lineY + _cardPosition[i * 2].y - 1);
			printf("%s 의 패", _players[i].name);
			printBackCard(_cardPosition[i*2].y, _cardPosition[i*2].x);
			printBackCard(_cardPosition[i * 2 + 1].y, _cardPosition[i * 2 + 1].x);
		}
		
		battingFund = batting();//배팅

		printGameInfo();

		//패 앞장 출력.
		gotoxy(_lineX, _lineY + _cardPosition[0].y - 1);
		printf("%s 의 패 - %s     ", _players[0].name, cardPaName(_players[0].pa));
		for (int i = 1; i < _personsNumber; i++)
		{
			gotoxy(_lineX, _lineY + _cardPosition[i * 2].y - 1);
			printf("%s 의 패 - %s     ", _players[i].name, cardPaName(_players[i].pa));
			printCard(_players[i].cards[0], _cardPosition[i * 2].y, _cardPosition[i * 2].x);
			printCard(_players[i].cards[1], _cardPosition[i * 2 + 1].y, _cardPosition[i * 2 + 1].x);
		}
		
		//패 비교
		bool isTtangjab = false;
		bool isSagu = false;
		bool isGusa = false;
		bool isRegame = false;

		winnerNum = 0;
		for (int i = 1; i < _personsNumber; i++)
		{
			if (_players[winnerNum].pa > _players[i].pa)//단순히 패 비교하여 높은쪽이 이김.
			{
				winnerNum = i;
				isDraw = false;
			}
			else if (_players[winnerNum].pa == _players[i].pa) //둘의 패가 같을때
			{
				int wc1 = _players[winnerNum].cards[0] % 2 + 1;
				int wc2 = _players[winnerNum].cards[1] % 2 + 1;
				int pc1 = _players[i].cards[0] % 2 + 1;
				int pc2 = _players[i].cards[1] % 2 + 1;
				
				if (_players[winnerNum].pa == GGUT && _players[i].pa == GGUT)//끗일때
				{
					if ((wc1 + wc2) % 10 < (pc1 + pc2) % 10)//끝자리가 높은 끗 패가 이김
					{
						winnerNum = i;
						isDraw = false;
					}
					else if ((wc1 + wc2) % 10 == (pc1 + pc2) % 10)//무승부면 돈 날아감
					{
						isDraw = true;
						//printf("무승무가 나왔으므로 판을 엎습니다.\n");
						//Sleep(3000);
						//continue;
					}
				}
				else//끗이 아닐때
				{
					/*
					if ((wc1 + wc2) < (pc1 + pc2))//두개를 합산해서 높은쪽이 이김. 규칙 다르면 바뀔수 있다.
					{
						winnerNum = i;
					}
					*/
					isDraw = true;
				}
			}

			//구사나 사구파토, 땡잡이 등등
			if (_players[i].pa < TTANG_10)
			{
				isTtangjab = true;
			}
			if (_players[i].pa < ALI)
			{
				isGusa = true;
			}
			if (_players[i].pa < GANGTTANG_38)
			{
				isSagu = true;
			}

			if (_players[i].pa == GUSA && isGusa || _players[i].pa == SAGUPATO && isSagu)
			{
				clearDisplayAndCursor();
				printf("사구가 떳습니다. 판을 엎고 재시작 합니다.");
				Sleep(5000);
				isRegame = true;
			}

			if (_players[i].pa == TTANGJAP && isTtangjab)
			{
				winnerNum = i;
				clearDisplayAndCursor();
				printf("땡잡이가 떳습니다. %d번째 플레이어가 이깁니다.");
				break;
			}				
		}

		if (isRegame)
		{
			clearDisplayAndCursor();
			printf("게임을 재시작 합니다.");
			setFund(getFund() + battingFund);
			Sleep(5000);
			continue;
		}

		if (isDraw)
		{
			clearDisplayAndCursor();
			printf("무승부 입니다. 판 엎습니다.\n");
			setFund(getFund() + battingFund);
			Sleep(5000);
			continue;
		}

		//이긴사람 받은 돈 출력. - 플레이어가 이기면 돈 오름
		clearDisplayAndCursor();
		printf("%d번째 사람이 이김.\n", winnerNum + 1);
		if (winnerNum == 0)
		{
			clearDisplayAndCursor();
			printf("플레이어가 이겼습니다. %d만큼의 돈을 얻었습니다.\n", battingFund * _personsNumber);

			setFund(getFund() + (battingFund * _personsNumber));
		}


		//패배 조건 - 돈 다 잃으면 나가짐.
		if (getFund() <= 0)
		{
			clearDisplayAndCursor();
			printf("돈을 전부 잃었습니다.");
			gameOut();
			break;
		}
		
		//나가기 여부 - 나갈거냐고 물어봄.
		if (gameOutInput())
		{
			gameOut();
			break;
		}
	}
}

void Sutda::check()
{
	printf("|||%d|||", getCardPa(AL4, DDI9));
}

SUTDA_JOKBO Sutda::getCardPa(int card1, int card2)
{
	SUTDA_JOKBO result = GGUT;
	int card1Num = card1 / 2 + 1;
	int card2Num = card2 / 2 + 1;
	if (card1 == GANG3 && card2 == GANG8 || card1 == GANG8 && card2 == GANG3)
	{
		result = GANGTTANG_38;
	}
	else if ((card1 == GANG1 && card2 == GANG8 || card1 == GANG8 && card2 == GANG1)
		|| (card1 == GANG1 && card2 == GANG3 || card1 == GANG3 && card2 == GANG1))
	{
		result = GANGTTANG_1813;
	}
	else if (card1 == AL10 && card2 == DDI10 || card1 == DDI10 && card2 == AL10)
	{
		result = TTANG_10;
	}
	else if (card1 == AL9 && card2 == DDI9 || card1 == DDI9 && card2 == AL9)
	{
		result = TTANG_9;
	}
	else if (card1 == GANG8 && card2 == DDI8 || card1 == DDI8 && card2 == GANG8)
	{
		result = TTANG_8;//4
	}
	else if (card1 == AL7 && card2 == DDI7 || card1 == DDI7 && card2 == AL7)
	{
		result = TTANG_7;//5
	}
	else if (card1 == AL6 && card2 == DDI6 || card1 == DDI6 && card2 == AL6)
	{
		result = TTANG_6;//6
	}
	else if (card1 == AL5 && card2 == DDI5 || card1 == DDI5 && card2 == AL5)
	{
		result = TTANG_5;//7
	}
	else if (card1 == AL4 && card2 == DDI4 || card1 == DDI4 && card2 == AL4)
	{
		result = TTANG_4;//8
	}
	else if (card1 == GANG3 && card2 == DDI3 || card1 == DDI3 && card2 == GANG3)
	{
		result = TTANG_3;//9
	}
	else if (card1 == AL2 && card2 == DDI2 || card1 == DDI2 && card2 == AL2)
	{
		result = TTANG_2;//10
	}
	else if (card1 == GANG1 && card2 == DDI1 || card1 == DDI1 && card2 == GANG1)
	{
		result = TTANG_1;
	}
	else if ((card1 == DDI2 && card2 == DDI1 || card1 == DDI1 && card2 == DDI2) ||
		(card1 == GANG1 && card2 == DDI2 || card1 == DDI2 && card2 == GANG1) ||
		(card1 == DDI1 && card2 == AL2 || card1 == AL2 && card2 == DDI1) ||
		(card1 == GANG1 && card2 == AL2 || card1 == AL2 && card2 == GANG1)
		)
	{
		result = ALI;
	}
	else if ((card1 == DDI4 && card2 == DDI1 || card1 == DDI1 && card2 == DDI4) ||
		(card1 == GANG1 && card2 == DDI4 || card1 == DDI4 && card2 == GANG1) ||
		(card1 == DDI1 && card2 == AL4 || card1 == AL4 && card2 == DDI1) ||
		(card1 == GANG1 && card2 == AL4 || card1 == AL4 && card2 == GANG1)
		)
	{
		result = DOKSA;
	}
	else if ((card1 == DDI9 && card2 == DDI1 || card1 == DDI1 && card2 == DDI9) ||
		(card1 == GANG1 && card2 == DDI9 || card1 == DDI9 && card2 == GANG1) ||
		(card1 == DDI1 && card2 == AL9 || card1 == AL9 && card2 == DDI1) ||
		(card1 == GANG1 && card2 == AL9 || card1 == AL9 && card2 == GANG1)
		)
	{
		result = GUBBING;
	}
	else if ((card1 == DDI10 && card2 == DDI1 || card1 == DDI1 && card2 == DDI10) ||
		(card1 == GANG1 && card2 == DDI10 || card1 == DDI10 && card2 == GANG1) ||
		(card1 == DDI1 && card2 == AL10 || card1 == AL10 && card2 == DDI1) ||
		(card1 == GANG1 && card2 == AL10 || card1 == AL10 && card2 == GANG1)
		)
	{
		result = JANGBBING;
	}
	else if ((card1 == DDI10 && card2 == DDI4 || card1 == DDI4 && card2 == DDI10) ||
		(card1 == AL4 && card2 == DDI10 || card1 == DDI10 && card2 == AL4) ||
		(card1 == DDI4 && card2 == AL10 || card1 == AL10 && card2 == DDI4) ||
		(card1 == AL4 && card2 == AL10 || card1 == AL10 && card2 == AL4)
		)
	{
		result = JANGSA;
	}
	else if ((card1 == DDI6 && card2 == DDI4 || card1 == DDI4 && card2 == DDI6) ||
		(card1 == AL4 && card2 == DDI6 || card1 == DDI6 && card2 == AL4) ||
		(card1 == DDI4 && card2 == AL6 || card1 == AL6 && card2 == DDI4) ||
		(card1 == AL4 && card2 == AL6 || card1 == AL6 && card2 == AL4)
		)
	{
		result = SERUK;
	}
	else if ((card1Num + card2Num == 9) || (card1Num + card2Num == 19))
	{
		result = GAPO;
	}
	else if ((card1 == GANG3 && card2 == AL7) || (card2 == AL7 && card1 == GANG3))
	{
		result = TTANGJAP;
	}
	else if ((card1 == AL4 && card2 == DDI9) || (card2 == AL4 && card1 == DDI9) ||
		(card1 == DDI4 && card2 == DDI9) || (card2 == DDI4 && card1 == DDI9) ||
		(card1 == DDI4 && card2 == AL9) || (card2 == DDI4 && card1 == AL9))

	{
		result = GUSA;
	}
	else if ((card1 == AL4 && card2 == AL9) || (card2 == AL4 && card1 == AL9))
	{
		result = SAGUPATO;
	}
	else if ((card1Num + card2Num == 10))
	{
		result = MANGTONG;
	}
	else if (((card1Num + card2Num < 9) || (card1Num + card2Num > 0)) ||
		((card1Num + card2Num < 19) || (card1Num + card2Num > 10)))
	{
		result = GGUT;
	}

	return result;
}

char* Sutda::cardPaName(SUTDA_JOKBO pa)
{
	char* name = "";
	switch (pa)
	{
	case SAGUPATO:
		name = "사구파토";
		break;
	case GANGTTANG_38:
		name = "38광땡";
		break;
	case GANGTTANG_1813:
		name = "13,18광땡";
		break;
	case TTANG_10:
		name = "10땡";
		break;
	case TTANG_9:
		name = "9땡";
		break;
	case TTANG_8:
		name = "8땡";
		break;
	case TTANG_7:
		name = "7땡";
		break;
	case TTANG_6:
		name = "6땡";
		break;
	case TTANG_5:
		name = "5땡";
		break;
	case TTANG_4:
		name = "4땡";
		break;
	case TTANG_3:
		name = "3땡";
		break;
	case TTANG_2:
		name = "2땡";
		break;
	case TTANG_1:
		name = "1땡";
		break;
	case ALI:
		name = "알리";
		break;
	case DOKSA:
		name = "독사";
		break;
	case GUBBING:
		name = "구삥";
		break;
	case JANGBBING:
		name = "장삥";
		break;
	case JANGSA:
		name = "장사";
		break;
	case SERUK:
		name = "세륙";
		break;
	case GAPO:
		name = "갑오";
		break;
	case GGUT:
		name = "끗";
		break;
	case MANGTONG:
		name = "망통";
		break;
	case TTANGJAP:
		name = "땡잡";
		break;
	case GUSA:
		name = "구사";
		break;
	}

	return name;
	//printf("%s", name);
}