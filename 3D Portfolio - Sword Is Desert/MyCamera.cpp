#include "stdafx.h"
#include "MyCamera.h"
#include "LerpAnimation.h"

using namespace LerpAnimation;

MyCamera::MyCamera()
	: _toPos(0, 0, 0)
	, _fromPos(0, 0, 0)
	, _toLook(0, 0, 0)
	, _fromLook(0, 0, 0)
	, _currentLook(0, 0, 0)
	, _currentPos(0, 0, 0)
{
	SetFar(3000);
}


MyCamera::~MyCamera()
{
}


void MyCamera::Update(float timeDelta)
{
	//update
	Frustum.UpdateFrustum(&GetViewProjectionMatrix());
	ShakeUpdate(timeDelta);
	this->UpdateCamToDevice(DEVICE);

	//위치
	SetWorldPosition(_currentPos);
	LookPosition(_currentLook);

	//위치 보간
	if (_posCurrentTime > 0)
	{
		_posCurrentTime -= timeDelta;
		_currentPos = Lerp<EaseOutQuart>(_fromPos, _toPos, 1 - _posCurrentTime / _posTotalTime);
	}
	else if(posCall != 0)
	{
		posCall();
		posCall = 0;
	}

	if (_lookCurrentTime > 0)
	{
		_lookCurrentTime -= timeDelta;
		_currentLook = Lerp<EaseOutQuart>(_fromLook, _toLook, 1 - _lookCurrentTime / _lookTotalTime);
	}
	else if (lookCall != 0)
	{
		lookCall();
		lookCall = 0;
	}

	if (_fovCurrentTime > 0)
	{
		_fovCurrentTime -= timeDelta;
		fov = Lerp<EaseOutQurd>(_fromFov, _toFov, 1 - _fovCurrentTime / _fovTotalTime);
	}
	else// if(_isPrevFov)
	{
		fov = _prevFov;
	}
}

LRESULT MyCamera::WndProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
/*
void MyCamera::Look(D3DXVECTOR3 look)
{
	//회전
	D3DXMATRIXA16 matRot, matRotX, matRotY;
	D3DXMatrixRotationX(&matRotX, RAD(-_rot.x));
	D3DXMatrixRotationY(&matRotY, RAD(_rot.y));
	matRot = matRotX * matRotY;

	//거리
	fov = RAD(max(min(_dist, 4), 2) * 15);

	D3DXVECTOR3 camPos = D3DXVECTOR3(0, 0, 1) * max(_dist, 4);
	D3DXVECTOR3 viewPos;
	//최종 보는 위치
	D3DXVec3TransformCoord(&viewPos, &_viewPos, &matRotY);
	D3DXVECTOR3 finalLook = look + viewPos;

	D3DXVec3TransformNormal(&camPos, &camPos, &matRot);

	this->SetDestPos(finalLook + camPos);
	this->LookPosition(finalLook);
}

void MyCamera::Look(D3DXVECTOR3 look, float dist)
{
	D3DXMATRIXA16 matRot, matRotX, matRotY;
	D3DXMatrixRotationX(&matRotX, RAD(-_rot.x));
	D3DXMatrixRotationY(&matRotY, RAD(_rot.y));
	matRot = matRotX * matRotY;

	D3DXVECTOR3 camPos = D3DXVECTOR3(0, 0, 1) * dist;
	D3DXVECTOR3 viewPos;
	//최종 보는 위치
	D3DXVec3TransformCoord(&viewPos, &_viewPos, &matRotY);
	D3DXVECTOR3 finalLook = look + viewPos;

	D3DXVec3TransformNormal(&camPos, &camPos, &matRot);

	this->SetDestPos(finalLook + camPos);
	this->LookPosition(finalLook);
}

void MyCamera::Look(D3DXVECTOR3 look, D3DXVECTOR3 pos)
{
	this->SetDestPos(pos);
	this->LookPosition(look);
}
*/

void MyCamera::SetLook(D3DXVECTOR3 from, D3DXVECTOR3 to, float totalTime, void(endCall)(void))
{
	_fromLook = from;
	_toLook = to;
	_lookTotalTime = _lookCurrentTime = totalTime;
	lookCall = endCall;
}

void MyCamera::SetPos(D3DXVECTOR3 from, D3DXVECTOR3 to, float totalTime, void(endCall)(void))
{
	_fromPos = from;
	_toPos = to;
	_posTotalTime = _posCurrentTime = totalTime;
	posCall = endCall;
}

void MyCamera::SetFov(float from, float to, float totalTome, float endFov, void(endCall)(void))
{
	_fromFov = from;
	_toFov = to;

	_fovTotalTime = _fovCurrentTime = totalTome;

	_prevFov = fov;
	lookCall = endCall;

	_prevFov = endFov;

	//LOG_F(from);
}

D3DXVECTOR3 MyCamera::GetDirectForward()
{
	D3DXVECTOR3 f = GetForward();
	f.y = 0;

	D3DXVECTOR3 result;
	D3DXVec3Normalize(&result, &f);
	return result;
}

void PlayerCamera::CameraMouseRotate::Update()
{
	//회전
	if (_isBoundMouse)
	{
		//화면의 중심위치
		int screenCenterX = WINSIZE_X / 2;
		int screenCenterY = WINSIZE_Y / 2;

		//현재 마우스 위치
		POINT mousePos = GetMousePos();

		//이동량 ( 중앙에서 멀어진 량 )
		float deltaX = mousePos.x - screenCenterX;
		float deltaY = mousePos.y - screenCenterY;

		//앵글 추가
		_y += deltaX * 0.33f;
		_x += deltaY * 0.33f;

		//앵글값을 min max 범위 안으로
		_x = Clamp(_x, -85.0f, 85.0f);

		//다시 마우스 위치를 센터로...
		SetMousePos(screenCenterX, screenCenterY);
	}
}

PlayerCamera* CameraManager::_playerCamera = new PlayerCamera;
MyCamera* CameraManager::_NPCamera = new MyCamera;
MyCamera* CameraManager::_MapViewCamera = new MyCamera;
MyCamera* CameraManager::_currentCamera = CameraManager::_playerCamera;

void PlayerCamera::Update(float timeDelta)
{
	//업데이트
	Frustum.UpdateFrustum(&GetViewProjectionMatrix());
	ShakeUpdate(timeDelta);
	this->UpdateCamToDevice(DEVICE);
	
	_camMouseRot.Update();

	//위치
	if (KEY_MGR->IsStayDown(VK_RIGHT))	_viewPos.x += 1.0f * timeDelta;
	if (KEY_MGR->IsStayDown(VK_LEFT))	_viewPos.x -= 1.0f * timeDelta;
	if (KEY_MGR->IsStayDown(VK_UP))		_viewPos.y += 1.0f * timeDelta;
	if (KEY_MGR->IsStayDown(VK_DOWN))	_viewPos.y -= 1.0f * timeDelta;

	if (KEY_MGR->IsOnceDown(VK_LCONTROL))
	{
		_camMouseRot.SetBoundMouseState(!_camMouseRot.IsBoundMouse());
		SetMousePos(WINSIZE_X / 2, WINSIZE_Y / 2);

		if (_camMouseRot.IsBoundMouse())
		{
			//ShowCursor(false);
		}
		else
		{
			//ShowCursor(true);
		}
	}

	D3DXVECTOR3 camPos(0, 0, -1), tempPos, viewTemp;

	TransRotate(&tempPos, &(camPos * _dist));
	TransRotateY(&viewTemp, &_viewPos);

	SetLook(_pos + viewTemp);
	SetPos(_pos + viewTemp + tempPos);

	SetWorldPosition(_currentPos);
	LookPosition(_currentLook);
	/*
	cTerrain* temp = TERRAIN_MGR->GetTerrain("worldMap");
	if (temp != NULL)
	{
		Ray r;
		
		r.origin = _pos + viewTemp + tempPos;
		r.direction = -tempPos;

		D3DXVECTOR3 pos;
		if (temp->IsIntersectRay(&pos, &r) &&
			D3DXVec3Length(&(r.origin - pos)) < _dist)
		{
			SetWorldPosition(pos);
		}
	}
	//*/
	if (_fovCurrentTime > 0)
	{
		_fovCurrentTime -= timeDelta;
		fov = Lerp<EaseOutQurd>(_fromFov, _toFov, 1 - _fovCurrentTime / _fovTotalTime);
	}
	else
	{
		fov = _prevFov;
	}
}

void PlayerCamera::TransRotate(OUT D3DXVECTOR3* out, IN D3DXVECTOR3* in)
{
	D3DXMATRIXA16 total, rx, ry;

	D3DXMatrixRotationX(&rx, RAD(_camMouseRot._x));
	D3DXMatrixRotationY(&ry, RAD(_camMouseRot._y));

	total = rx * ry;

	D3DXVec3TransformCoord(out, in, &total);
}

void PlayerCamera::TransRotateY(OUT D3DXVECTOR3* out, IN D3DXVECTOR3* in)
{
	D3DXMATRIXA16 ry;
	
	D3DXMatrixRotationY(&ry, RAD(_camMouseRot._y));

	D3DXVec3TransformCoord(out, in, &ry);
}

LRESULT PlayerCamera::WndProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_MOUSEWHEEL)
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			_dist -= 10 * TIME_MGR->GetFrameDeltaSec();
		}
		else
		{
			_dist += 10 * TIME_MGR->GetFrameDeltaSec();
		}

		if (_dist > 7) _dist = 7;
		if (_dist < 2) _dist = 2;
	}

	return 0;
}
