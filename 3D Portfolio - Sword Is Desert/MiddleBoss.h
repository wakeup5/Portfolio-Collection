#pragma once
#include "Monster.h"
#include "Orc_Dust.h"
#include "OrcPoisonCloud.h"

class MonsterManager;

class MiddleBoss :
	public Monster
{
private: 
	bool isSpawn = false;
	float _waitTime = 0.0f;
	float _atkRunTime = 0.0f;

	UINT _attackNum;

	cBoundBox _attackBound1; //1¹ø °ø°Ý
	Transform* _attackTrans1;
	Monster* Mons[5];
	bool isOrcSpawn;
	Monster* Mon;
	Orc_Dust* _Dust;
	OrcPoisonCloud* _Poison;

public:
	MiddleBoss();
	~MiddleBoss();

	EffectVector _Atk01FX;
	EffectVector _Atk02FX;
	EffectVector _Atk03FX;
	EffectVector _Atk04FX;

	virtual void Setup(const char* path, RECT rect, CharacterInfo &info);
	virtual void BaseObjectUpdate(float timeDelta);
	virtual void BaseObjectRender();
	virtual void AIBattleUpdate(float timeDelta);
	virtual void MoveUpdate(float timeDelta);

	virtual void AINormalUpdate(float timeDelta);
	virtual bool GetHitTransform(OUT Transform ** transArr, OUT int * arrLength);
	virtual void SetHeight(float height);
	virtual void Alive();
	void CreateOrc();

	void Hit(float damage);

	EffectVector Attack01FX;

	void AttackUpdate();
};

