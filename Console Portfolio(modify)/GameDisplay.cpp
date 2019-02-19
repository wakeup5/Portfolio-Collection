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
@param	int y �� ����
@brief	����� �� ������ ����
*/
void GameDisplay::gotoY(int y)
{
	gotoXY(0, y);
}

/**
@param	int x ���� �¿� ��ġ
@param	int y �� ����
@brief	����� ��ġ�� ����
*/
void GameDisplay::gotoXY(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });

	//_cursorX = x;
	//_cursorY = y;
}

/**
@param	int DISPLAY_COLOR color ���ڻ�
@param	int DISPLAY_COLOR bgcolor ���� ���
@brief	������ ���� ������ ����
*/
void GameDisplay::setColor(int color, int bgcolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | color & 0xf);
}

/**
@param	int y ���� ��
@brief	Ŀ���κ��� �Էµ� �� ���θ�ŭ ȭ���� ����. ���� ���θ� ����
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
@param	int x ��
@param	int y ���� ��
@brief	Ŀ���κ��� �Էµ� ���� ���θ�ŭ ȭ���� ����
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
@param	string question �������� ����
@param	string[] dist ������
@param	int length �������� ����
@param	string symbol = " > " ������ �տ� ��Ÿ���� ���� ȭ��ǥ
@brief	�������� dialog �κп� ����� ��, ȭ��ǥ ���Ϸ� ����
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
@param	string �ƽ�Ű ��Ʈ�� �� �̹���
@param	int x x ��ġ
@param	int y y ��ġ
@param	string symbol = "\n" �ƽ�Ű ��Ʈ ���뿡�� �ش� ���ڸ� ������ �ٹٲ��� �Ѵ�.
@brief	�ش� ��ġ�� �׸��� �׸�
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
@brief	�̹��� ��� �κп� Ŀ���� ��
		0 ~ 53 ���� 54��
*/
void GameDisplay::image()
{
	gotoY(0);
}

/**
@param	string image ���ڿ� ������ �ƽ�Ű ��Ʈ
@brief	�̹��� ��� �κп� Ŀ���� �ΰ� �Էµ� �ƽ�Ű ��Ʈ�� ���
*/
void GameDisplay::image(string image)
{
	gotoY(0);
	printf("%s", image.c_str());
}

/**
@brief	�̹��� ��� �κ��� ����� Ŀ���� ��
*/
void GameDisplay::imageClear()
{
	image();
	clear(54);
	image();
}

/**
@brief	���, ������ ��� �κп� Ŀ���� ��
		54 ~ 64 ���� 10��
*/
void GameDisplay::dialog()
{
	gotoY(54);
}

/**
@brief	���, ������ ��� �κ��� ����� Ŀ���� ��
*/
void GameDisplay::dialogClear()
{
	dialog();
	clear(10);
	dialog();
}

/**
@brief	�Է� �κп� Ŀ���� ��
		65 ����
*/
void GameDisplay::input()
{
	gotoY(64);
}

/**
@brief	�Է� �κ��� ����� Ŀ���� ��
*/
void GameDisplay::inputClear()
{
	input();
	clear(1);
	input();
}

