#pragma once
#include <string>
#include "GameDisplay.h"
#include "Unit.h"

using namespace std;

struct CardPosition
{
	int x;
	int y;
};

void gotoxy(int x, int y);
void setcolor(int color, int bgcolor);

enum MINI_GAME
{
	MINI_GAME_SUTDA,
	MINI_GAME_HIGHLOWSEVEN,
	MINI_GAME_WOLNAMPPONG
};

class Gamble
{
protected:
	Player* _player;

	int _fund;
	int _minBatting;

	int _lineX, _lineY;

	virtual void createCards();
	virtual void suffleCards();
	virtual void printCard(int, int = 0, int = 0);
	virtual void printBackCard(int = 0, int = 0);

	virtual int batting();
	virtual void clearDisplayAndCursor();
	virtual void printGameInfo();

	bool gameOutInput();
	void gameOut();
public:
	Gamble();
	Gamble(Player*);
	~Gamble();

	virtual void startGame();

	void setFund(int);
	int getFund();
};


