#include "stdafx.h"
#include "Boss.h"
//#include "BossAttack1.h"

Boss::Boss()
{
	_attackNum = 0;
	_shouting = false;
	SetRandomSeed();
}


Boss::~Boss()
{
	//delete _transShout;
	delete _attackTrans4;
	delete _attackTrans5;
}

void Boss::Setup(const char* path, RECT rect, CharacterInfo &info)
{
	Monster::Setup(path, rect, info);

	animation->Play("NSTAND");

	//샤우팅 셋팅
	//animation->AddBoneTransform("Bip01_Head", _transShout = new Transform);

	//바운드 박스 셋팅
	_attackBound1.SetBound(&D3DXVECTOR3(0, 0, -8), &D3DXVECTOR3(2, 6, 15));
	_attackTrans1 = pTransform;

	_attackBound2.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(3, 3, 3));
	_attackTrans21 = _hitTransform[1];
	_attackTrans22 = _hitTransform[0];

	_attackBound3.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(5, 6, 5));
	_attackTrans3 = pTransform;

	_attackBound4.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(6, 6, 6));
	animation->AddBoneTransform("Bip01_L_Hand", _attackTrans4 = new Transform);

	_attackBound5.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(5, 5, 5));
	animation->AddBoneTransform("Bip01_R_Hand", _attackTrans5 = new Transform);

	_attackBound6.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(3, 3, 3));
	_attackTrans6 = _hitTransform[0];

	_attackBound7.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(3, 3, 3));
	_attackTrans7 = _hitTransform[1];

	//이펙트
	EffectSetup();

	//타이머
	_shoutingTimer = TIME_MGR->CreateTimer();
}

void Boss::BaseObjectUpdate(float timeDelta)
{
	Monster::BaseObjectUpdate(timeDelta);
	EffectUpdate(timeDelta);
	//_effects.Update(timeDelta);
	//_effects.SetPosition(PLAYER_MGR->GetPlayer()->GetSwordTransform()->GetWorldPosition());
	//_effects.SetRotation(PLAYER_MGR->GetPlayer()->GetSwordTransform()->GetWorldRotateMatrix());
}

void Boss::BaseObjectRender()
{
	this->animation->Render(this->pTransform);
	//animation->RenderBoneName(CAMERA, this->pTransform);
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

	EffectRender();
}

void Boss::AIUpdate(float timeDelta)
{
	float timeD = timeDelta;

	if (_charInfo.Hp < _charInfo.MaxHp / 3) timeD *= 1.5;

	if (_charInfo.Hp <= 0){
		SOUND_MGR->stop("bossBgm");
		SOUND_MGR->isPlay("bgm",0.4);
	}

	Monster::AIUpdate(timeD);
}

void Boss::AIBattleUpdate(float timeDelta)
{
	if (IsState(Character::CS_DOWN)) return;

	//공격 들어가기 전에 외친다
	if (Shouting()) return;

	//공격 상태
	if (IsState(Character::CS_ATTACK))
	{
		AttackUpdate(_attackNum);//
	}
	//공격 상태가 아니면
	else
	{
		//공격 쿨타임 : 공격 상태가 아닐때 쿨타임이 돈다
		if (_AttackTime >= 0) _AttackTime -= timeDelta;

		//일단 플레이어 위치를 목적지로 잡는다.
		_DestPos = PlayerTrans->GetWorldPosition();

		////////////////////////////보스 패턴/////////////////////////////////////
		//패턴 1. 플레이어가 멀리 있으면 겉으며 공격 혹은 원거리 공격을 한다.
		//패턴 2. 플레이어가 가까이 있으면
		//2-1. 플레이어가 앞에 있을 경우 일반, 내려찍기 등 공격을 한다.
		//2-2. 플레이어가 옆이나 뒤에 있을 경우 회전 공격을 한다.
		//2-3. 방향 상관없이 일정 확률로 브레스 공격.

		if (_AttackTime < 0) //공격 쿨타임 끝났을 경우 공격 한다.
		{
			_atkTimer->Start(INT_MAX);

			//1. 플레이어가 공격 범위에 없을 경우.
			if (!IsDestPos(8)) //_charInfo.AtkRange
			{
				LookPosition(_DestPos); //일단 플레이어를 본다.

				//공격 랜덤
				if (RandomFloatRange(0, 100) < 30) //30% - 원거리 공격
				{
					ActionState("ATK02", Character::CS_ATTACK, 0.0f, true);
					_attackNum = 1;
					_AttackTime = 1;
				}
				else //70% - 이동 공격
				{
					ActionState("WALK", Character::CS_ATTACK, 0.0f, true);
					_attackNum = 2;
					_AttackTime = 1;

					//_charInfo.WalkSpeed = (D3DXVec3Length(&(pTransform->GetWorldPosition() - _DestPos)) / 20);
					//if (_charInfo.WalkSpeed > 5) _charInfo.WalkSpeed = 5;
				}
			}
			//2. 가까이 있을 경우
			else
			{
				//브레스?
				if (RandomFloatRange(0, 100) < 30) //30%
				{
					ActionState("ATK03", Character::CS_ATTACK, 0.0f, true);
					_attackNum = 3;

					_AttackTime = 1;
				}
				//브레스 아니면
				else //70%
				{
					D3DXVECTOR3 forward, look;
					D3DXVec3Normalize(&forward, &(-pTransform->GetForward()));
					D3DXVec3Normalize(&look, &(PlayerTrans->GetWorldPosition() - pTransform->GetWorldPosition()));
					//각도 보고
					float angle = D3DXVec3Dot(&forward, &look);

					if (angle > 0) //2.1 앞에 있니?
					{
						if (RandomFloatRange(0, 1) < 0.5f)
						{
							ActionState("ATK01", Character::CS_ATTACK, 0.0f, true);
							_attackNum = 4;
							_AttackTime = 1;
						}
						else
						{
							ActionState("ATK04", Character::CS_ATTACK, 0.0f, true);
							_attackNum = 5;
							_AttackTime = 1;
						}
					}
					else
					{
						D3DXVec3Normalize(&forward, &(-pTransform->GetRight()));
						float angle = D3DXVec3Dot(&forward, &look);
						//2.2 뒤에 있는데.. 좌? 우?
						if (angle > 0) //몬스터 기준 우
						{
							ActionState("RIGHT_MOVEATTACK", Character::CS_ATTACK, 0.0f, true);
							_attackNum = 6;
							_AttackTime = 1;
						}
						else //좌
						{
							ActionState("LEFT_MOVEATTACK", Character::CS_ATTACK, 0.0f, true);
							_attackNum = 7;
							_AttackTime = 1;
						}
					} 
				} //근원거리 구분
			}//atk end
		}
		else //공격 쿨타임이면 그냥 서 있는다.
		{
			SetState(Character::CS_NONE);
			ActionState("BSTAND", Character::CS_STAND, 0, false);
		}

		/*
		//공격 범위에 플레이어가 없으면 이동한다..
		if (!IsDestPos(_charInfo.AtkRange))
		{
			LookPosition(_DestPos); //목적지를 보고
			ActionState("RUN", Character::CS_RUN, 0, false); //이동 애니메이션을 취한 후
			MoveDestPosition(_charInfo.WalkSpeed * timeDelta); //이동..
		}
		//플레이어가 공격 범위에 있으면!
		else
		{
			//공격 쿨타임이 끝나면
			if (_AttackTime < 0)
			{
				AttackStart();
			}
			//공격 쿨타임이면
			else
			{
				//일단 선다.
				SetState(Character::CS_NONE);
				ActionState("BSTAND", Character::CS_STAND, 0, false);
			}
		}
		*/
		////////////////////////////보스 패턴 end/////////////////////////////////////
	}

	//활동 범위를 넘어서면 돌아간다..
	if (!IsPosInDistance(_PrevPos, 100.0f))
	{
		_DestPos = _PrevPos;
		//공격 상태 해제
		_battleState = Character::CBS_BACK;

		_shouting = false;
	}

	//LOG_F(_atkTimer->GetTime());
}

bool Boss::GetHitTransform(OUT Transform ** transArr, OUT int * arrLength)
{
	ZeroMemory(transArr, sizeof(Transform*) * 4);
	for (int i = 0; i < 4; i++)
	{
		transArr[i] = new Transform;
	}

	animation->AddBoneTransform("Bip01_R_Foot", transArr[0]);
	animation->AddBoneTransform("Bip01_L_Foot", transArr[1]);

	if (arrLength) *arrLength = 4;

	return true;
}


void Boss::MoveUpdate(float timeDelta)
{

}

void Boss::Alive()
{
	//Monster::Alive();
	_shouting = false;
}
bool Boss::Shouting()
{
	if (_shouting) return false;

	D3DXVECTOR3 pos = pTransform->GetWorldPosition();

	//샤우팅이 아닌데 애니메이션이 멈추면 끝냄
	if (!animation->IsPlay())
	{
		_shouting = true;
		CAMERA_MGR->SetPlayer();
	
		_shoutingTimer->Stop();////////////TIMER
		return false;
	}

	pos.y += 4;

	//샤우팅 시작
	if (!IsState(Character::CS_ATTACK))
	{
		LookPosition(PlayerTrans->GetWorldPosition());
		ActionState("BSTART", Character::CS_ATTACK, 0, true);
	
		//CAMERA_MGR->SetMapView();
		//MAPVIEW_CAMERA->SetPos(pos - pTransform->GetForward() * 35, pos - pTransform->GetForward() * 12.5f, 4);
		//MAPVIEW_CAMERA->SetFov(RAD(70), RAD(60), 1);
		SOUND_MGR->stop("bgm");
		SOUND_MGR->stop("ridingBgm");
		SOUND_MGR->play("bossBgm", 0.4f);
		_shoutingTimer->Start(20);////////////TIMER
	}

	pos.y += 4;
	MAPVIEW_CAMERA->SetLook(pos);

	if (_shoutingTimer->Check(10, 2.5f))
	{
		//MAPVIEW_CAMERA->SetFov(RAD(60), RAD(45), 3);
	}
	if (_shoutingTimer->Check(9, 3.0f))
	{
		//MAPVIEW_CAMERA->ShakePos(0.1, 0.05);
		//_effectShout.Start(0.5, true);
	}

	if (_shoutingTimer->Check(11, 1.5f))
	{
		SOUND_MGR->play("boss_shooting", 0.7f);
	}


	return true;
}
void Boss::AttackUpdate(UINT atkNum)
{
	float timeDelta = TIME_MGR->GetFrameDeltaSec();

	if (_charInfo.Hp < _charInfo.MaxHp / 3) timeDelta *= 1.5;

	//공격 애니메이션이 끝나면..
	if (!animation->IsPlay())
	{
		//좌우 회전 공격이면 보는 방향으로 돌려줘야 한다.
		if (_attackNum == 6)
		{
			pTransform->RotateLocal(0, RAD(90), 0);
		}
		else if (_attackNum == 7)
		{
			pTransform->RotateLocal(0, RAD(-90), 0);
		}
		//서있는 상태로 변경
		SetState(Character::CS_STAND);
		//_AttackTime = _charInfo.AtkSpeed;
		_attackNum = 0;
		_isAtkPlayer = false;
	}

	//공격 흐름 시간을 증가
	Player* player = PLAYER_MGR->GetPlayer();
	cBoundBox* pBox = &player->BoundBox;

	//공격 타임에 따라 공격!
	switch (atkNum)
	{
	case 1: //원거리 공격
		if (_atkTimer->Check(21, 2.35f))
		{
			CAMERA->ShakePos(0.35, 0.5);
			_effect[0].Start(1, false);
			//LOG("text");
			LOG_MGR->AddLog("원거리보스공격");
			SOUND_MGR->play("Attack_Ground_03", 0.2f);
			if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1)) player->Hit(500, 1.0f);
		}
		break;
	case 2: //걸으며 공격
		MoveDestPosition(_charInfo.WalkSpeed * timeDelta); //이동..
		if (_atkTimer->Check(22, 2.2f)) //첫발
		{
			CAMERA->ShakePos(0.2, 1);
			_effect[1].Start(1, false);
			D3DXVECTOR3 pos = _attackTrans21->GetWorldPosition();
			pos.y -= 1.5;
			_WalkDust->pTransform->SetWorldPosition(pos);
			_WalkDust->StartEmission();
			if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans21, &_attackBound2)) player->Hit(250, 1.0f);
			SOUND_MGR->play("Attack_Ground_03", 0.2f);
		}
		if (_atkTimer->Check(23, 4.2f)) //두번째 발
		{
			CAMERA->ShakePos(0.2, 1);
			_effect[2].Start(1, false);
			D3DXVECTOR3 pos = _attackTrans22->GetWorldPosition();
			pos.y -= 1.5;
			_WalkDust->pTransform->SetWorldPosition(pos);
			_WalkDust->StartEmission();
			if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans22, &_attackBound2)) player->Hit(250, 1.0f);
			SOUND_MGR->play("Attack_Ground_03", 0.2f);
		}
		if (_atkTimer->Check(24, 2.5f))//첫발
		{
			_WalkDust->StopEmission();
		}
		if (_atkTimer->Check(25, 4.4f))//두번째 발
		{
			_WalkDust->StopEmission();
		}
		break;
	case 3:
		for (int i = 0; i < 4; i++)
		{
			
			if (_atkTimer->Check(23 + i, 2.6f + i * 0.4f))
			{
				CAMERA->ShakePos(0.2, 1);
				_effect[3].Start(0.4, false);
				SOUND_MGR->play("fire01", 0.2f);
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans3, &_attackBound3)) player->Hit(50, 0.0f);
			}
		}
		if (_atkTimer->Check(30, 2.0f))
		{
			_PoisonCloud->StartEmission();
		}
		if (_atkTimer->Check(31, 4.0f))
		{
			_PoisonCloud->StopEmission();
		}
		break;
	case 4:
		if (_atkTimer->Check(30, 1.8f))
		{
			CAMERA->ShakePos(0.2f, 0.2f);
			_effect[4].Start(1, false);
			_effect[8].Start(0.5, false);
			_effect[9].Start(0.7, false);

			
			D3DXVECTOR3 pos = _attackTrans4->GetWorldPosition();
			pos.y -= 1;
						
			_WalkDust->pTransform->SetWorldPosition(pos);
			_WalkDust->StartEmission();
		
			if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans4, &_attackBound4))
			{
				SOUND_MGR->play("Attack_Ground_01", 0.2f);
				player->Hit(150, 0.5f);
			}
		}
		if (_atkTimer->Check(40, 2.1f))
		{
			_WalkDust->StopEmission();
		}
		break;
	case 5:
		if (_atkTimer->Check(31, 2.0f))
		{
			_effect[5].Start(1, false);
			SOUND_MGR->play("swing", 0.8f);
			LOG_MGR->AddLog("보스스윙");
			if (!_isAtkPlayer && PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans5, &_attackBound5))
			{
				player->Hit(150, 1.0f);
				_isAtkPlayer = true;
			}
		}
		break;
	case 6:
		if (_atkTimer->Check(32, 1.2f))
		{
			_effect[6].Start(0.5, false);
			D3DXVECTOR3 pos = _attackTrans6->GetWorldPosition();
			pos.y -= 1;

			_WalkDust->pTransform->SetWorldPosition(pos);
			_WalkDust->StartEmission();
			SOUND_MGR->isPlay("Attack_Ground_03");
			CAMERA->ShakePos(0.2, 1);
			if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans6, &_attackBound7))
			{
				player->Hit(150, 1.0f);
			}
		}
		if (_atkTimer->Check(36, 1.5f))
		{
			_WalkDust->StopEmission();
		}
		break;
	case 7:
		if (_atkTimer->Check(33, 1.2f))
		{
			_effect[7].Start(0.5, false);
			D3DXVECTOR3 pos = _attackTrans7->GetWorldPosition();
			pos.y -= 4;

			_WalkDust->pTransform->SetWorldPosition(pos);
			_WalkDust->StartEmission();
			SOUND_MGR->isPlay("Attack_Ground_03");
			CAMERA->ShakePos(0.2, 1);
			if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans7, &_attackBound7))
			{
				player->Hit(150, 1.0f);
			}
		}
		if (_atkTimer->Check(34, 1.5f))
		{
			_WalkDust->StopEmission();
		}
		break;
	default :
		break;
	}
}

void Boss::Attack()
{
	
}

void Boss::AttackStart()
{
	/*
	ActionState("ATK01", Character::CS_ATTACK, 0, true);
	_attackNum = 1;
	_AttackTime = 5;
	*/
}
void Boss::EffectSetup()
{
	SpriteEffect* e;
	LPDIRECT3DTEXTURE9 tex, tex2, tex3;
	D3DXVECTOR3 pos, rot, scale;
	SEParam param, param2;

	/////////////////////////////////////////////////////////1번 원거리
	param.Clear(); param2.Clear();
	param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param.Colors.push_back(D3DXCOLOR(1, 1, 1, 0.3));

	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 0.5));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 0.5));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 0.5));
	param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 0.5));

	tex = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/explosion02.png"));
	tex2 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/EarthQuake.png")); 

	for (int i = 0; i < 10; i++)
	{
		pos = D3DXVECTOR3( 0, 0.1, -2 - i * 2.5f );
		_effect[0].push_back(EFFECT_MGR->CreateEffect("", tex, 15, 1, pTransform, pos, { 0, 0, 0 }, { 5, 10, 5 }, true, &param));
		_effect[0].push_back(EFFECT_MGR->CreateEffect("", tex2, 1, 1, pTransform, pos, { RAD(90), 0, 0 }, { 4, 4, 4 }, false, &param2));
	}

	_effect[8].push_back(EFFECT_MGR->CreateEffect("", tex, 15, 1, _attackTrans4, { 0, 0, 0 }, { 0, 0, 0 }, { 5, 10, 5 }, false, &param)->SetLookCam(true));
	_effect[9].push_back(EFFECT_MGR->CreateEffect("", tex2, 1, 1, _attackTrans4, { 0, 0, 0 }, { 0, 0, 0 }, { 3, 3, 3 }, false, &param2));
	_effect[6].push_back(EFFECT_MGR->CreateEffect("", tex, 15, 1, _attackTrans6, { 0, 0, 0 }, { 0, 0, 0 }, { 5, 10, 5 }, false, &param)->SetLookCam(true));
	_effect[7].push_back(EFFECT_MGR->CreateEffect("", tex, 15, 1, _attackTrans7, { 0, 0, 0 }, { 0, 0, 0 }, { 5, 10, 5 }, false, &param)->SetLookCam(true));
	//_effect1.Stop();

	////////////////////////////////////////////////////////////2번 걷기
	param.Clear(); param2.Clear();
	param.Colors.push_back(D3DXCOLOR(1, 1, 1, 0.1f));
	param.Colors.push_back(D3DXCOLOR(1, 1, 1, 0));
	param.Scales.push_back(D3DXVECTOR3(1, 1, 1) * 0);
	param.Scales.push_back(D3DXVECTOR3(1, 1, 1) * 10);
	tex = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Dust03.png"));

	//왼발
	_effect[1].push_back(e = EFFECT_MGR->CreateEffect("boss walk left sprite", tex, 1, 1, _attackTrans21, false, &param)->SetLookCam(true));

	//오른발
	_effect[2].push_back(e = EFFECT_MGR->CreateEffect("boss walk right sprite", tex, 1, 1, _attackTrans22, false, &param)->SetLookCam(true));

	_WalkDust = new Boss_Around_Dust;
	_WalkDust->Setup();
	_WalkDust->SetActive(true);

	//_//StonePT = new BossStonePT;
	//_StonePT->Setup();
	//_StonePT->SetActive(true);

	_PoisonCloud = new BossPoisonCloud;
	_PoisonCloud->Setup();
	_PoisonCloud->SetActive(true);

	animation->AddBoneTransform("Bip01_Head", _PoisonCloud->pTransform);


	LPDIRECT3DTEXTURE9 tex11 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/BowEffect02.png"));

	SEParam Param17;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage.push_back(EFFECT_MGR->CreateEffect("", tex11, 6, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(3, 3, 3), false, &Param17)->SetLookCam(true)); //크기));


	//fire
	LPDIRECT3DTEXTURE9 tex13 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/explosion.png"));

	SEParam Param12;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex13, 7, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param12)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex14 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/hit02.png"));

	SEParam Param13;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param13.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param13.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex14, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param13)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex15 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/FireRing.png"));

	SEParam Param14;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param14.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param14.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param14.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param14.Scales.push_back(D3DXVECTOR3(3, 3, 3));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex15, 1, 1, pTransform,
		D3DXVECTOR3(0, 1, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param14)->SetLookCam(true)); //크기));




	//ice
	LPDIRECT3DTEXTURE9 tex16 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Ice.png"));

	SEParam Param15;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex16, 5, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param15)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex17 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/HitEffect.png"));

	SEParam Param16;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param16.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param16.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex17, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param16)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex18 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/outside.png"));

	SEParam Param18;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param18.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param18.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param18.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param18.Scales.push_back(D3DXVECTOR3(3, 3, 3));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex18, 1, 1, pTransform,
		D3DXVECTOR3(0, 1, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param18)->SetLookCam(true)); //크기));



	//Thunder
	LPDIRECT3DTEXTURE9 tex19 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Thunder.png"));

	SEParam Param20;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex19, 6, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param20)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex20 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Lightning.png"));

	SEParam Param21;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param21.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param21.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex20, 25, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 2, 1), false, &Param21)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex21 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Sword2.png"));

	SEParam Param22;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param22.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param22.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param22.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param22.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex21, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param22)->SetLookCam(true)); //크기));


	
}
void Boss::EffectUpdate(float timeDelta)
{
	for (int i = 0; i < 8; i++)
	{
		_effect[i].Update(timeDelta);
	}
	_WalkDust->Update(timeDelta);
	//_StonePT->Update(timeDelta);
	_PoisonCloud->Update(timeDelta);
	_effect[8].Update(timeDelta);
	_effect[9].Update(timeDelta);



	_MonsterDamage.Update(timeDelta);
	_MonsterDamage2.Update(timeDelta);
	_MonsterDamage3.Update(timeDelta);
	_MonsterDamage4.Update(timeDelta);
}
void Boss::EffectRender()
{
	for (int i = 0; i < 8; i++)
	{
		_effect[i].Render();
	}
	_WalkDust->Render();
	//_StonePT->Render();
	_effect[8].Render();
	_effect[9].Render();
	_PoisonCloud->Render();



	_MonsterDamage.Render();
	_MonsterDamage2.Render();
	_MonsterDamage3.Render();
	_MonsterDamage4.Render();
}
//ATK01 ~ 04
//RIGHT_MOVEATTACK
//LEFT
//N, BSTAND
//WALK
//RUN
//HIT
//DOWN
//BSTART

void Boss::Hit(float damage)
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