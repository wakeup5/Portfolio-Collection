#include "Display.h"

Display::Display()
{

}

Display::Display(int x, int y, string title)
{
	setDisplaySize(x, y);
	setDisplayTitle(title);
	initalize();
}

Display::~Display()
{

}

void Display::setDisplaySize(int x, int y)
{
	_displayWidth = x;
	_displayHeight = y;
}

void Display::setDisplayTitle(string title)
{
	_displayTitle = title;
}

void Display::initalize()
{
	system(("mode CON COLS=" + to_string(_displayWidth) + "LINES=" + to_string(_displayHeight)).c_str());
	system(("title " + _displayTitle).c_str());
}


/**
@param	int y �� ����
@brief	����� �� ������ ����
*/
void Display::gotoY(int y)
{
	gotoXY(0, y);
}

/**
@param	int x ���� �¿� ��ġ
@param	int y �� ����
@brief	����� ��ġ�� ����
*/
void Display::gotoXY(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });

	_cursorX = x;
	_cursorY = y;
}

/**
@param	int DISPLAY_COLOR color ���ڻ�
@param	int DISPLAY_COLOR bgcolor ���� ���
@brief	������ ���� ������ ����
*/
void Display::setColor(int color, int bgcolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | color & 0xf);
}

/**
@param	int y ���� ��
@brief	Ŀ���κ��� �Էµ� �� ���θ�ŭ ȭ���� ����. ���� ���θ� ����
*/
void Display::clear(int y)
{
	for (int i = 0; i < y; i++)
	{
		//     "|______10|_______20|_______30|_______40|_______50|_______60|_______70|_______80|_______90|______100|______110|______120|__125|\n"	
		//printf("                                                                                                                              \n");
		string empty = "";
		for (int j = 0; j < _displayWidth - 1; j++)
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
void Display::clear(int x, int y)
{
	for (int i = 0; i < y; i++)
	{
		//     "|______10|_______20|_______30|_______40|_______50|_______60|_______70|_______80|_______90|______100|______110|______120|__125|\n"	
		//printf("                                                                                                                              \n");
		gotoXY(_cursorX, _cursorY + i);
		for (int j = 0; j < x; j++)
		{
			printf(" ");
		}
	}
}


