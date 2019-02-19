#pragma once
#include "Monster.h"
class Muspel :
	public Monster
{
public:
	Muspel();
	~Muspel();

	void AttackUpdate(UINT atkNum);
};

