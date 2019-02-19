#pragma once
//#include "Display.h"
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>

using namespace std;

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

class GameDisplay
{
private:
	//static Display* _display;

	GameDisplay();
	~GameDisplay();
public:
	static void setDisplayInfo(string = "Watcher the console", int = 128, int = 65);

	static void gotoY(int);
	static void gotoXY(int, int);	///< ����� ��ġ�� ����
	static void setColor(int = DISPLAY_COLOR_WHITE, int = DISPLAY_COLOR_BLACK); ///< ������ ���� ������ ����

	static void clear(int);
	static void clear(int, int);		///< Ŀ���κ��� �Էµ� �� ���θ�ŭ ȭ���� ����

	static void image();			///< �̹��� ��� �κп� Ŀ���� ��
	static void image(string);		///< �̹��� ��� �κп� Ŀ���� �ΰ� �Էµ� �ƽ�Ű ��Ʈ�� ���
	static void imageClear();		///< �̹��� ��� �κ��� ����� Ŀ���� ��
	static void dialog();			///< ���, ������ ��� �κп� Ŀ���� ��
	static void dialogClear();		///< ���, ������ ��� �κ��� ����� Ŀ���� ��
	static void input();			///< �Է� �κп� Ŀ���� ��
	static void inputClear();		///< �Է� �κ��� ����� Ŀ���� ��

	static int choise(string, string[], int, string = " > "); ///< �������� dialog �κп� ����� ��, ȭ��ǥ ���Ϸ� ����
	static void draw(string image, int, int, string = "\n"); ///< �ش� ��ġ�� �׸��� �׸�
};

