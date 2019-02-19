#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>
//#define length(arr) (sizeof(arr) / sizeof(arr[0]))

using namespace std;

/**
@enum DISPLAY_COLOR - Display::setcolor �Լ����� ����ϴ� ���.
*/
enum DISPLAY_COLOR
{
	DISPLAY_COLOR_BLACK = 0,	/**< ������ */
	DISPLAY_COLOR_DARKBLUE = 1,	/**< ��ο� �Ķ��� */
	DISPLAY_COLOR_DARKGREEN = 2,	/**< ��ο� �ʷϻ� */
	DISPLAY_COLOR_DARKSKY = 3,	/**< ��ο� �ϴû� */
	DISPLAY_COLOR_DARKRED = 4,	/**< ��ο� ������ */
	DISPLAY_COLOR_DARKPURPLE = 5,	/**< ��ο� ����� */
	DISPLAY_COLOR_DARKYELLOW = 6,	/**< ��ο� ����� */
	DISPLAY_COLOR_DARKWHITE = 7,	/**< ��ο� ��� */
	DISPLAY_COLOR_GRAY = 8,	/**< ȸ�� */
	DISPLAY_COLOR_BLUE = 9,	/**< �Ķ��� */
	DISPLAY_COLOR_GREEN = 10,	/**< �ʷϻ� */
	DISPLAY_COLOR_SKY = 11,	/**< �ϴû� */
	DISPLAY_COLOR_RED = 12,	/**< ������ */
	DISPLAY_COLOR_PURPLE = 13,	/**< ����� */
	DISPLAY_COLOR_YELLOW = 14,	/**< ����� */
	DISPLAY_COLOR_WHITE = 15,	/**< ��� */
};

/**
@class	Display
@brief	�ܼ� ȭ���� ��� �κ��� ������ ����. �̹���, ���̾˷α�(���), �Է� �κ����� ������.
*/
class Display
{
private:
	int _displayWidth, _displayHeight;
	string _displayTitle;

	int _cursorX, _cursorY;

public:
	Display();
	Display(int, int, string = "");
	~Display();

	void setDisplaySize(int, int);
	void setDisplayTitle(string);
	void initalize();

	void gotoY(int);			///< ����� �� ������ ����
	void gotoXY(int, int);	///< ����� ��ġ�� ����
	void setColor(int = DISPLAY_COLOR_WHITE, int = DISPLAY_COLOR_BLACK); ///< ������ ���� ������ ����

	void clear(int);
	void clear(int, int);		///< Ŀ���κ��� �Էµ� �� ���θ�ŭ ȭ���� ����


};

