#pragma once
#include "Camera.h"
#include "cFrustum.h"

typedef void(*callback)(void);

class MyCamera : public Camera
{
public:
	cFrustum Frustum;

protected:
	D3DXVECTOR3 _fromPos;
	D3DXVECTOR3 _toPos;

	D3DXVECTOR3 _fromLook;
	D3DXVECTOR3 _toLook;

	D3DXVECTOR3 _currentPos;
	D3DXVECTOR3 _currentLook;

	//보간을 위한 타이머 변수
	float _posTotalTime = 0;
	float _posCurrentTime = 0;

	float _lookTotalTime = 0;
	float _lookCurrentTime = 0;

	//보간 끝나면 호출 콜백 함수
	callback posCall = NULL;
	callback lookCall = NULL;
	callback fovCall = NULL;

	//fov 값 보간
	float _prevFov = RAD(60);
	float _fromFov;
	float _toFov;

	float _fovTotalTime = 0;
	float _fovCurrentTime = 0;

	bool _isPrevFov;
public:
	MyCamera();
	~MyCamera();

	virtual void Update(float timeDelta);

	void SetLook(D3DXVECTOR3 look) { _toLook = _currentLook = look; }
	void SetPos(D3DXVECTOR3 pos) { _toPos = _currentPos = pos; }
	void SetFov(float f) { fov = f; };

	void SetLook(D3DXVECTOR3 from, D3DXVECTOR3 to, float totalTime, void(endCall)(void) = 0);
	void SetPos(D3DXVECTOR3 from, D3DXVECTOR3 to, float totalTime, void(endCall)(void) = 0);
	void SetFov(float from, float to, float totalTome, float endFov = RAD(60), void(endCall)(void) = 0);

	bool IsPosLerpEnd() { return _posCurrentTime <= 0; }
	bool IsLookLerpEnd() { return _lookCurrentTime <= 0; }

	D3DXVECTOR3 GetDirectForward();

	virtual float GetDistance() { return D3DXVec3Length(&(_currentPos - _currentLook)); }
	virtual LRESULT WndProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam);
};

class PlayerCamera : public MyCamera
{
private:
	class CameraMouseRotate
	{
	public:
		float _x = 0, _y = 0, _z = 0;

		bool _isBoundMouse = false;
	public:
		bool IsBoundMouse() { return _isBoundMouse; }
		void SetBoundMouseState(bool is) { _isBoundMouse = is; }

		void Update();
	} _camMouseRot;

private:
	float _dist = 5;

	D3DXVECTOR3 _pos = { 0, 0, 0 }, _viewPos = { 0, 2, 0 };
public:
	virtual void Update(float timeDelta);

	void SetPlayerPosition(D3DXVECTOR3 pos) { _pos = pos; }
	void TransRotate(OUT D3DXVECTOR3* pos, IN D3DXVECTOR3* in);
	void TransRotateY(OUT D3DXVECTOR3* out, IN D3DXVECTOR3* in);

	float GetX() { return _camMouseRot._x; }
	float GetY() { return 180 + _camMouseRot._y; }

	virtual LRESULT WndProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam);
};

class CameraManager : public Singleton < CameraManager >
{
private:
	static PlayerCamera* _playerCamera;
	static MyCamera* _NPCamera;
	static MyCamera* _MapViewCamera;

	static MyCamera* _currentCamera;

public:
	void SetPlayer() { _currentCamera = _playerCamera; ShowCursor(true); }
	void SetNPC() { _currentCamera = _NPCamera; ShowCursor(true); }
	void SetMapView() { _currentCamera = _MapViewCamera; ShowCursor(true); }

	static PlayerCamera* GetPlayerCamera() { return _playerCamera; }
	static MyCamera* GetNPCamera() { return _NPCamera; }
	static MyCamera* GetMapViewCamera() { return _MapViewCamera; }

	static MyCamera* GetCamera() { return _currentCamera; }
};

#define CAMERA_MGR CameraManager::GetInstance()
#define CAMERA CameraManager::GetCamera()
#define PLAYER_CAMERA CameraManager::GetPlayerCamera()
#define NPC_CAMERA CameraManager::GetNPCamera()
#define MAPVIEW_CAMERA CameraManager::GetMapViewCamera()
