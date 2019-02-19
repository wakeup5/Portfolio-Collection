#pragma once
#include "singleton.h"

#define CIRCLEGIZMO_SEGMENTS 36

class GizmoManager : public Singleton < GizmoManager >
{
private:
	typedef struct tagGIZMOVERTEX {
		D3DXVECTOR3 p;
		DWORD c;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	}GIZMOVERTEX, *LPGIZMOVERTEX;

private:
	LPDIRECT3DDEVICE9		m_pDevice;									//기억된 Device;
	D3DXVECTOR3				m_pCirclePos[CIRCLEGIZMO_SEGMENTS + 1];	//Circle 의 정점 위치

public:
	GizmoManager(void);
	~GizmoManager(void);


	HRESULT Setup(LPDIRECT3DDEVICE9	device);
	void Release() override;

	//선을 그리는 함수
	void Line(const D3DXVECTOR3& startPos, const D3DXVECTOR3& endPos, DWORD color = 0xff808080) const;

	//월드 그리드를 그리는 함수
	void WorldGrid(float cellSize, int cellNum) const;

	//원을 그리는 함수
	void Circle(const D3DXVECTOR3& centerPos, float radius, const D3DXVECTOR3& axis = D3DXVECTOR3(0, 0, 1), DWORD color = 0xff808080) const;

	//와이어 구를 그리는 함수
	void WireSphere(const D3DXVECTOR3& centerPos, float radius, DWORD color = 0xff808080) const;

	//월드 AABB Box 를 그리는 함수
	void AABBBox(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos, DWORD color = 0xff808080) const;
};
#define GIZMO_MGR GizmoManager::GetInstance()
