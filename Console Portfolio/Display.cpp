#include "Display.h"

/**
	@param	int y 줄 라인
	@brief	출력할 줄 라인을 선택
*/
void Display::gotoY(int y)
{
	COORD pos = { 0, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

/**
	@param	int x 줄의 좌우 위치
	@param	int y 줄 라인
	@brief	출력할 위치를 선택
*/
void Display::gotoXY(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/**
	@param	int DISPLAY_COLOR color 글자색
	@param	int DISPLAY_COLOR bgcolor 글자 배경
	@brief	글자의 색과 배경색을 지정
*/
void Display::setColor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

/**
	@param	int y 라인 수
	@brief	커서로부터 입력된 줄 라인만큼 화면을 지움
*/
void Display::clear(int y)
{
	for (int i = 0; i < y; i++)
	{
		//     "|______10|_______20|_______30|_______40|_______50|_______60|_______70|_______80|_______90|______100|______110|______120|__125|\n"	
		printf("                                                                                                                              \n");
	}
}

/**
	@brief	이미지 출력 부분에 커서를 둠
*/
void Display::image()
{
	gotoY(0);
}

/**
	@param	char* image 문자열 형태의 아스키 아트
	@brief	이미지 출력 부분에 커서를 두고 입력된 아스키 아트를 출력
*/
void Display::image(char* image)
{
	gotoY(0);
	printf("%s", image);
}

/**
	@brief	이미지 출력 부분을 지우고 커서를 둠
*/
void Display::imageClear()
{
	image();
	clear(55);
	image();
}

/**
	@brief	대사, 선택지 출력 부분에 커서를 둠
*/
void Display::dialog()
{
	gotoY(54);
}

/**
	@brief	대사, 선택지 출력 부분을 지우고 커서를 둠
*/
void Display::dialogClear()
{
	dialog();
	clear(11); 
	dialog();
}

/**
	@brief	입력 부분에 커서를 둠
*/
void Display::input()
{
	gotoY(64);
}

/**
	@brief	입력 부분을 지우고 커서를 둠
*/
void Display::inputClear()
{
	input();
	clear(1);
	input();
}

/**
	@param	char* question 선택지의 질문
	@param	char*[] dist 선택지
	@param	int length 선택지의 갯수
	@param	char* symbol = " > " 선택지 앞에 나타나는 선택 화살표
	@brief	선택지를 dialog 부분에 만들어 줌, 화살표 상하로 선택
*/
int Display::choise(char* question, char* dist[], int length, char* symbol)
{
	int selectNum = 0;
	int input;
	while (true)
	{
		input = 0;
		//printf("|%d", _getch());//코드 확인용
		//continue;
		
		dialogClear();
		printf("\n %s\n\n", question);
		for (int i = 0; i < length; i++)
		{
			if (selectNum == i)
			{
				printf(" %s", symbol);
			}
			else
			{
				printf(" ");
				for (int j = 0; j < strlen(symbol); j++)
				{
					printf("%c", ' ');
				}
			}

			printf("%s\n", dist[i]);
		}

		input = _getch();
		if (input == 224)
		{
			input = _getch();
			if (input == 72)
			{
				selectNum--;
				if (selectNum < 0) selectNum = length - 1;
			}
			else if (input == 80)
			{
				selectNum++;
				if (selectNum >= length) selectNum = 0;
			}
		}
		else if (input == 13)
		{
			break;
		}

	}

	return selectNum;
}

/**
	@param	char* 아스키 아트로 된 이미지
	@param	int x x 위치
	@param	int y y 위치
	@param	char* symbol = "\n" 아스키 아트 내용에서 해당 문자를 만나면 줄바꿈을 한다.
	@brief	해당 위치에 그림을 그림
*/
void Display::gotoXYImage(char* image, int x, int y, char* symbol)
{
	char i[125*60];
	strcpy_s(i, image);

	char* context = NULL;
	char* token = strtok_s(i, symbol, &context);
	int ty = y;
	gotoXY(x, ty++);
	printf("%s", token);

	while (token = strtok_s(NULL, symbol, &context))
	{
		gotoXY(x, ty++);
		printf("%s", token);
	}
}