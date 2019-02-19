#pragma once
#include "Monster.h"
class Pet :
	public cBaseObject
{
protected:
	cSkinnedAnimation* _animation;
	D3DXVECTOR3 _Pos;
	bool isMove;
	float Speed;
	float _height;
	Transform* _PCtrans;
	float _waitTime = 0;

public:
	Pet();
	~Pet();
	EffectVector _effects;
	void Setup();
	virtual void BaseObjectUpdate(float timeDelta);
	virtual void BaseObjectRender();
	void LookPosition(D3DXVECTOR3 pos);
	void SetPos(D3DXVECTOR3 pos) { _Pos = pos; _Pos.y = pTransform->GetWorldPosition().y; }
	bool IsPosInDistance(D3DXVECTOR3 destPos, float distance);
	void MoveDestPosition(float speed);
	D3DXVECTOR3 GetDeltaDestPos(float speed);
	void SetSpeed(float speed){ Speed = speed; }
	void SetHeight(float height);
	void SetPcTrans(Transform* trans){ _PCtrans = trans; }
};

