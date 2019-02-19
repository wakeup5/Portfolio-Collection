#include "stdafx.h"
#include "Golem.h"


Golem::Golem()
{
}


Golem::~Golem()
{
}

void Golem::AttackUpdate(UINT atkNum)
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
		if (_atkTimer->Check(1, 0.8))
		{
			PLAYER_MGR->MonsterCollision(this);
		}
		break;
	}
	case 1:
	{
		if (_atkTimer->Check(2, 0.6))
		{
			PLAYER_MGR->MonsterCollision(this);
		}
		else if (_atkTimer->Check(3, 0.8))
		{
			PLAYER_MGR->MonsterCollision(this);
		}
		else if (_atkTimer->Check(4, 1.0))
		{
			PLAYER_MGR->MonsterCollision(this);
		}
		else if (_atkTimer->Check(5, 1.2))
		{
			PLAYER_MGR->MonsterCollision(this);
		}
		break;

	}

	}
}
