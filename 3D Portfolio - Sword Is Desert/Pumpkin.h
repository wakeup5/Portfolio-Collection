#pragma once
#include "Monster.h"
class Pumpkin :
	public Monster
{
public:
	Pumpkin();
	~Pumpkin();

	void AttackUpdate(UINT atkNum);
};

