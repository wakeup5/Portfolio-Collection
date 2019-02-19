#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
//#define length(arr) (sizeof(arr) / sizeof(arr[0]))

/**
	@enum DISPLAY_COLOR - Display::setcolor 함수에서 사용하는 상수.
*/
enum DISPLAY_COLOR
{
	DISPLAY_COLOR_BLACK			= 0,	/**< 검은색 */
	DISPLAY_COLOR_DARKBLUE		= 1,	/**< 어두운 파랑색 */
	DISPLAY_COLOR_DARKGREEN		= 2,	/**< 어두운 초록색 */
	DISPLAY_COLOR_DARKSKY		= 3,	/**< 어두운 하늘색 */
	DISPLAY_COLOR_DARKRED		= 4,	/**< 어두운 빨강색 */
	DISPLAY_COLOR_DARKPURPLE	= 5,	/**< 어두운 보라색 */
	DISPLAY_COLOR_DARKYELLOW	= 6,	/**< 어두운 노랑색 */
	DISPLAY_COLOR_DARKWHITE		= 7,	/**< 어두운 흰색 */
	DISPLAY_COLOR_GRAY			= 8,	/**< 회색 */
	DISPLAY_COLOR_BLUE			= 9,	/**< 파랑색 */
	DISPLAY_COLOR_GREEN			= 10,	/**< 초록색 */
	DISPLAY_COLOR_SKY			= 11,	/**< 하늘색 */
	DISPLAY_COLOR_RED			= 12,	/**< 빨강색 */
	DISPLAY_COLOR_PURPLE		= 13,	/**< 보라색 */
	DISPLAY_COLOR_YELLOW		= 14,	/**< 노랑색 */
	DISPLAY_COLOR_WHITE			= 15,	/**< 흰색 */
};

/**
	@class	Display
	@brief	콘솔 화면의 출력 부분을 나누는 역할. 이미지, 다이알로그(대사), 입력 부분으로 나뉜다.
*/
class Display
{
private:
	Display(){}
	~Display(){}

public:
	static void gotoY(int);			///< 출력할 줄 라인을 선택
	static void gotoXY(int, int);	///< 출력할 위치를 선택
	static void setColor(int = DISPLAY_COLOR_WHITE, int = DISPLAY_COLOR_BLACK); ///< 글자의 색과 배경색을 지정

	static void clear(int = 0);		///< 커서로부터 입력된 줄 라인만큼 화면을 지움

	static void image();			///< 이미지 출력 부분에 커서를 둠
	static void image(char*);		///< 이미지 출력 부분에 커서를 두고 입력된 아스키 아트를 출력
	static void imageClear();		///< 이미지 출력 부분을 지우고 커서를 둠
	static void dialog();			///< 대사, 선택지 출력 부분에 커서를 둠
	static void dialogClear();		///< 대사, 선택지 출력 부분을 지우고 커서를 둠
	static void input();			///< 입력 부분에 커서를 둠
	static void inputClear();		///< 입력 부분을 지우고 커서를 둠

	static int choise(char*, char*[], int, char* = " > "); ///< 선택지를 dialog 부분에 만들어 줌, 화살표 상하로 선택
	static void gotoXYImage(char image[], int, int, char* = "\n"); ///< 해당 위치에 그림을 그림
};

