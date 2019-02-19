#include "NumberPuzzle.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>

NumberPuzzle::NumberPuzzle()
{
	_x = 0;
	_y = 0;
}


NumberPuzzle::~NumberPuzzle()
{
}

void NumberPuzzle::startGame()
{
	createBoardAndSuffle();
	while (true)
	{
		findZero();
		print();
		switch (getArrow())
		{
		case NP_GAME_INPUT_ARROW_UP:
			moveUp();
			break;
		case NP_GAME_INPUT_ARROW_DOWN:
			moveDown();
			break;
		case NP_GAME_INPUT_ARROW_LEFT:
			moveLeft();
			break;
		case NP_GAME_INPUT_ARROW_RIGHT:
			moveRight();
			break;
		default:
			break;
		}
	}
}

NP_GAME_INPUT_ARROW NumberPuzzle::getArrow()
{
	int input;
	NP_GAME_INPUT_ARROW result = NP_GAME_INPUT_ARROW_NONE;
	input = _getch();

	switch (input)
	{
	case 72: case 56:
		result = NP_GAME_INPUT_ARROW_UP;
		break;
	case 75: case 52:
		result = NP_GAME_INPUT_ARROW_LEFT;
		break;
	case 77: case 54:
		result = NP_GAME_INPUT_ARROW_RIGHT;
		break;
	case 80: case 50:
		result = NP_GAME_INPUT_ARROW_DOWN;
		break;
	case 224: default:
		result = NP_GAME_INPUT_ARROW_NONE;
		break;
	}
	printf("input : %d\n", input);

	return result;
}

void NumberPuzzle::createBoardAndSuffle()
{
	//int dest, sour, temp;

	for (int i = 0; i < 25; i++)
	{
		_board[i / 5][i % 5] = 24 - i;//숫자가 거꾸로 들어가도록.
	}
	/*
	for (int i = 0; i < 1000; i++)
	{
	dest = (rand() % 24);
	sour = (rand() % 24);

	temp = board[dest / 5][dest % 5];
	board[dest / 5][dest % 5] = board[sour / 5][sour % 5];
	board[sour / 5][sour % 5] = temp;
	}
	*/
}

void NumberPuzzle::print()
{
	system("cls");
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf("%d\t", _board[i][j]);
		}
		printf("\n");
	}
	printf("\n\ny : %d, x : %d\n", _y, _x);
}

void NumberPuzzle::findZero(void)
{
	for (int i = 0; i < 25; i++)
	{
		if (_board[i / 5][i % 5] == 0)
		{
			_x = i % 5;
			_y = i / 5;
			return;
		}
	}
}

void NumberPuzzle::positionChange(int n3, int n4)
{
	int temp;

	temp = _board[_y][_x];
	_board[_y][_x] = _board[n3][n4];
	_board[n3][n4] = temp;

	//x = n4;
	//y = n3;

	//findZero();
}

void NumberPuzzle::moveUp()
{
	if (_y <= 0) return;

	positionChange(_y - 1, _x);
}

void NumberPuzzle::moveDown()
{
	if (_y >= 4) return;

	positionChange(_y + 1, _x);
}

void NumberPuzzle::moveLeft()
{
	if (_x <= 0) return;

	positionChange(_y, _x - 1);
}

void NumberPuzzle::moveRight()
{
	if (_x >= 4) return;

	positionChange(_y, _x + 1);
}