#pragma once
#include "Character.h"
#include "cSkinnedAnimation.h"
#include "NameTag.h"
#include "UIProgress.h"

class Monster : public Character
{
protected:

	RECT _Area;
	D3DXVECTOR3 _DestPos;
	//float _Speed;
	float _StandTime;
public:
	D3DXVECTOR3 _PrevPos;

	bool _isBoss = false;
	bool _isMiddleBoss = false;
protected:
	float _AttackTime;
	//float _AttackCoolTime;

	float _respawnTime;

	const static Transform* PlayerTrans;

	//플레이어 때렸냐? 공격모션 끝나면 다시 false됨.
	bool _isAtkPlayer;

	//몬스터 정보 출력용 네임태그
	NameTag _tag;
	NameTagTexture _tagTexture; //이름이랑 피 그릴 텍스쳐 가지고 있음
	UIProgress _hpProgress;

	std::string _name;

	UINT _attackNum;

	Timer* _atkTimer;
public:
	Monster();
	virtual ~Monster();

	EffectVector	_MonsterDamage;
	EffectVector	_MonsterDamage2;
	EffectVector	_MonsterDamage3;
	EffectVector	_MonsterDamage4;

	virtual void Setup(const char* path, RECT rect, CharacterInfo &info);

	virtual void BaseObjectEnable();						//BaseObject 가 활성화 될때 실행
	virtual void BaseObjectDisable();						//BaseObject 가 비활성화 될때 실행
	virtual void BaseObjectUpdate(float timeDelta);			//BaseObject 가 Update 때 실행....
	virtual void BaseObjectNoActiveUpdate(float timeDelte);	//BaseObject 가 비활성화시 업데이트 실행....
	virtual void BaseObjectRender();

	void SetArea(RECT rect);

	void SetName(std::string name) { _name = name; }
	std::string GetName(){ return _name; }
protected:
	virtual void AIUpdate(float timeDelta);
	void AINormalUpdate(float timeDelta);
	virtual void AIBattleUpdate(float timeDelta);
	void AIBackUpdate(float timeDelta);
	virtual void MoveUpdate(float timeDelta);

protected:
	D3DXVECTOR3 GetDeltaDestPos(float speed);
	void LookPosition(D3DXVECTOR3 pos);
	void MoveDestPosition(float speed);

	bool IsDestPos(float distance = 0.0f);
	bool IsPosInDistance(D3DXVECTOR3 destPos, float distance = 0.0f);

	virtual void AttackUpdate(UINT atkNum);
public:
	float GetPlayerDistance();

	static void SetPlayerTrans(const Transform* ptrans){ PlayerTrans = ptrans; }

	virtual bool GetAttackTransform(OUT Transform** transArr, OUT int* arrLength);
	virtual bool GetHitTransform(OUT Transform** transArr, OUT int* arrLength);

	Transform* GetAtkTrans() { return _hitTransform[0]; }

	virtual void Hit(float damage);
	void Down();
	virtual void Alive();

	void PrintNameTag();
};

