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

	//���� ����
	while (true)
	{
		system("cls");

		//�� ���
		cout << "====================" << endl;
		cout << round << " round\t���� : " << bingo << endl << endl;

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

			cout << "��ȣ�� �����Ͻÿ�." << endl;
			cout << "> ";
			cin >> input;
			cin.clear();

			if (input < 1 || input > 25)
			{
				cout << "�ٽ� �Է��Ͻÿ�." << endl;
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
			else cout << "�̹� ������ ��ȣ�Դϴ�. �ٽ� �����Ͻÿ�." << endl;
		}

		bingo = 0;

		//���� ����
		slashCheck = bslashCheck = 0;
		for (int i = 0; i < 5; i++)
		{
			hCheck = vCheck = 0;
			for (int j = 0; j < 5; j++){
				if (bingoNum[i * 5 + j] == -1) hCheck++;//�������� üũ��. 1��° ������ ���� ���� üũ
				if (bingoNum[j * 5 + i] == -1) vCheck++;//�������� üũ��. 1��° ������ �� ���� ���� üũ
			}

			if (hCheck == 5) bingo++;
			if (vCheck == 5) bingo++;

			if (bingoNum[i * 5 + i] == -1) slashCheck++;
			if (bingoNum[i * (5 - 1)] == -1) bslashCheck++;
		}

		if (slashCheck == 5) bingo++;
		if (bslashCheck == 5) bingo++;

		//�¸�����
		if (bingo >= 5)
		{
			cout << "���� 5���� �޼��Ͽ����Ƿ� ���ӿ��� �̰���ϴ�!" << endl;
			break;
		}

		round++;
	}
}