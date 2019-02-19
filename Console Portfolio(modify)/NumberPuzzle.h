#pragma once
#include "Gamble.h"

enum NP_GAME_INPUT_ARROW
{
	NP_GAME_INPUT_ARROW_NONE, NP_GAME_INPUT_ARROW_UP, NP_GAME_INPUT_ARROW_DOWN, NP_GAME_INPUT_ARROW_LEFT, NP_GAME_INPUT_ARROW_RIGHT
};

class NumberPuzzle : public Gamble
{
private:
	int _board[5][5];
	int _x, _y;

	NP_GAME_INPUT_ARROW getArrow();
	void createBoardAndSuffle();
	void print();
	void findZero();
	void positionChange(int, int);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
public:
	NumberPuzzle();
	~NumberPuzzle();
	void startGame();
};

