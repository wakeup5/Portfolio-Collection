#include "stdafx.h"
#include "Mercenary.h"


Mercenary::Mercenary()
{


}


Mercenary::~Mercenary()
{
}

void Mercenary::Setup()
{
	this->pMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/meshes/Mercenary/Mercenary.x");

	_Mweapon = new MercenaryWeapon;
	_Mweapon->Setup();
	_Mweapon->SetActive(true);

	_animation = new cSkinnedAnimation;
	_animation->Init((cXMesh_Skinned*)this->pMesh);
	this->pTransform->SetScale(0.05, 0.05, 0.05);

	this->_animation->AddBoneTransform("Bip002_L_Hand", _Mweapon->pTransform);

	_state = NORMAL;
	_AttackTime = 0.1;

	_effects.push_back(EFFECT_MGR->CreateParticleEmitter<Dust>("MN_RUN"));
	_effects.SetTransform(pTransform);

	LPDIRECT3DTEXTURE9 tex4 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/HitFX.png"));

	SEParam Param4;
	Param4.MinRot = D3DXVECTOR3(0, 0, 0);
	Param4.MaxRot = D3DXVECTOR3(0, 0, 0);
	Param4.MinSpeed = D3DXVECTOR3(0, 2, 0);
	Param4.MaxSpeed = D3DXVECTOR3(0, 2, 0);
	//Param4.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param4.Colors.push_back(D3DXCOLOR(1, 1, 1, 0));
	Param4.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param4.Scales.push_back(D3DXVECTOR3(1.4, 1.4,1.4));

	_ShotFX.push_back(EFFECT_MGR->CreateEffect("ShotFX", tex4, 1, 1, pTransform,
		D3DXVECTOR3(0, 25, -10), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(10, 10, 10), false, &Param4)); //크기));
		
	
}
void Mercenary::BaseObjectUpdate(float timeDelta)
{
	SetPos(_PCtrans->GetWorldPosition());
	_animation->Update(timeDelta);
	_effects.Update(timeDelta);
	_ShotFX.Update(timeDelta);
	AIUpdate(timeDelta);

	if (KEY_MGR->IsOnceDown(VK_F1))
	{
		if (_state == NORMAL)
		{
			_state = BATTLE;
		}
		else
		{
			_state = NORMAL;
		}
	}
	if (KEY_MGR->IsOnceDown(VK_F2))
	{
		_ShotFX.Start(0.3, false);
	}

}

void Mercenary::BaseObjectRender()
{
	_animation->Render(this->pTransform);
	_Mweapon->Render();
	_effects.Render();
	_ShotFX.Render();
//	static_cast<cXMesh_Skinned*>(pMesh)->ShowAnimationName(0, 100);
}

void Mercenary::AIUpdate(float timeDelta)
{
	if (_state == BATTLE)
	{
		AIBattle(timeDelta);
	}
	else if(_state == NORMAL)
	{
		AINormal(timeDelta);
	}
}

void Mercenary::AINormal(float timeDelta)
{
	if (isMove)
	{
		if (KEY_MGR->IsStayDown(VK_LSHIFT))
		{
			_animation->Play("BRUN");
			Speed = 5.1;
		}
		else
		{
			_animation->Play("NRUN");
			Speed = 3.2;
		}
	}
	else
	{
		_animation->Play("BSTAND");
	}

	if (IsPosInDistance(_Pos, 2.5))
	{
		_waitTime += timeDelta;
		if (_waitTime > 0.4)
			MoveDestPosition((Speed)* timeDelta);
		//pTransform->LookDirection(_PCtrans->GetForward());

		D3DXVECTOR3 dir = (_Pos - pTransform->GetWorldPosition());
		//dir.y = _Pos.y;
		D3DXVec3Normalize(&dir, &dir);

		pTransform->LookDirection(-dir);
	}
	else
	{
		isMove = false;
		_waitTime = 0;
	}
}

void Mercenary::AIBattle(float timeDelta)
{
	//주변의 몬스터를 탐색
	Monster* mon = MONSTER_MGR->GetNearMonster(this->pTransform->GetWorldPosition(), 10.0f);

	//먼저 플레이어와 멀리 떨어지면 전투 상태 해제 한다. 몬스터가 죽어도 해제
	if (IsPosInDistance(_PCtrans->GetWorldPosition(), 15) ||
		(mon && mon->IsState(Character::CS_DOWN)))
	{
		AINormal(timeDelta);
		return;
	}

	//몬스터가 있을 경우
	if (mon && mon->GetBattleState() != Character::CBS_BACK)
	{
		SetPos(mon->pTransform->GetWorldPosition());
		D3DXVECTOR3 dir = (_Pos - pTransform->GetWorldPosition());
		//dir.y = _Pos.y;
		D3DXVec3Normalize(&dir, &dir);

		pTransform->LookDirection(-dir);

		//공격 범위에 들어가면..
		if (!IsPosInDistance(_Pos, 5.0f))
		{
			//공격!
			if (_AttackTime < 0)
			{
				if (RandomFloatRange(0, 100) < 20)
				{
					_animation->PlayOneShotAfterHold("ATK_02");
					_attackNum = 2;
					
					_AttackTime = 4;
					_waitTime = 4;
				}
				else
				{
					_animation->PlayOneShotAfterHold("ATK_01");
					_attackNum = 1;

					_AttackTime = 3;
					_waitTime = 3;
				}
			}
			else
			{
				if (_attackNum == 0)
				{
					_animation->Play("BSTAND");
				}

				_waitTime = 0.0f;
			}
		}
		//공격 범위가 아니면 몬스터를 향해 움직인다.
		else
		{
			_waitTime += timeDelta;
			if (_waitTime > 0.4f) MoveDestPosition((Speed)* timeDelta);

			_animation->Play("BRUN");

			_attackNum = 0;
		}
	}
	//몬스터가 없을 경우
	else
	{
		AINormal(timeDelta);
	}

	//공격 쿨타임 시간 적용
	if (_AttackTime > 0)
	{
		_AttackTime -= timeDelta;

		if (_attackNum == 1)
		{
			if (IsTimeMoment(_AttackTime, 2.5f, timeDelta))
			{
				Attack(mon);
			
				_ShotFX.Start(0.3, false);
			}

		}
		else if (_attackNum == 2)
		{
			if (IsTimeMoment(_AttackTime, 3.0f, timeDelta) ||
				IsTimeMoment(_AttackTime, 2.75f, timeDelta) ||
				IsTimeMoment(_AttackTime, 2.35f, timeDelta) ||
				IsTimeMoment(_AttackTime, 2.2f, timeDelta) ||
				IsTimeMoment(_AttackTime, 2.0f, timeDelta))
			{
				Attack(mon);
			
				_ShotFX.Start(0.3, false);
			}
		}
	}

	//애니메이션 끝나면 서있는 걸로
	if (!_animation->IsPlay())
	{
		_animation->Play("BSTAND");
	}

	/*
	if (_mon)
	{
		if (_AttackTime <= 0)
		{
			if (RandomFloatRange(0, 100) < 30)
			{
				_animation->Play("ATK_02");
			}
			else
			{
				_animation->Play("ATK_01");
			}

			_AttackTime = 1;
		}
		else
		{
			_animation->Play("BSTAND");
		}
	}

	else
	{
		if (isMove)
		{
			_animation->Play("BRUN");
			Speed = 5.1;
		}
		else
		{
			_animation->Play("BSTAND");
			Speed = 3.2;
		}


		if (IsPosInDistance(_Pos, 2.5))
		{
			_waitTime += timeDelta;
			if (_waitTime > 0.4)
				MoveDestPosition((Speed)* timeDelta);

			pTransform->LookDirection(_PCtrans->GetForward());
		}
		else
		{
			isMove = false;
			_waitTime = 0;
		}
	}
	*/
}

void Mercenary::Attack(Monster* mon)
{
	if (mon == NULL) return;

	LOG_MGR->AddLog("타격1!!");
		
	mon->GetCharacterInfo().Hp -= 10;
	mon->_MonsterDamage.Start(0.5, false);
}

D3DXVECTOR3 Mercenary::GetDeltaDestPos(float speed)
{
	D3DXVECTOR3 direction, temp = pTransform->GetWorldPosition();// = _DestPos - pTransform->GetWorldPosition();
	temp.y = _Pos.y;
	_Pos.z -= 2.0;
	D3DXVec3Normalize(&direction, &(_Pos - temp));

	return direction * speed;
}
void Mercenary::MoveDestPosition(float speed)
{
	isMove = true;
	_effects.Start(0.3, false);
	pTransform->MovePositionWorld(GetDeltaDestPos(speed));
}

void Mercenary::LookPosition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	D3DXVECTOR3 tempDest = pos;
	tempDest.y = temp.y;

	D3DXVECTOR3 direction = temp - tempDest;
	direction.z += 2;
	D3DXVec3Normalize(&direction, &direction);
	pTransform->LookDirection(direction);
}

bool Mercenary::IsPosInDistance(D3DXVECTOR3 destPos, float distance)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	//destPos.y = 0;
	temp.y = destPos.y;
	return D3DXVec3Length(&(temp - destPos)) > distance;
}

