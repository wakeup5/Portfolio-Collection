#pragma once
#include "Gamble.h"

enum SUTDA_JOKBO
{
	GANGTTANG_38,
	GANGTTANG_1813,
	TTANG_10,
	TTANG_9,
	TTANG_8,
	TTANG_7,
	TTANG_6,
	TTANG_5,
	TTANG_4,
	TTANG_3,
	TTANG_2,
	TTANG_1,
	ALI,
	DOKSA,
	GUBBING,
	JANGBBING,
	JANGSA,
	SERUK,
	GAPO,
	GGUT,
	MANGTONG,
	SAGUPATO,
	TTANGJAP,
	GUSA
};

enum CARDVIEW
{
	GANG1, DDI1,
	AL2, DDI2,
	GANG3, DDI3,
	AL4, DDI4,
	AL5, DDI5,
	AL6, DDI6,
	AL7, DDI7,
	GANG8, DDI8,
	AL9, DDI9,
	AL10, DDI10
};

struct tagPlayer
{
	char name[20];
	int fund;
	char dambo[100];
	int cards[2];
	SUTDA_JOKBO pa;
};

class Sutda : public Gamble
{
private:
	int _cards[20];

	tagPlayer* _players;
	int _personsNumber;

	CardPosition* _cardPosition;

	int _cardTurn;

	void createCards();
	void suffleCards();
	void printCard(int, int = 0, int = 0);
	void printBackCard(int = 0, int = 0);
	void cardName(int);
	char* cardPaName(SUTDA_JOKBO);

	void clearDisplayAndCursor();
	void inputGamePlayers();
	void printGameInfo();
	void printCard(int);

	int batting();

	void giveOutCard();
	SUTDA_JOKBO getCardPa(int, int);
public:
	Sutda(Player*);
	~Sutda();

	void startGame();
	void check();
};

