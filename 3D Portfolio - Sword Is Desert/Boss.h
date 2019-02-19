#pragma once
#include "Monster.h"
#include "SpriteEffect.h"
#include "Boss_Around_Dust.h"
//#include "BossStonePT.h"
#include "BossPoisonCloud.h"

class Boss :
	public Monster
{
private:

	D3DXVECTOR3 _tempDir;
	D3DXVECTOR3 _lookDir;

	bool _shouting;

	float _atkRunTime;

	//공격 범위들
	cBoundBox _attackBound1; //1번 공격
	Transform* _attackTrans1;

	cBoundBox _attackBound2; //이동 공격
	Transform* _attackTrans21;
	Transform* _attackTrans22;

	cBoundBox _attackBound3; //브레스
	Transform* _attackTrans3;

	cBoundBox _attackBound4; //내려찍음
	Transform* _attackTrans4;

	cBoundBox _attackBound5; //팔 휘두름
	Transform* _attackTrans5;

	cBoundBox _attackBound6; //우 회전 공격
	Transform* _attackTrans6;

	cBoundBox _attackBound7; //죄 회전 공격
	Transform* _attackTrans7;

	//이펙트
	EffectVector _effect[10]; //그냥 공격들 이펙트

	//타이밍 조절을 위한 타이머 객체
	Timer* _shoutingTimer;
public:
	Boss();
	~Boss();

	Boss_Around_Dust* _WalkDust;
	//BossStonePT* _StonePT;
	BossPoisonCloud* _PoisonCloud;

	virtual void Setup(const char* path, RECT rect, CharacterInfo &info);
	//virtual void Setup(const char* path, RECT rect, CharacterInfo &info);
	virtual void BaseObjectUpdate(float timeDelta);
	virtual void BaseObjectRender();
	
	virtual void AIUpdate(float timeDelta);
	virtual void AIBattleUpdate(float timeDelta);
	virtual void MoveUpdate(float timeDelta);


	virtual bool GetHitTransform(OUT Transform ** transArr, OUT int * arrLength);

	virtual void Alive();

	bool Shouting();
	void AttackUpdate(UINT atkNum);
	void Attack();
	void AttackStart();
	
	void EffectSetup();
	void EffectUpdate(float timeDelta);
	void EffectRender();

	void Hit(float damage);
};

