#pragma once
#include "Singleton.h"


//로그 출력 타입
#define LOG_NONE		0x00000000
#define LOG_CONSOLE		0x00000001
#define LOG_FILE		0x00000002
#define LOG_WINDOW		0x00000004
#define LOG_ALL			( LOG_CONSOLE | LOG_FILE | LOG_WINDOW )

#define LOG_WINDOW_TITLE	"Log..."		//로그 창 윈도우 타이틀
#define LOG_WINDOW_CX		250				//로그 창 윈도우 가로 크기
#define LOG_WINDOW_CY		600				//로그 창 윈도우 세로 크기


//
// 로그 매니져
//

class LogMgr : public Singleton < LogMgr >
{
private:
	unsigned int		m_nTarget;
	char				m_szFileName[MAX_PATH];		//로그 내용 저장할 파일 이름
	HWND				m_hWndLog;					//로그창 윈도우 핸들
	HWND				m_hWndListBox;				//로그창 리스트 박스

protected:
	LogMgr(void);

public:
	~LogMgr(void);


public:

	//BaseSingleton 친구 등록
	friend LogMgr* Singleton<LogMgr>::GetInstance(void);

	//로그창 설정
	HRESULT			Setup(unsigned int nTarget, HWND hParentWnd, const char* pszFileName = NULL);

	//로그 찍기
	VOID			AddLog(char* strmsg, ...);

	//파일로 로그 저장
	VOID			LogFile(char* strmsg, ...);

	//해재
	VOID			Release(VOID) override;


private:
	//로그 윈도우생성
	VOID			CreateLogWindow(HWND hWnd);

	//로그 윈도우 메세지 프로시져
	static LRESULT CALLBACK WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
#define LOG_MGR LogMgr::GetInstance()

#define LOG_V(vector) LOG_MGR->AddLog("%.2f, %.2f, %.2f", vector.x, vector.y, vector.z)
#define LOG(text) LOG_MGR->AddLog(text)
#define LOG_F(f) LOG_MGR->AddLog("%f", f)
#define LOG_I(i) LOG_MGR->AddLog("%d", i)