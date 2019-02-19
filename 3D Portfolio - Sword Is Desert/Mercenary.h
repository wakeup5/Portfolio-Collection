#pragma once
#include "Pet.h"
#include "MercenaryWeapon.h"

enum MERCENARY_State
{
	NORMAL,
	BATTLE
};

class Mercenary :
	public Pet
{
private:
	float AtkSpeed = 100.0f;
	MERCENARY_State _state;
	D3DXVECTOR3 EnemyPos;
	float _AttackTime;
	MercenaryWeapon* _Mweapon;

	UINT _attackNum;
public:
	Mercenary();
	~Mercenary();

	void Setup();

	EffectVector _effects;
	EffectVector _ShotFX;
	EffectVector _MonDamageFX;

	D3DXVECTOR3 GetDeltaDestPos(float speed);
	
	void LookPosition(D3DXVECTOR3 pos);
	void BaseObjectUpdate(float timeDelta);
	bool IsPosInDistance(D3DXVECTOR3 destPos, float distance);
	void MoveDestPosition(float speed);
	virtual void BaseObjectRender();

	void AIUpdate(float timeDelta);
	void AINormal(float timeDelta);
	void AIBattle(float timeDelta);

	void Attack(Monster* mon);
};

