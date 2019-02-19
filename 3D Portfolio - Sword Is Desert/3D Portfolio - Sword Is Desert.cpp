// 3D Portfolio - Sword Is Desert.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "3D Portfolio - Sword Is Desert.h"
#include "GameMain.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;       
HWND hWnd;										// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

GameMain gm;

HCURSOR customCursor;
WNDCLASSEX wcex;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MY3DPORTFOLIOSWORDISDESERT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DPORTFOLIOSWORDISDESERT));

	//메시지 구조체
	MSG Message;
	ZeroMemory(&Message, sizeof(MSG));

	//게임 초기화 성공했다면.....
	if (SUCCEEDED(gm.Setup()))		//S_OK 를 리턴했다면...
	{

		//위에서 만든 윈도우를 화면에 띄운다.
		ShowWindow(hWnd, nCmdShow);

		//
		// 메시지 루프
		//
		/*
		while( GetMessage( &Message, NULL, NULL, NULL ) )
		{
		TranslateMessage( &Message ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
		DispatchMessage( &Message );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
		}
		*/


		//메시큐에서 얻어온 메시지가 종료메시지가 아니면 계속 루프
		while (Message.message != WM_QUIT) {

			//메시지큐의 메시지를본다 
			//하지만 마지막의 PM_REMOVE 옵션으로인해 본메시지는
			//지운다.
			//만약 Message 가 없다면 false 를 리턴한다.
			if (PeekMessage(&Message, NULL, NULL, NULL, PM_REMOVE)) {
				TranslateMessage(&Message); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
				DispatchMessage(&Message);  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
			}

			//업데이트 
			gm.Update();

			//드로우
			gm.Render();

		}

	}

	//해제
	gm.Release();

	//등록된 윈도우 클레스 해제 ( 해줘도 그만 안해줘도 그만.. )
	UnregisterClass(WIN_CLASS, hInstance);

    return (int)Message.wParam;
}


void setWindowPositionAndSize(int x, int y, int width, int height)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(hWnd, NULL, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
}


//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
   // WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DPORTFOLIOSWORDISDESERT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY3DPORTFOLIOSWORDISDESERT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	customCursor = LoadCursorFromFile("stand_mouse.cur");
	wcex.hCursor = customCursor;
    return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND h;
   /*
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   */
   h = CreateWindow(
	   szWindowClass,				//생성되는 윈도우의 클래스이름
	   szTitle,				//윈도우 타이틀바에 출력되는 이름
	   WIN_STYLE,				//윈도우 스타일
	   WINPOS_X,				//윈도우 시작 위치 X 
	   WINPOS_Y,				//윈도우 시작 위치 Y
	   WINSIZE_X,				//윈도우 가로 크기 ( 작업영역의 크기가 아님 )
	   WINSIZE_Y,				//윈도우 세로 크기 ( 작업영역의 크기가 아님 )
	   GetDesktopWindow(),		//부모 윈도우 핸들 ( 프로그램에서 최상위 윈도우면 NULL 또는 GetDesktopWindow() )
	   NULL,					//메뉴 ID ( 자신의 컨트롤 객체의 윈도우인경우 컨트롤 ID 가 된	
	   hInstance,				//이 윈도우가 물릴 프로그램 인스턴스 핸들
	   NULL					//추가 정보 NULL ( 신경끄자 )
	   );

   if (!h)
   {
      return FALSE;
   }

   ShowWindow(h, nCmdShow);
   UpdateWindow(h);

   hWnd = h;
   setWindowPositionAndSize(WINPOS_X, WINPOS_Y, WINSIZE_X, WINSIZE_Y);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(h, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
		return gm.WndProc(h, message, wParam, lParam);
    }
    return 0;
}