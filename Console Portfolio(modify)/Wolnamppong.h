#include "Gamble.h"
#pragma once

enum WNP_GAME_RESULT
{
	WNP_GAME_WIN, WNP_GAME_LOSE
};

class Wolnamppong : public Gamble
{
private:
	int _cards[52];
	CardPosition* _cardPosition;

	int _cardTurn;

	void createCards();
	void suffleCards();
	void printCard(int, int = 0, int = 0);
	void printBackCard(int = 0, int = 0);

	void printGameInfo();

public:
	Wolnamppong(Player* player);
	~Wolnamppong();

	void startGame();

};

