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
	//변수들
	int comNumbers[3];
	int input;
	int inputNumbers[3];

	int strike;
	int ball;

	int round = 10;


	printf("========== 숫자 야구 게임 ==========\n");
	printf("숫자를 선택하는 중입니다..");

	//랜덤 숫자 초기화 - 옆에분꺼 보고 따라함..
	while (true){
		comNumbers[0] = rand() % 9 + 1;
		comNumbers[1] = rand() % 10;
		comNumbers[2] = rand() % 10;

		//자릿수마다 숫자가 같지 않고, //첫번째가 0이 아니면 빠져나감.(랜덤 숫자 범위로 지정.)// 아니면 다시 숫자 초기화.
		if (comNumbers[0] != comNumbers[1] && comNumbers[0] != comNumbers[2] && comNumbers[1] != comNumbers[2])// && comNumbers[0] != 0
		{
			break;
		}
		printf(".");
	}

	printf("\n숫자 선택이 완료되었습니다. 게임이 시작됩니다.\n");

	//라운드 시작
	while (true)
	{
		//변수들 초기화.
		input = 0;
		strike = 0;
		ball = 0;

		//숫자 입력.
		printf("round %d\n", round);
		printf("> ");

		scanf_s("%d", &input);

		//각 자릿수로 나눠 배열에 입력.
		inputNumbers[0] = input / 100; //100의 자리수
		inputNumbers[1] = (input % 100) / 10; //10의 자시루
		inputNumbers[2] = input % 10; //1의 자리수

		//printf("입력한 수 %d : %d %d %d\n", input, inputNumbers[0], inputNumbers[1], inputNumbers[2]);

		//입력값이 최소값, 최대값을 넘어가거나 중복된 숫자의 자릿수가 나오면 다시 입력.
		if ((input < 102 || input > 999) || inputNumbers[0] == inputNumbers[1] || inputNumbers[1] == inputNumbers[2] || inputNumbers[0] == inputNumbers[2])
		{
			printf("올바르게 입력하시오.\n");
			continue;
		}

		//숫자 비교
		//랜덤 숫자랑 입력 숫자를 각 자릿수를 모두 비교하며, 같은 숫자일때 같은 자릿수이면 스트라잌, 아니면 볼.
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

		//비교 출력
		printf("%dStrike, %dBall\n", strike, ball);

		//승리
		if (strike == 3)
		{
			printf("게임에서 이겼습니다! 축하드립니다~\n");
			break;
		}

		//라운드 감소
		round--;
		if (round < 1)
		{
			printf("게임에서 졌습니다. 정답은 %d%d%d였습니다.\n", comNumbers[0], comNumbers[1], comNumbers[2]);
			break;
		}
	}
}
