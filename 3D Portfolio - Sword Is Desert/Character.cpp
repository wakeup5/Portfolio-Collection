#include "stdafx.h"
#include "Character.h"


Character::Character()
{
}


Character::~Character()
{
}

void Character::Setup()
{
	
}

void Character::Release()
{
}

void Character::BaseObjectEnable()
{
}

void Character::BaseObjectDisable()
{
}

void Character::BaseObjectUpdate(float timeDelta)
{
	if (_hitStopTime > 0) _hitStopTime -= timeDelta;
}

void Character::BaseObjectNoActiveUpdate(float timeDelte)
{
}

void Character::BaseObjectRender()
{
}

bool Character::IsState(Character::State state)
{
	return _state == state;
}

bool Character::CanState(Character::State state)
{
	return _state < state;
}

bool Character::SetState(Character::State state)
{
	bool result = IsState(state);

	_state = state;

	return result;
}

void Character::ActionState(const char *animationName, Character::State state, float fadeTime, bool isOnce)
{
	if (!CanState(state))
		return;

	if (isOnce)
		animation->PlayOneShotAfterHold(animationName, fadeTime);
	else
		animation->Play(animationName, fadeTime);

	SetState(state);
}

void Character::SetHeight(float height)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	pTransform->SetWorldPosition(temp.x, height, temp.z);
}

/*
void Character::SetAttackBound(OUT cBoundBox ** transArr, IN int arrLength)
{
	ZeroMemory(_attackBound, sizeof(cBoundBox*) * arrLength);

	for (int i = 0; i < arrLength; i++)
	{
		_attackBound[i] = new cBoundBox;
		_attackBound[i]->SetBound()
	}
}
*/

void Character::SetCharacterInfo(CharacterInfo & info)
{
	_charInfo = info;
	_charInfo.Hp = _charInfo.MaxHp;
	_charInfo.Mp = _charInfo.MaxMp;
	
}

CharacterInfo& Character::GetCharacterInfo()
{
	return _charInfo;
}

bool Character::GetAttackBound(Transform*** TransArr, int* num)
{
	if (AtkTransLength == 0)
	{
		*TransArr = NULL;
		*num = 0;
		return false;
	}

	*TransArr = _attackTransform;
	*num = AtkTransLength;
	return true;
	
}
bool Character::GetHitBound(Transform*** TransArr, int* num)
{
	if (hitTransLength == 0)
	{
		*TransArr = NULL;
		*num = 0;
		return false;
	}

	*TransArr = _hitTransform;
	*num = hitTransLength;

	return true;
}