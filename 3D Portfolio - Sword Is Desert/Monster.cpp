#include "stdafx.h"
#include "Monster.h"

const Transform* Monster::PlayerTrans = NULL;
Monster::Monster()
{
}


Monster::~Monster()
{
}

void Monster::Setup(const char* path, RECT rect, CharacterInfo &info)
{
	this->pMesh = RESOURCE_SKINNEDXMESH->GetResource(path);
	this->animation = new cSkinnedAnimation;
	this->animation->Init((cXMesh_Skinned*)this->pMesh);


	_StandTime = RandomFloatRange(5, 20);

	SetArea(rect);

	_DestPos.x = RandomFloatRange(_Area.left, _Area.right);
	_DestPos.z = RandomFloatRange(_Area.top, _Area.bottom);
	_DestPos.y = 0;

	pTransform->SetWorldPosition(_DestPos);

	_battleState == Character::CBS_NORMAL;

	_AttackTime = 0;

	_respawnTime = info.RespwanTime;

	SetCharacterInfo(info);

	GetAttackTransform(_attackTransform, &AtkTransLength);
	GetHitTransform(_hitTransform, &hitTransLength);

	//네임태그 셋팅
	_tagTexture.Setup(100, 50); //텍스쳐 생성 후,
	_tag.SetTexture(_tagTexture.GetTexture(), _tagTexture.GetInfo()); //텍스쳐와 인포를 넣어준다.

	_hpProgress.SetPosition(0, 30)->SetSize(100, 20);
	_hpProgress.SetBarTexture(RESOURCE("Textures/Bar/hp_bar.png"), RESOURCE("Textures/Bar/hp_back.png"));
	_hpProgress.SetMax(_charInfo.MaxHp);
	
	//Character::ActionState("NSTAND", CS_STAND, 0.0f, false);
	SetState(CS_STAND);
	animation->Play("NSTAND");

	_atkTimer = TIME_MGR->CreateTimer();

	LPDIRECT3DTEXTURE9 tex1 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/BowEffect02.png"));

	SEParam Param7;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage.push_back(EFFECT_MGR->CreateEffect("", tex1, 6, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(3, 3,3), false, &Param7)->SetLookCam(true)); //크기));

	
	//fire
	LPDIRECT3DTEXTURE9 tex3 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/explosion.png"));

	SEParam Param2;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex3, 7, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param2)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex4 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/hit02.png"));

	SEParam Param3;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param3.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param3.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex4, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param3)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex5 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/FireRing.png"));

	SEParam Param4;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param4.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param4.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param4.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param4.Scales.push_back(D3DXVECTOR3(3, 3, 3));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex5, 1, 1, pTransform,
		D3DXVECTOR3(0, 1, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param4)->SetLookCam(true)); //크기));




	//ice
	LPDIRECT3DTEXTURE9 tex6 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Ice.png"));

	SEParam Param5;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex6, 5, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param5)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex7 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/HitEffect.png"));

	SEParam Param6;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param6.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param6.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex7, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param6)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex8 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/outside.png"));

	SEParam Param8;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param8.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param8.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param8.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param8.Scales.push_back(D3DXVECTOR3(3, 3, 3));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex8, 1, 1, pTransform,
		D3DXVECTOR3(0, 1, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param8)->SetLookCam(true)); //크기));



	//Thunder
	LPDIRECT3DTEXTURE9 tex9 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Thunder.png"));

	SEParam Param10;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex9, 6, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param10)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex10 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Lightning.png"));

	SEParam Param11;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param11.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param11.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex10, 25, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 2, 1), false, &Param11)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex11 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Sword2.png"));

	SEParam Param12;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param12.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param12.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param12.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param12.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex11, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param12)->SetLookCam(true)); //크기));


}

void Monster::BaseObjectEnable()
{

}

void Monster::BaseObjectDisable()
{
	//Character::ActionState("DOWN", CS_KNOCK, 0.0f, true);
}

void Monster::BaseObjectUpdate(float timeDelta)
{
	Character::BaseObjectUpdate(timeDelta);

	D3DXVECTOR3 selfPos = pTransform->GetWorldPosition();

	_MonsterDamage.Update(timeDelta);
	_MonsterDamage2.Update(timeDelta);
	_MonsterDamage3.Update(timeDelta);
	_MonsterDamage4.Update(timeDelta);
	
	if(_hitStopTime < 0.1) AIUpdate(timeDelta);

	if (!IsState(Character::CS_DOWN))
	{
		if (GetCharacterInfo().Hp <= 0) Down();

		//네임태그 셋
		D3DXVECTOR3 TagPos = selfPos;
		TagPos.y += 3;
		_tag.SetPosition(TagPos);
		_tag.Look(CAMERA->GetWorldPosition(), D3DXVECTOR3(0, 1, 0));

		_hpProgress.SetCurrent(_charInfo.Hp);
	}
	else
	{
		_respawnTime -= timeDelta;
		
		if (_respawnTime <= 0) Alive();
	}
}

void Monster::BaseObjectNoActiveUpdate(float timeDelta)
{
	this->animation->Update(timeDelta);
}

void Monster::BaseObjectRender()
{
	if (!CAMERA->Frustum.IsInFrustum(this)) return;

	_MonsterDamage.Render();
	_MonsterDamage2.Render();
	_MonsterDamage3.Render();
	_MonsterDamage4.Render();


	this->animation->Render(this->pTransform);
	//static_cast<cXMesh_Skinned*>(pMesh)->ShowAnimationName(0, 100);
	//animation->RenderBoneName(CAMERA, this->pTransform);
	cBoundBox atkBox;
	atkBox.SetBound(&D3DXVECTOR3(0, 1.5f, 0), &D3DXVECTOR3(1, 1, _charInfo.AtkScale * 2));
	atkBox.RenderGizmo(pTransform);
}

void Monster::AIUpdate(float timeDelta)
{
	this->animation->Update(timeDelta);
	if (IsState(CS_KNOCK))
	{
		if (!animation->IsPlay())
		{
			SetState(CS_NONE);
			ActionState("BSTAND", Character::CS_STAND, 0, false);
			_battleState = Character::CBS_NORMAL;
		}
	}
	else
	{
		switch (_battleState)
		{
		case Character::CBS_BATTLE:
			AIBattleUpdate(timeDelta);
			break;
		case Character::CBS_BACK:
			AIBackUpdate(timeDelta);
			break;
		case Character::CBS_NORMAL: default:
			AINormalUpdate(timeDelta);
			break;
		}
	}
}

void Monster::AIBattleUpdate(float timeDelta)
{
	if (!IsState(Character::CS_DOWN))
	{
		if (IsState(Character::CS_ATTACK))
		{
			if (!animation->IsPlay())
			{
				SetState(Character::CS_STAND);
				_AttackTime = _charInfo.AtkSpeed;
			}

			//플레이어 충돌처리
			//if (!_isAtkPlayer)
			//{
			//	PLAYER_MGR->MonsterCollision(this);
//
			//	_isAtkPlayer = true;
			//}

			AttackUpdate(_attackNum);
		}
		else
		{
			D3DXVECTOR3 temp = _DestPos = PlayerTrans->GetWorldPosition();
			temp.y = pTransform->GetWorldPosition().y;
			LookPosition(temp);

			if (IsDestPos(3))
			{
				SetState(Character::CS_NONE);
				ActionState("BSTAND", Character::CS_STAND, 0, false);

				if (_AttackTime <= 0)
				{
					if (RandomFloatRange(0, 100) < 30)
					{
						ActionState("ATK02", Character::CS_ATTACK, 0, true);
						_attackNum = 1;
					}
					else
					{
						ActionState("ATK01", Character::CS_ATTACK, 0, true);//SetState(Character::CS_ATTACK);
						_attackNum = 2;
					}
					_isAtkPlayer = false;

					_atkTimer->Start(INT_MAX);
				}
				else
				{
					_attackNum = 0;
				}
			}
			else
			{
				ActionState("RUN", Character::CS_RUN, 0, false);
				MoveDestPosition(_charInfo.WalkSpeed * timeDelta);
			}

			if (!IsPosInDistance(_PrevPos, 20.0f))
			{
				_DestPos = _PrevPos;
				_battleState = Character::CBS_BACK;
			}

			if (_AttackTime > 0)
			{
				_AttackTime = _AttackTime - timeDelta;
			}
		}
	}
}

void Monster::AINormalUpdate(float timeDelta)
{
	MoveUpdate(timeDelta);

	if (IsPosInDistance(PlayerTrans->GetWorldPosition(), _charInfo.AtkRange))
	{
		_battleState = Character::CBS_BATTLE;
		_PrevPos = pTransform->GetWorldPosition();
	}
}
void Monster::AIBackUpdate(float timeDelta)			
{
	ActionState("RUN", Character::CS_RUN, 0, false);
	
	LookPosition(_DestPos);
	MoveDestPosition(_charInfo.RunSpeed * timeDelta);

	if (IsDestPos(1.0f))
	{
		SetState(Character::CS_NONE);
		_StandTime = RandomFloatRange(5, 20);
		ActionState("NSTAND", Character::CS_STAND, 0, false);

		_battleState = Character::CBS_NORMAL;
	}
}

void Monster::MoveUpdate(float timeDelta)
{
	//서 있는 상태일때
	if (IsState(Character::CS_STAND))
	{
		//서 있는게 끝나서 이동할 위치랑 방향 설정해주고 이동 상태로 변경
		if (_StandTime < 0)
		{
			_DestPos.x = RandomFloatRange(_Area.left, _Area.right);
			_DestPos.z = RandomFloatRange(_Area.top, _Area.bottom);
			_DestPos.y = 0;

			LookPosition(_DestPos);
			ActionState("WALK", Character::CS_RUN, 0, false);

		}
		//서 있는 중
		else
		{
			_StandTime = _StandTime - timeDelta;
		}
	}
	//이동하는 상태일 때
	else if (IsState(Character::CS_RUN))
	{
		//목적지에 도달하면 서있는 시간을 셋팅 해주고 서있는 상태로 변경
		if (IsPosInDistance(_DestPos, 1.0f))
		{
			SetState(Character::CS_NONE);
			_StandTime = RandomFloatRange(5, 20);
			ActionState("NSTAND", Character::CS_STAND, 0.2f, false);
		}
		else
		{
			MoveDestPosition(_charInfo.WalkSpeed * timeDelta);
		}
	}
}

D3DXVECTOR3 Monster::GetDeltaDestPos(float speed)
{
	D3DXVECTOR3 direction, temp = pTransform->GetWorldPosition();// = _DestPos - pTransform->GetWorldPosition();
	temp.y = _DestPos.y;
	D3DXVec3Normalize(&direction, &(_DestPos - temp));
	return direction * speed;
}

void Monster::LookPosition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	D3DXVECTOR3 tempDest = pos;
	tempDest.y = temp.y;
	D3DXVECTOR3 direction = temp - tempDest;
	D3DXVec3Normalize(&direction, &direction);
	pTransform->LookDirection(direction);
}

void Monster::MoveDestPosition(float speed)
{
	pTransform->MovePositionWorld(GetDeltaDestPos(speed));
}

bool Monster::IsDestPos(float distance)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	_DestPos.y = 0;
	temp.y = 0;
	return D3DXVec3Length(&(temp - _DestPos)) < distance;
}

bool Monster::IsPosInDistance(D3DXVECTOR3 destPos, float distance)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	//destPos.y = 0;
	temp.y = destPos.y;
	return D3DXVec3Length(&(temp - destPos)) < distance;
}

void Monster::AttackUpdate(UINT atkNum)
{
	if (!_isAtkPlayer)
	{
		PLAYER_MGR->MonsterCollision(this);
	
		_isAtkPlayer = true;
	}
}

float Monster::GetPlayerDistance()
{
	if (!PlayerTrans) return 0;

	return D3DXVec3Length(&(pTransform->GetWorldPosition() - PlayerTrans->GetWorldPosition()));
}

bool Monster::GetAttackTransform(OUT Transform ** transArr, OUT int * arrLength)
{
	ZeroMemory(transArr, sizeof(Transform*));
	//transArr[0] = new Transform;
	transArr[0] = pTransform;

	//animation->AddBoneTransform("Bip01_Spine1", transArr[0]);

	if (arrLength) *arrLength = 1;

	return true;
}

bool Monster::GetHitTransform(OUT Transform ** transArr, OUT int * arrLength)
{
	ZeroMemory(transArr, sizeof(Transform*));
	transArr[0] = new Transform;

	animation->AddBoneTransform("Bip01_Spine1", transArr[0]);

	if(arrLength) *arrLength = 1;

	return true;
}

void Monster::Hit(float damage)
{
	float totalDamage = max(0, damage - _charInfo.Def);
	
	_charInfo.Hp -= totalDamage;
	if (PLAYER_MGR->GetPlayer()->GetIsBuffState() == PB_FIRE)
		_MonsterDamage2.Start(0.3, false);
	else if (PLAYER_MGR->GetPlayer()->GetIsBuffState() == PB_ICE)
		_MonsterDamage3.Start(0.3, false);
	else if (PLAYER_MGR->GetPlayer()->GetIsBuffState() == PB_THUNDER)
		_MonsterDamage4.Start(0.3, false);

	//_MonsterDamage3.Start(0.3, false);
	if (!IsState(CS_ATTACK) && RandomFloatRange(0, 100) < 30)
	{
		ActionState("HIT", CS_KNOCK, 0, true);
	}

	_hitStopTime = 0.2f;
}

void Monster::SetArea(RECT rect)
{
	_Area = rect;
}

void Monster::Down()
{
	ActionState("DOWN", CS_DOWN, 0, true);
	
	MONSTER_MGR->AddDeadMonster(this);

	QuestManager::GetInstance()->SetTargetName(_name);

	PLAYER_MGR->GetPlayer()->AddExp(_charInfo.Level);
}

void Monster::Alive()
{
	SetState(Character::CS_NONE);
	ActionState("NSTAND", CS_STAND, 0, true);

	_DestPos.x = RandomFloatRange(_Area.left, _Area.right);
	_DestPos.z = RandomFloatRange(_Area.top, _Area.bottom);
	_DestPos.y = 0;

	_charInfo.Hp = _charInfo.MaxHp;
	_respawnTime = _charInfo.RespwanTime;

	pTransform->SetWorldPosition(_DestPos);

	_battleState = Character::CBS_NORMAL;
		
	_AttackTime = 0;

	MONSTER_MGR->EraseDeadMonster(this);
}

void Monster::PrintNameTag()
{
	if (!CAMERA->Frustum.IsInFrustum(this)) return;
	//네임태그용
	_tagTexture.Begin(); //네임태그 텍스쳐로 렌더 타겟 변경 후 그림

	_hpProgress.Render(SPRITE);
	RECT rc = { 0, 0, 100, 30 };
	LPD3DXFONT font = DXFONT_MGR->GetStyle("");
	font->DrawText(NULL, _name.c_str(), _name.size(), &rc, DT_CENTER | DT_TOP | DT_WORDBREAK, 0xffffffff);

	_tagTexture.End(); //렌더 타겟 원상태로 복구

					   //플레이어 거리를 구하고 거리에 따라 알파값 적용
	float distance = D3DXVec3Length(&(PlayerTrans->GetWorldPosition() - pTransform->GetWorldPosition()));
	float alpha = 1 - MyUtil::Clamp01((distance - 20.0f) / 20.0f);
	if (alpha > 0.01)
		_tag.RenderToSprite(SPRITE, CAMERA, D3DXCOLOR(1, 1, 1, alpha));
}
