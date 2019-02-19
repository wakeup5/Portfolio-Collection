#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>
//#define length(arr) (sizeof(arr) / sizeof(arr[0]))

using namespace std;

/**
@enum DISPLAY_COLOR - Display::setcolor 함수에서 사용하는 상수.
*/
enum DISPLAY_COLOR
{
	DISPLAY_COLOR_BLACK = 0,	/**< 검은색 */
	DISPLAY_COLOR_DARKBLUE = 1,	/**< 어두운 파랑색 */
	DISPLAY_COLOR_DARKGREEN = 2,	/**< 어두운 초록색 */
	DISPLAY_COLOR_DARKSKY = 3,	/**< 어두운 하늘색 */
	DISPLAY_COLOR_DARKRED = 4,	/**< 어두운 빨강색 */
	DISPLAY_COLOR_DARKPURPLE = 5,	/**< 어두운 보라색 */
	DISPLAY_COLOR_DARKYELLOW = 6,	/**< 어두운 노랑색 */
	DISPLAY_COLOR_DARKWHITE = 7,	/**< 어두운 흰색 */
	DISPLAY_COLOR_GRAY = 8,	/**< 회색 */
	DISPLAY_COLOR_BLUE = 9,	/**< 파랑색 */
	DISPLAY_COLOR_GREEN = 10,	/**< 초록색 */
	DISPLAY_COLOR_SKY = 11,	/**< 하늘색 */
	DISPLAY_COLOR_RED = 12,	/**< 빨강색 */
	DISPLAY_COLOR_PURPLE = 13,	/**< 보라색 */
	DISPLAY_COLOR_YELLOW = 14,	/**< 노랑색 */
	DISPLAY_COLOR_WHITE = 15,	/**< 흰색 */
};

/**
@class	Display
@brief	콘솔 화면의 출력 부분을 나누는 역할. 이미지, 다이알로그(대사), 입력 부분으로 나뉜다.
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

	void gotoY(int);			///< 출력할 줄 라인을 선택
	void gotoXY(int, int);	///< 출력할 위치를 선택
	void setColor(int = DISPLAY_COLOR_WHITE, int = DISPLAY_COLOR_BLACK); ///< 글자의 색과 배경색을 지정

	void clear(int);
	void clear(int, int);		///< 커서로부터 입력된 줄 라인만큼 화면을 지움


};

