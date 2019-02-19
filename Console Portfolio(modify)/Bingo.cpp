#include "Bingo.h"
#include <iostream>
#include <Windows.h>

using namespace std;

Bingo::Bingo()
{
}


Bingo::~Bingo()
{
}

void Bingo::startGame()
{
	int bingoNum[25];
	//int view[5][5];
	int bingo = 0;
	int round = 1;

	int hCheck, vCheck, slashCheck, bslashCheck;

	int input;
	bool isExist = false;

	int dest, sour, temp;

	for (int i = 0; i < 25; i++)
	{
		bingoNum[i] = i + 1;
	}

	for (int i = 0; i < 777; i++)
	{
		dest = rand() % 25;
		sour = rand() % 25;

		temp = bingoNum[dest];
		bingoNum[dest] = bingoNum[sour];
		bingoNum[sour] = temp;
	}

	//게임 시작
	while (true)
	{
		system("cls");

		//판 출력
		cout << "====================" << endl;
		cout << round << " round\t빙고 : " << bingo << endl << endl;

		for (int i = 0; i < 25; i++)
		{
			cout << "\t";
			if (bingoNum[i] == -1) cout << "#";
			else cout << bingoNum[i];
			if (i % 5 == 4) cout << endl << endl;
		}

		while (true)
		{
			input = -1;
			isExist = false;

			cout << "번호를 선택하시오." << endl;
			cout << "> ";
			cin >> input;
			cin.clear();

			if (input < 1 || input > 25)
			{
				cout << "다시 입력하시오." << endl;
				continue;
			}

			for (int i = 0; i < 25; i++)
			{
				if (bingoNum[i] == input)
				{
					bingoNum[i] = -1;
					isExist = true;
				}
			}

			if (isExist) break;
			else cout << "이미 선택한 번호입니다. 다시 선택하시오." << endl;
		}

		bingo = 0;

		//빙고 갯수
		slashCheck = bslashCheck = 0;
		for (int i = 0; i < 5; i++)
		{
			hCheck = vCheck = 0;
			for (int j = 0; j < 5; j++){
				if (bingoNum[i * 5 + j] == -1) hCheck++;//가로줄을 체크함. 1번째 루프면 맨위 한줄 체크
				if (bingoNum[j * 5 + i] == -1) vCheck++;//세로줄을 체크함. 1번째 루프면 맨 왼쪽 한줄 체크
			}

			if (hCheck == 5) bingo++;
			if (vCheck == 5) bingo++;

			if (bingoNum[i * 5 + i] == -1) slashCheck++;
			if (bingoNum[i * (5 - 1)] == -1) bslashCheck++;
		}

		if (slashCheck == 5) bingo++;
		if (bslashCheck == 5) bingo++;

		//승리조건
		if (bingo >= 5)
		{
			cout << "빙고 5개를 달성하였으므로 게임에서 이겼습니다!" << endl;
			break;
		}

		round++;
	}
}