#pragma once
#include "Monster.h"
class SkullWarrior :
	public Monster
{
public:
	SkullWarrior();
	~SkullWarrior();

	void AttackUpdate(UINT atkNum);
};

