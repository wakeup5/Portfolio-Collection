#include "Gamble.h"
#pragma once

enum HLS_GAME_RESULT
{
	HLS_GAME_HIGH_WIN, HLS_GAME_LOW_WIN, HLS_GAME_SEVEN_WIN, HLS_GAME_LOSE
};

class HighLowSeven : public Gamble
{
private:
	int _cards[52];
	int _viewCards[4];
	int _playerCard;

	int _cardTurn;
	int _gameTurn;
	CardPosition* _cardPosition;

	int selectHighLowSeven();

	void createCards();
	void suffleCards();
	void printCard(int, int = 0, int = 0);
	void printBackCard(int = 0, int = 0);

	void printGameInfo();

public:
	HighLowSeven(Player*);
	~HighLowSeven();

	void startGame(void);

};

