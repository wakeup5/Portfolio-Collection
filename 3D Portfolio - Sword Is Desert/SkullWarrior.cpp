#include "stdafx.h"
#include "SkullWarrior.h"


SkullWarrior::SkullWarrior()
{
}


SkullWarrior::~SkullWarrior()
{
}


void SkullWarrior::AttackUpdate(UINT atkNum)
{
	Player* player = PLAYER_MGR->GetPlayer();
	cBoundBox* pBox = &player->BoundBox;

	if (!animation->IsPlay())
	{
		_attackNum = 0;
	}

	switch (_attackNum)
	{
	case 2:
	{
		if (_atkTimer->Check(1, 0.9))
		{
			PLAYER_MGR->MonsterCollision(this);
		}
		break;
	}
	case 1:
	{
		if (_atkTimer->Check(2, 1.1))
		{
			PLAYER_MGR->MonsterCollision(this);
		}
	

		break;

	}

	}
}
