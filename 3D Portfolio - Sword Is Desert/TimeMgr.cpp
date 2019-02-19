#include "StdAfx.h"
#include "TimeMgr.h"
#include <stdio.h>

//timeGetTime 을 이용하기위한 헤더와 라이브러리
#include <MMSystem.h>
#pragma comment( lib, "winmm.lib" )		//winmm.lib 추가

TimeMgr::TimeMgr(void)
{
}

TimeMgr::~TimeMgr(void)
{
}

HRESULT TimeMgr::Setup(void)
{
	//고성능 타이머 지원 여부
	//QueryPerformanceFrequency 함수로 초당 카운트 가능한 값을 얻는다.
	//고성능 타이머 지원 된다면 TRUE 안된다면 FALSE
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPreriodTime))
	{
		//고성능 타이머를 지원한다면...
		m_bHardWare = true;

		//1 카운트당 초가 얼마나 되느냐?
		m_TimeScaleSec = 1.0 / m_nPreriodTime;

		//초기화 시점의 시간을 LastTime 으로...
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
	}

	else
	{
		//고성능 타이머를 지원하지 않는다면...
		m_bHardWare = false;

		//1000 밀리세컨드 단위의 시간값을 얻는다.
		//timeGetTime 이 GetTickCount 보다 좀더 정밀하다.
		m_nLastTime = timeGetTime();

		//1 카운트당 초가 얼마나 되느냐?
		m_TimeScaleSec = 1.0 / 1000.0;

	}

	m_TotalSec = 0.0f;
	m_FrameCountSec = 0.0f;
	m_FramePerSec = 0;
	m_FrameCount = 0;

	return S_OK;
}

void TimeMgr::Release(void)
{
}

//매업데이트 물려준다.
void TimeMgr::UpdateTime(float frameLock)
{
	//현제 시간을 얻는다.
	if (m_bHardWare)
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurTime);
	else
		m_nCurTime = timeGetTime();

	//지난 시간과의 격차를 계산
	m_nDeltaTime = m_nCurTime - m_nLastTime;


	//한프레임 간격의 지난 초시간 계산
	m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;

	//프레임 락이 존해한다면...( 프레임 스키핑 )
	if (frameLock > 0.0f) {

		//고여있을 시간.
		float fLockTime = (1.0f / frameLock);

		while (m_FrameDeltaSec < fLockTime)
		{
			//현제 시간을 얻는다.
			if (m_bHardWare)
				QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurTime);
			else
				m_nCurTime = timeGetTime();

			//지난 시간과의 격를 계산
			m_nDeltaTime = m_nCurTime - m_nLastTime;

			//한프레임 간격의 지난 초시간 계산
			m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;
		}
	}

	//지난 시간 갱신
	m_nLastTime = m_nCurTime;

	//총지난 시간 누적
	m_TotalSec += m_FrameDeltaSec;

	//프레임 초 누적
	m_FrameCountSec += m_FrameDeltaSec;

	//1초가 지났다면....
	if (m_FrameCountSec >= 1.0) {
		m_FramePerSec = m_FrameCount;
		m_FrameCount = 0;
		m_FrameCountSec -= 1.0;
	}
	m_FrameCount++;
}

void TimeMgr::Update(float timeDelta)
{
	//타이머 업데이트
	for (auto i = _timers.begin(); i != _timers.end(); i++)
	{
		i->second->Update(timeDelta);
	}
}

//타임정보를 출력한다
void TimeMgr::DrawTimeInfo()
{
	char szTemp[128];
	wsprintf(szTemp, "FPS : %d", m_FramePerSec);
	DXFONT_MGR->PrintTextOutline(szTemp, 10, 100, 0xffffff00, 0xff000000);

	sprintf(szTemp, "FrameDeltaSec : %.10f", m_FrameDeltaSec);
	DXFONT_MGR->PrintTextOutline(szTemp, 10, 130, 0xffffff00, 0xff000000);

	sprintf(szTemp, "TotalDeltaSec : %.2f", m_TotalSec);
	DXFONT_MGR->PrintTextOutline(szTemp, 10, 160, 0xffffff00, 0xff000000);
}

Timer* TimeMgr::GetTimer(std::string name)
{
	auto iter = _timers.find(name);

	if (iter == _timers.end())
	{
		return NULL;
	}

	return iter->second;
}

Timer * TimeMgr::CreateTimer(std::string name)
{
	static UINT num = 0;

	if (name == "") name = "timer name - " + num++;

	Timer* result = GetTimer(name);

	if (result == NULL)
	{
		result = new Timer;
		_timers.insert(std::make_pair(name, result));
	}

	return result;
}
