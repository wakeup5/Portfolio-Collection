#include "StdAfx.h"
#include "KeyMgr.h"

KeyMgr::KeyMgr(void)
{
}

KeyMgr::~KeyMgr(void)
{
}

HRESULT KeyMgr::Setup(void)
{
	for (int i = 0; i < MAX_KEY_NUM; i++) {
		m_bDownCheck[i] = false;
		m_bUpCheck[i] = false;
		m_bDownOnceCheck[i] = false;
	}

	return S_OK;
}
void KeyMgr::Release(void)
{

}

void KeyMgr::Update()
{
	for (int i = 0; i < MAX_KEY_NUM; i++) {
		//m_bDownOnceCheck[i] = false;
		//m_bUpCheck[i] = false;
	}
}

bool KeyMgr::IsStayDown(int nVKey)
{
	return (GetAsyncKeyState(nVKey) & 0x8000);
}

bool KeyMgr::IsOnceDown(int nVKey)
{
	if (GetAsyncKeyState(nVKey) & 0x8000) 
	{
		if (m_bDownCheck[nVKey] == false) 
		{
			m_bDownCheck[nVKey] = true;
			return true;
		}

	}
	else 
	{
		m_bDownCheck[nVKey] = false;
	}

	return false;
}

bool KeyMgr::IsOnceUp(int nVKey)
{
	if (GetAsyncKeyState(nVKey) & 0x8000) {
		if (m_bUpCheck[nVKey] == false) {
			m_bUpCheck[nVKey] = true;
		}
	}

	else {
		if (m_bUpCheck[nVKey]) {
			m_bUpCheck[nVKey] = false;
			return true;
		}

	}

	return false;
}