#include "Display.h"

/**
	@param	int y �� ����
	@brief	����� �� ������ ����
*/
void Display::gotoY(int y)
{
	COORD pos = { 0, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

/**
	@param	int x ���� �¿� ��ġ
	@param	int y �� ����
	@brief	����� ��ġ�� ����
*/
void Display::gotoXY(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/**
	@param	int DISPLAY_COLOR color ���ڻ�
	@param	int DISPLAY_COLOR bgcolor ���� ���
	@brief	������ ���� ������ ����
*/
void Display::setColor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

/**
	@param	int y ���� ��
	@brief	Ŀ���κ��� �Էµ� �� ���θ�ŭ ȭ���� ����
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
	@brief	�̹��� ��� �κп� Ŀ���� ��
*/
void Display::image()
{
	gotoY(0);
}

/**
	@param	char* image ���ڿ� ������ �ƽ�Ű ��Ʈ
	@brief	�̹��� ��� �κп� Ŀ���� �ΰ� �Էµ� �ƽ�Ű ��Ʈ�� ���
*/
void Display::image(char* image)
{
	gotoY(0);
	printf("%s", image);
}

/**
	@brief	�̹��� ��� �κ��� ����� Ŀ���� ��
*/
void Display::imageClear()
{
	image();
	clear(55);
	image();
}

/**
	@brief	���, ������ ��� �κп� Ŀ���� ��
*/
void Display::dialog()
{
	gotoY(54);
}

/**
	@brief	���, ������ ��� �κ��� ����� Ŀ���� ��
*/
void Display::dialogClear()
{
	dialog();
	clear(11); 
	dialog();
}

/**
	@brief	�Է� �κп� Ŀ���� ��
*/
void Display::input()
{
	gotoY(64);
}

/**
	@brief	�Է� �κ��� ����� Ŀ���� ��
*/
void Display::inputClear()
{
	input();
	clear(1);
	input();
}

/**
	@param	char* question �������� ����
	@param	char*[] dist ������
	@param	int length �������� ����
	@param	char* symbol = " > " ������ �տ� ��Ÿ���� ���� ȭ��ǥ
	@brief	�������� dialog �κп� ����� ��, ȭ��ǥ ���Ϸ� ����
*/
int Display::choise(char* question, char* dist[], int length, char* symbol)
{
	int selectNum = 0;
	int input;
	while (true)
	{
		input = 0;
		//printf("|%d", _getch());//�ڵ� Ȯ�ο�
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
	@param	char* �ƽ�Ű ��Ʈ�� �� �̹���
	@param	int x x ��ġ
	@param	int y y ��ġ
	@param	char* symbol = "\n" �ƽ�Ű ��Ʈ ���뿡�� �ش� ���ڸ� ������ �ٹٲ��� �Ѵ�.
	@brief	�ش� ��ġ�� �׸��� �׸�
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