#include "GameDisplay.h"


GameDisplay::GameDisplay()
{
	//_display = new Display(128, 65, "Watcher the console");
}


GameDisplay::~GameDisplay()
{
	//delete _display;
}

void GameDisplay::setDisplayInfo(string title, int width, int height)
{
	system(("mode CON COLS=" + to_string(width) + "LINES=" + to_string(height)).c_str());
	system(("title " + title).c_str());
}

/**
@param	int y 줄 라인
@brief	출력할 줄 라인을 선택
*/
void GameDisplay::gotoY(int y)
{
	gotoXY(0, y);
}

/**
@param	int x 줄의 좌우 위치
@param	int y 줄 라인
@brief	출력할 위치를 선택
*/
void GameDisplay::gotoXY(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });

	//_cursorX = x;
	//_cursorY = y;
}

/**
@param	int DISPLAY_COLOR color 글자색
@param	int DISPLAY_COLOR bgcolor 글자 배경
@brief	글자의 색과 배경색을 지정
*/
void GameDisplay::setColor(int color, int bgcolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | color & 0xf);
}

/**
@param	int y 라인 수
@brief	커서로부터 입력된 줄 라인만큼 화면을 지움. 라인 전부를 지움
*/
void GameDisplay::clear(int y)
{
	for (int i = 0; i < y; i++)
	{
		//     "|______10|_______20|_______30|_______40|_______50|_______60|_______70|_______80|_______90|______100|______110|______120|__125|\n"	
		//printf("                                                                                                                              \n");
		string empty = "";
		for (int j = 0; j < 125; j++)
		{
			empty += " ";
		}
		printf(empty.c_str());
	}
}

/**
@param	int x 폭
@param	int y 라인 수
@brief	커서로부터 입력된 폭과 라인만큼 화면을 지움
*/
void GameDisplay::clear(int x, int y)
{
	for (int i = 0; i < y; i++)
	{
		//     "|______10|_______20|_______30|_______40|_______50|_______60|_______70|_______80|_______90|______100|______110|______120|__125|\n"	
		//printf("                                                                                                                              \n");
		//gotoXY(_cursorX, _cursorY + i);
		for (int j = 0; j < x; j++)
		{
			printf(" ");
		}
	}
}

/**
@param	string question 선택지의 질문
@param	string[] dist 선택지
@param	int length 선택지의 갯수
@param	string symbol = " > " 선택지 앞에 나타나는 선택 화살표
@brief	선택지를 dialog 부분에 만들어 줌, 화살표 상하로 선택
*/
int GameDisplay::choise(string question, string dist[], int length, string symbol)
{
	//return 0;
	int selectNum = 0;
	int input;
	while (true)
	{
		input = 0;

		dialogClear();
		printf("\n %s\n\n", question.c_str());
		for (int i = 0; i < length; i++)
		{
			if (selectNum == i)
			{
				printf(" %s", symbol.c_str());
			}
			else
			{
				printf(" ");
				for (int j = 0; j < symbol.length(); j++)
				{
					printf("%c", ' ');
				}
			}

			printf("%s\n", dist[i].c_str());
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
		else if (input == 27)
		{
			return -1;
		}

	}

	return selectNum;
}

/**
@param	string 아스키 아트로 된 이미지
@param	int x x 위치
@param	int y y 위치
@param	string symbol = "\n" 아스키 아트 내용에서 해당 문자를 만나면 줄바꿈을 한다.
@brief	해당 위치에 그림을 그림
*/
void GameDisplay::draw(string image, int x, int y, string symbol)
{
	vector<string> split;

	size_t pos = 0;
	std::string token;
	while ((pos = image.find(symbol)) != std::string::npos) {
		token = image.substr(0, pos);
		split.push_back(token);
		image.erase(0, pos + symbol.length());
	}
	split.push_back(image);

	for (int i = 0; i < (int)split.size(); i++)
	{
		gotoXY(x, y+i);
		printf("%s", split[i].c_str());
	}
}


/**
@brief	이미지 출력 부분에 커서를 둠
		0 ~ 53 까지 54줄
*/
void GameDisplay::image()
{
	gotoY(0);
}

/**
@param	string image 문자열 형태의 아스키 아트
@brief	이미지 출력 부분에 커서를 두고 입력된 아스키 아트를 출력
*/
void GameDisplay::image(string image)
{
	gotoY(0);
	printf("%s", image.c_str());
}

/**
@brief	이미지 출력 부분을 지우고 커서를 둠
*/
void GameDisplay::imageClear()
{
	image();
	clear(54);
	image();
}

/**
@brief	대사, 선택지 출력 부분에 커서를 둠
		54 ~ 64 까지 10줄
*/
void GameDisplay::dialog()
{
	gotoY(54);
}

/**
@brief	대사, 선택지 출력 부분을 지우고 커서를 둠
*/
void GameDisplay::dialogClear()
{
	dialog();
	clear(10);
	dialog();
}

/**
@brief	입력 부분에 커서를 둠
		65 한줄
*/
void GameDisplay::input()
{
	gotoY(64);
}

/**
@brief	입력 부분을 지우고 커서를 둠
*/
void GameDisplay::inputClear()
{
	input();
	clear(1);
	input();
}

