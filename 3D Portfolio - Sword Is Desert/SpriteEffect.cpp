#include "stdafx.h"
#include "SpriteEffect.h"

SpriteEffect::SpriteEffect()
{
}


SpriteEffect::~SpriteEffect()
{
}

void SpriteEffect::Setup(const SEParam* param)
{
	//버퍼 셋팅
	_vertex[0] = SpriteEffect::MyPCT_Vertex(1, 1, 0, 0, 0);
	_vertex[1] = SpriteEffect::MyPCT_Vertex(-1, 1, 0, 1, 0);
	_vertex[2] = SpriteEffect::MyPCT_Vertex(-1, -1, 0, 1, 1);
	_vertex[3] = _vertex[0];
	_vertex[4] = _vertex[2];
	_vertex[5] = SpriteEffect::MyPCT_Vertex(1, -1, 0, 0, 1);

	_vertex2[0] = SpriteEffect::MyPCT_Vertex(0, 1, 1, 0, 0);
	_vertex2[1] = SpriteEffect::MyPCT_Vertex(0, 1, -1, 1, 0);
	_vertex2[2] = SpriteEffect::MyPCT_Vertex(0, -1, -1, 1, 1);
	_vertex2[3] = _vertex2[0];
	_vertex2[4] = _vertex2[2];
	_vertex2[5] = SpriteEffect::MyPCT_Vertex(0, -1, 1, 0, 1);

	for (int i = 0; i < 6; i++)
	{
		_vertex2[i].c = _vertex[i].c = D3DXCOLOR(1, 1, 1, 1);
	}

	if (Local) SAFE_DELETE(Local);
	Local = new Transform;

	_prevMat = Local->GetFinalMatrix();
	_prevScale = Local->GetScale();

	if (param != NULL)
	{
		_posDelta.x = RandomFloatRange(param->MinSpeed.x, param->MaxSpeed.x);
		_posDelta.y = RandomFloatRange(param->MinSpeed.y, param->MaxSpeed.y);
		_posDelta.z = RandomFloatRange(param->MinSpeed.z, param->MaxSpeed.z); 
		_rotDelta.x = RandomFloatRange(param->MinRot.x, param->MaxRot.x);
		_rotDelta.y = RandomFloatRange(param->MinRot.y, param->MaxRot.y);
		_rotDelta.z = RandomFloatRange(param->MinRot.z, param->MaxRot.z);
		_scaleDalta = param->Scales;
		_colorDalta = param->Colors;
	}
	else
	{
		_posDelta = D3DXVECTOR3(0, 0, 0);
		_rotDelta = D3DXVECTOR3(0, 0, 0);
		_scaleDalta.clear();
		_colorDalta.clear();
	}

	_isLookCamera = false;
	_isRendering = true;
	_isZBufferOff = false;

	_lifeTime = _currentTime = 0;
}

void SpriteEffect::Release()
{
	//SAFE_DELETE(Origin);
	Origin = NULL;
	SAFE_DELETE(Local);
}

void SpriteEffect::Update(float timeDelta)
{
	//재생 안할 경우
	if (!_isRun) return;

	//시간 지나면?
	if (_lifeTime <= _currentTime)
	{
		//반복일 경우
		if (_isLoop)
		{
			Stop();
			Start();
		}
		else
		{
			Stop();
			return;
		}
	}
	else
	{
		//시간 증가
		_currentTime += timeDelta;
	}

	//업데이트
	float liveTime = Clamp01(_currentTime / _lifeTime);

	if (liveTime >= 0.99) liveTime = 0.99;

	if (_lifeTime == 0)
	{
		liveTime = 0;
	}

	DWORD color = D3DXCOLOR(1, 1, 1, 1);

	//이동 회전 크기
	Local->MovePositionLocal(_posDelta * timeDelta);
	Local->RotateLocal(_rotDelta * timeDelta);

	if (_scaleDalta.size() >= 2)
	{
		D3DXVECTOR3 s, scale = this->Scale;
		int lastIndex = _scaleDalta.size() - 1;
		if (liveTime >= 0.98)
		{
			s = _scaleDalta[_scaleDalta.size() - 1];
		}
		else if (liveTime == 0.0f)
		{
			s = _scaleDalta[0];
		}
		else 
		{
			float interval = 1.0f / lastIndex;
			float position = liveTime / interval;
			int startIndex = (int)position;
			int endIndex = startIndex + 1;
			float fN = position - startIndex;
			s = (_scaleDalta[endIndex] - _scaleDalta[startIndex]) * fN + _scaleDalta[startIndex];
		}

		scale.x *= s.x;
		scale.y *= s.y;
		scale.z *= s.z;
		Local->SetScale(scale);
	}

	if (_colorDalta.size() >= 2)
	{
		int lastIndex = _colorDalta.size() - 1;
		float interval = 1.0f / lastIndex;
		float position = liveTime / interval;
		int startIndex = (int)position;
		int endIndex = startIndex + 1;

		//보간값
		float fN = position - startIndex;

		//최종 컬러값
		D3DXCOLOR color1;

		//컬러 선형 보간
		D3DXColorLerp(&color1,			//결과
			&D3DXCOLOR(_colorDalta[startIndex]),		//from
			&D3DXCOLOR(_colorDalta[endIndex]),			//to
			fN);						//normalize Factor

		color = (DWORD)(color1);
	}
	else if (_colorDalta.size() == 1)
	{
		color = _colorDalta[0];
	}
	
	//UV애니메이션 셋팅
	float blockXSize = 1 / (float)_maxU, blockYSize = 1 / (float)_maxV;
	UINT totalLength = _maxU * _maxV;
	UINT currentIndex = totalLength * liveTime;
	UINT cx, cy;
	float startU, endU, startV, endV;

	if (currentIndex > totalLength) currentIndex = totalLength;

	cx = currentIndex % _maxU;
	cy = currentIndex / _maxV;

	startU = (float)cx * blockXSize;
	endU = (float)(cx + 1) * blockXSize;
	startV = (float)cy * blockYSize;
	endV = (float)(cy + 1) * blockYSize;

	_vertex[0].t = _vertex2[0].t = D3DXVECTOR2(startU, startV);
	_vertex[1].t = _vertex2[1].t = D3DXVECTOR2(endU, startV);
	_vertex[2].t = _vertex2[2].t = D3DXVECTOR2(endU, endV);

	_vertex[3].t = _vertex2[3].t = D3DXVECTOR2(startU, startV);
	_vertex[4].t = _vertex2[4].t = D3DXVECTOR2(endU, endV);
	_vertex[5].t = _vertex2[5].t = D3DXVECTOR2(startU, endV);

	for (int i = 0; i < 6; i++)
	{
		_vertex[i].c = _vertex2[i].c = color;
	}

	if (_isLookCamera)
	{
		if (_rotDelta == D3DXVECTOR3(0, 0, 0))
		{
			Local->LookPosition(CAMERA->GetWorldPosition(), Origin, D3DXVECTOR3(0, 1, 0));
		}
		else
		{
			Local->LookPosition(CAMERA->GetWorldPosition(), Origin, Local->GetUp());
		}
	}
}

void SpriteEffect::Render()
{
	if (!_isRun || !_texture || !_isRendering) return;

	D3DXMATRIXA16 matWorld = Local->GetFinalMatrix();

	if (Origin) matWorld *= Origin->GetFinalMatrix();

	if(_isZBufferOff) 
		DEVICE->SetRenderState(D3DRS_ZENABLE, false);

	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	DEVICE->SetTexture(0, _texture);
	DEVICE->SetFVF(SpriteEffect::MyPCT_Vertex::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _vertex, sizeof(SpriteEffect::MyPCT_Vertex));

	if (_isCross)
	{
		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _vertex2, sizeof(SpriteEffect::MyPCT_Vertex));
	}

	if (_isZBufferOff) DEVICE->SetRenderState(D3DRS_ZENABLE, true);
}

void SpriteEffect::Start()
{
	_currentTime = 0;
	_isRun = true;

	_prevMat = Local->GetFinalMatrix();
	_prevScale = Local->GetScale();
}

void SpriteEffect::Start(float lifeTime, bool isLoop)
{
	_lifeTime = lifeTime;
	_currentTime = 0;

	_isLoop = isLoop;
	_isRun = true;

	_prevMat = Local->GetFinalMatrix();
	_prevScale = Local->GetScale();
}

void SpriteEffect::Stop()
{
	_isRun = false;

	Local->SetWorldMatrix(_prevMat);
	Local->SetScale(_prevScale);
}

void SpriteEffect::SetCrossState(bool is)
{
	_isCross = is;
}

void SpriteEffect::SetLookCameraState(bool is)
{
	_isLookCamera = is;
}

void SpriteEffect::SetRenderingState(bool is)
{
	_isRendering = is;
}

void SpriteEffect::SetZBufferOffState(bool is)
{
	_isZBufferOff = is;
}

void SpriteEffect::SetUV(UINT u, UINT v)
{
	_maxU = u;
	_maxV = v;

	if (_maxU < 1) _maxU = 1;
	if (_maxV < 1) _maxV = 1;
}

void SpriteEffect::SetTexture(LPDIRECT3DTEXTURE9 tex)
{
	_texture = tex;
}
