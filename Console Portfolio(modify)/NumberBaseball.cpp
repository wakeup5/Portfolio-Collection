#include "NumberBaseball.h"
#include <iostream>

NumberBaseball::NumberBaseball()
{
}


NumberBaseball::~NumberBaseball()
{
}

void NumberBaseball::startGame()
{
	//������
	int comNumbers[3];
	int input;
	int inputNumbers[3];

	int strike;
	int ball;

	int round = 10;


	printf("========== ���� �߱� ���� ==========\n");
	printf("���ڸ� �����ϴ� ���Դϴ�..");

	//���� ���� �ʱ�ȭ - �����в� ���� ������..
	while (true){
		comNumbers[0] = rand() % 9 + 1;
		comNumbers[1] = rand() % 10;
		comNumbers[2] = rand() % 10;

		//�ڸ������� ���ڰ� ���� �ʰ�, //ù��°�� 0�� �ƴϸ� ��������.(���� ���� ������ ����.)// �ƴϸ� �ٽ� ���� �ʱ�ȭ.
		if (comNumbers[0] != comNumbers[1] && comNumbers[0] != comNumbers[2] && comNumbers[1] != comNumbers[2])// && comNumbers[0] != 0
		{
			break;
		}
		printf(".");
	}

	printf("\n���� ������ �Ϸ�Ǿ����ϴ�. ������ ���۵˴ϴ�.\n");

	//���� ����
	while (true)
	{
		//������ �ʱ�ȭ.
		input = 0;
		strike = 0;
		ball = 0;

		//���� �Է�.
		printf("round %d\n", round);
		printf("> ");

		scanf_s("%d", &input);

		//�� �ڸ����� ���� �迭�� �Է�.
		inputNumbers[0] = input / 100; //100�� �ڸ���
		inputNumbers[1] = (input % 100) / 10; //10�� �ڽ÷�
		inputNumbers[2] = input % 10; //1�� �ڸ���

		//printf("�Է��� �� %d : %d %d %d\n", input, inputNumbers[0], inputNumbers[1], inputNumbers[2]);

		//�Է°��� �ּҰ�, �ִ밪�� �Ѿ�ų� �ߺ��� ������ �ڸ����� ������ �ٽ� �Է�.
		if ((input < 102 || input > 999) || inputNumbers[0] == inputNumbers[1] || inputNumbers[1] == inputNumbers[2] || inputNumbers[0] == inputNumbers[2])
		{
			printf("�ùٸ��� �Է��Ͻÿ�.\n");
			continue;
		}

		//���� ��
		//���� ���ڶ� �Է� ���ڸ� �� �ڸ����� ��� ���ϸ�, ���� �����϶� ���� �ڸ����̸� ��Ʈ���, �ƴϸ� ��.
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (comNumbers[i] == inputNumbers[j])
				{
					if (i == j){
						strike++;
					}
					else
					{
						ball++;
					}
				}
			}
		}

		//�� ���
		printf("%dStrike, %dBall\n", strike, ball);

		//�¸�
		if (strike == 3)
		{
			printf("���ӿ��� �̰���ϴ�! ���ϵ帳�ϴ�~\n");
			break;
		}

		//���� ����
		round--;
		if (round < 1)
		{
			printf("���ӿ��� �����ϴ�. ������ %d%d%d�����ϴ�.\n", comNumbers[0], comNumbers[1], comNumbers[2]);
			break;
		}
	}
}
