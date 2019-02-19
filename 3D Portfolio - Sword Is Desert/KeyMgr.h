#pragma once

#include "Singleton.h"

#define MAX_KEY_NUM		256

class KeyMgr : public Singleton<KeyMgr>
{
private:
	bool			m_bDownCheck[MAX_KEY_NUM];
	bool			m_bDownOnceCheck[MAX_KEY_NUM];
	bool			m_bUpCheck[MAX_KEY_NUM];

private:
	KeyMgr(void);

public:
	friend KeyMgr* Singleton<KeyMgr>::GetInstance();
	~KeyMgr(void);

	HRESULT Setup(void);
	void Release(void);
	void Update();

	bool IsStayDown(int nVKey);
	bool IsOnceDown(int nVKey);
	bool IsOnceUp(int nVKey);
};

#define KEY_MGR KeyMgr::GetInstance()