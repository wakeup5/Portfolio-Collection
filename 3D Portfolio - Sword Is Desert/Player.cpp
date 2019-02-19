#include "stdafx.h"
#include "Player.h"
#include "ActionPlayer.h"
#include "Skill.h"
#include "Monster.h"
#include "UIWindow.h"
#include "SkillManager.h"
#include "Equipment.h"

Player::Player()
{
	_dist = 5;
	_timer = TIME_MGR->CreateTimer();
}


Player::~Player()
{
	for (auto iter = _skills.begin(); iter != _skills.end(); iter++)
	{
		delete *iter;
	}

	_skills.clear();
}

void Player::Setup()
{
	this->pMesh = RESOURCE_SKINNEDXMESH->LoadResource("../Resources/meshes/player/MAINCHARACTER.x");
	this->pTransform->SetWorldPosition(0, 0, 0);

	this->animation = new cSkinnedAnimation;
	animation->Init((cXMesh_Skinned*)this->pMesh);
	this->pTransform->SetScale(0.05, 0.05, 0.05);

	animation->Play("NSTAND");

	ActionPlayer::SetPlayer(this);

	rootBoneTransform = new Transform;
	rootBoneTransform->SetScale(0.05, 0.05, 0.05);
	this->animation->AddBoneTransform("Scene_Root", rootBoneTransform);

	_state = PS_STAND;
	_battleState = CBS_NORMAL;

	//능력치 셋팅
	CharacterInfo info(1000, 1000, 1, 10, 0, 0, 0, 0);
	info.Exp = 0;
	info.Level = 1;
	SetCharacterInfo(info);

	_charInfo.Hp = 500;

	//바운드 박스 셋팅
	GetAttackTransform(_attackTransform, NULL);
	GetHitTransform(_hitTransform, NULL);

	BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 2, 1));

	_vehicle = new vehicle;
	_vehicle->Setup();
	_vehicle->SetActive(false);


	EffectSetup();

	_bowAttack = TIME_MGR->CreateTimer();
	_bowBound.SetBound(&D3DXVECTOR3(0, 20, -80), &D3DXVECTOR3(80, 40, 120));

	_StormBound.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 2, 1));
	_StormBound2.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 2, 1));

}

void Player::BaseObjectEnable()
{

}

void Player::BaseObjectDisable()
{

}

void Player::BaseObjectUpdate(float timeDelta)
{
	animation->Update(timeDelta);
	EffectUpdate(timeDelta);
	_vehicle->BaseObjectUpdate(timeDelta, this->pTransform);
	

	if (!IsState(PS_KNOCKDOWN))
	{
		//키 버퍼 입력
		DetectKeyAction('W', PIK_FRONT);
		DetectKeyAction('S', PIK_BACK);
		DetectKeyAction('D', PIK_RIGHT);
		DetectKeyAction('A', PIK_LEFT);
		if (!UIWindow::GetRoot()->IsMouseOver())
		{
			DetectKeyAction(VK_LBUTTON, PIK_MAIN_ATTACK);
			DetectKeyAction(VK_RBUTTON, PIK_SUB_ATTACK);
		}
		PopKeyBuffer();

		if (KEY_MGR->IsOnceDown(VK_F3))
		{
			if (_vehicle->IsActive())
			{
				if (SOUND_MGR->isBossBgm()){
					SOUND_MGR->stop("ridingBgm");
					SOUND_MGR->isPlay("bossBgm", 0.4f);
				}
				else{
					SOUND_MGR->stop("ridingBgm");
					SOUND_MGR->play("bgm", 0.4f);
				}
				
				SetState(PS_NONE);
				_vehicle->SetActive(false);
			}
			else
			{
				if (SOUND_MGR->isBossBgm()){
					SOUND_MGR->stop("bossBgm");
					
				}
				else{
					SOUND_MGR->stop("bgm");
				}
				
				SOUND_MGR->play("ridingBgm",0.4f);
				SOUND_MGR->play("horseRiding",0.4f);
				Ride();
				_RideFX.Start(0.3, false);
			}
		}

		Command();
		BasicCommand();

		//애니메이션 액션 업데이트
		if (_action)
			_action->Update();
	}

	if (IsState(PS_KNOCKBACK) && !animation->IsPlay())
	{
		Stand(0, true);
	}

	if (_charInfo.Hp <= 0)
	{
		SetState(PS_KNOCKDOWN);
		Down();
	}
	if (IsState(PS_STAND))
	{
		_Storm->StopEmission();
		_Storm2->StopEmission();
		_StormPt->StopEmission();
		_StormPt2->StopEmission();
		_StormDust->StopEmission();
		_StormDust2->StopEmission();
	}

	//자연 회복
	if (_charInfo.Hp < GetTotalStatus().MaxHp && _charInfo.Hp > 0) 
		_charInfo.Hp += 10.0f * timeDelta;

	//높이 설정
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	pTransform->SetWorldPosition(temp.x, (IsState(PS_JUMP) ? _jumpHeight : _height), temp.z);

	//버프 입력 키
	if (KEY_MGR->IsOnceDown(VK_F5)){ 
		Buff = PB_NONE; 
		_SwordFX.Stop();
		_SwordFX2.Stop();
		_SwordFX3.Stop();
	}
	if (KEY_MGR->IsOnceDown(VK_F6)){
		SOUND_MGR->isPlay("buff", 0.2f);
		Buff = PB_FIRE;
		_SwordFX.Start(1.0,true);
		_SwordFX2.Stop();
		_SwordFX3.Stop();
	}
	if (KEY_MGR->IsOnceDown(VK_F7)){ 
		SOUND_MGR->isPlay("buff", 0.2f);
		Buff = PB_ICE;
		_SwordFX.Stop();
		_SwordFX2.Start(1.0f,true);
		_SwordFX3.Stop();
	}
	if (KEY_MGR->IsOnceDown(VK_F8)){ 
		SOUND_MGR->isPlay("buff", 0.2f);
		Buff = PB_THUNDER; 
		_SwordFX.Stop();
		_SwordFX2.Stop();
		_SwordFX3.Start(1.0f,true);
	}
}

void Player::BaseObjectNoActiveUpdate(float timeDelte)
{

}

void Player::BaseObjectRender()
{
	animation->Render(this->pTransform);
	EffectRender();
	_vehicle->Render();

	//animation->RenderBoneName(CAMERA, this->pTransform);
	
	//static_cast<cXMesh_Skinned*>(pMesh)->ShowAnimationName(0, 100);
	//*/
	BoundBox.RenderGizmo(rootBoneTransform);

	//DXFONT_MGR->PrintText(std::to_string(_keyBuffer.size()), 10, 80, 0xffffffff);

	_bowBound.RenderGizmo(pTransform);
	_StormBound.RenderGizmo(_Storm->pTransform);
	_StormBound2.RenderGizmo(_Storm2->pTransform);
}

void Player::SetHeight(float height)
{
	_height = height;
}

void Player::DetectKeyAction(const UINT key, PLAYER_INPUT_KEY action)
{
	//ui클릭 막아둠
	if (key == VK_LBUTTON && UIWindow::GetRoot()->IsMouseOver())
	{
		return;
	}

	if (KEY_MGR->IsOnceDown(key))
	{
		_keyBuffer.push_back({ action, TIME_MGR->GetLastTime(), key });
	}
	
	if (_keyOrder.find(action) == _keyOrder.end())
		_keyOrder.insert(std::make_pair(action, key));
}

void Player::PopKeyBuffer()
{
	if (_keyBuffer.size() == 0) return;

	if (_keyBuffer.front().time + _keyDelay < TIME_MGR->GetLastTime())
	{
		_keyBuffer.erase(_keyBuffer.begin());
	}
}

bool Player::ActionToCommand(std::vector<KeyType> &command, Action *action)
{
	int i = 0;

	for (; i < command.size(); i++)
	{
		if (command[i].ons != 1) break;

		if (!ActionStay(command[i])) return false;
	}

	for (int j = 0; i < command.size(); i++, j++)
	{
		if (!ActionOnce(command[i], j)) return false;
	}

	if (action)
	{
		this->SetAction(action);
		action->Start();
	}

	return true;
}

bool Player::ActionStay(KeyType &key)
{
	if (_keyOrder[key.key] == VK_LBUTTON && UIWindow::GetRoot()->IsMouseOver())
		return false;

	return KEY_MGR->IsStayDown(_keyOrder[key.key]);
}

bool Player::ActionOnce(KeyType &key, int num)
{
	if (_keyBuffer.size() <= num) return false;

	return _keyBuffer[num].key == key.key;
}

void Player::BasicCommand()
{
	if (IsState(PS_RIDE))
	{
		RideMoveCommand();
		return;
	}
	
	JumpCommand();
	AttackCommand();
	MoveCommand();
	BowAttackCommand();
}

void Player::JumpCommand()
{	
	

	if (!IsState(PS_JUMP))
	{
		if (KEY_MGR->IsOnceDown(VK_SPACE))
		{
			_timer->Start(INT_MAX);
			_jumpSpeed = 15;
			_jumpHeight += _jumpSpeed * TIME_MGR->GetFrameDeltaSec();
			animation->Stop();
			JumpStart(0);
			SOUND_MGR->play("보이스1", 0.2f);
			if (KEY_MGR->IsStayDown('W') || KEY_MGR->IsStayDown('D') || KEY_MGR->IsStayDown('A'))
			{
				if (KEY_MGR->IsStayDown(VK_LSHIFT))
					_jumpMoveSpeed = 10;
				else
					_jumpMoveSpeed = 7;
			}
			else
			{
				_jumpMoveSpeed = 0;
			}

			return;
		}
	}
	else
	{
		//점프
		switch (_jumpState)
		{
		case PJS_END: default:
			if (_state == PS_JUMP && !IsPlayAnimation()) Stand(0, true);
			break;
		case PJS_START:
			if (_state == PS_JUMP && !IsPlayAnimation()) Jumping(0);
			break;
		case PJS_JUMPING:
			if (_state == PS_JUMP && _jumpHeight <= _height) JumpEnd(0.0f);
			break;
		}
	}

	//점프 업데이트
	if (!IsState(PS_JUMP))
	{
		_jumpSpeed = 0;
		_jumpHeight = _height;
		
			_JumpFX->StopEmission();
		
		return;
	}

	if (_jumpHeight > _height)
	{
		_jumpSpeed -= 20 * TIME_MGR->GetFrameDeltaSec();
		_jumpHeight += _jumpSpeed * TIME_MGR->GetFrameDeltaSec();

		pTransform->MovePositionWorld(-pTransform->GetForward() * _jumpMoveSpeed * TIME_MGR->GetFrameDeltaSec());
	}
	else
	{
		_jumpSpeed = 0;
		_jumpHeight = _height;
		JumpEnd(0.0f);
		_JumpFX->StartEmission();
		
		
	}
}
void Player::AttackCommand()
{
	//태세 변환
	if (IsState(PS_STAND) && KEY_MGR->IsOnceDown(VK_TAB))
	{
		_battleState = (Character::BattleState)!_battleState;
		_keyBuffer.clear();
		
		if (_battleState == CBS_NORMAL)
		{
			animation->Play("NSTAND", 0.2f);
		}
		else
		{
			animation->Play("BSTAND", 0.2f);
		}
	}
}
void Player::MoveCommand()
{
	if (_state > PS_BACKWALK) return;

	bool isMove = false, isMoveBack = false;

	float rotY = 0;
	static float tempRot = 0;

	D3DXVECTOR3 direct(0, 0, 1);

	//앞으로
	if (KEY_MGR->IsStayDown('W'))
	{
		isMove = true;
		_effects.Start(0.2, false);
		if (KEY_MGR->IsStayDown('A')) rotY += RAD(-45);
		if (KEY_MGR->IsStayDown('D')) rotY += RAD(45);
	}
	//뒤로
	else if (KEY_MGR->IsStayDown('S'))
	{
		isMoveBack = true;

		if (KEY_MGR->IsStayDown('A')) rotY += RAD(45);
		if (KEY_MGR->IsStayDown('D')) rotY += RAD(-45);
	}
	//옆으로
	else if (KEY_MGR->IsStayDown('A'))
	{
		_effects.Start(0.2, false);
		rotY += RAD(-90);
		isMove = true;
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		_effects.Start(0.2, false);
		rotY += RAD(90);
		isMove = true;
	}

	//카메라방향
	rotY += RAD(PLAYER_CAMERA->GetY());
	tempRot = tempRot + (rotY - tempRot) / 10;

	if (isMove)
	{
		
		if (KEY_MGR->IsStayDown(VK_LSHIFT))
		{
			speed = 8;
			Sprint(0.2f);
		}
		else
		{
			speed = 5;
			Run(0.2f);
		}

		pTransform->SetRotateLocal(0, tempRot, 0);
		//if(_height < _frontHeight + 1)
		pTransform->MovePositionWorld(-pTransform->GetForward() * TIME_MGR->GetFrameDeltaSec() * speed);
	}
	else if (isMoveBack)
	{
		BackWalk(0.2f);

		pTransform->SetRotateLocal(0, tempRot, 0);
		//if (_height < _frontHeight + 1)
		pTransform->MovePositionWorld(pTransform->GetForward() * TIME_MGR->GetFrameDeltaSec() * 2.5);
	}
	else
	{
		Stand(0.2f);
	}

}
void Player::RideMoveCommand()
{
	static bool isCamerafov = false;
	bool isMove = false;
	
	float rotY = 0;
	static float tempRot = 0;

	D3DXVECTOR3 direct(0, 0, 1);

	//앞으로
	if (KEY_MGR->IsStayDown('W'))
	{
		isMove = true;
	
		_effects.Start(0.2, false); 
		if (KEY_MGR->IsStayDown('A')) rotY += RAD(-45);
		if (KEY_MGR->IsStayDown('D')) rotY += RAD(45);
		
	}
	
	//옆으로
	else if (KEY_MGR->IsStayDown('A'))
	{
		_effects.Start(0.2, false);
		rotY += RAD(-90);
		isMove = true;
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		_effects.Start(0.2, false);
		rotY += RAD(90);
		isMove = true;
	}
		
	if (KEY_MGR->IsOnceDown(VK_SPACE))
	{
		_vehicle->SetisJump(true);
	}
	if (_vehicle->GetisJump())
	{
		animation->Play("RIDE_JUMP");
	}
	
	

	//카메라방향
	rotY += RAD(PLAYER_CAMERA->GetY());
	tempRot = tempRot + (rotY - tempRot) / 10;
		

	if (isMove)
	{
		
		float speed = 0;
		if (!_vehicle->GetisJump())
		animation->Play("RIDE");
		_vehicle->SetisMove(true);
		SOUND_MGR->isPlay("move");
		if (KEY_MGR->IsStayDown(VK_LSHIFT))
		{
			speed = 120;
		}
		else
		{
			speed = 8;
		}

		pTransform->SetRotateLocal(0, tempRot, 0);
		pTransform->MovePositionWorld(-pTransform->GetForward() * TIME_MGR->GetFrameDeltaSec() * speed);
	}

	else 
	{
		
		if (!_vehicle->GetisJump())
		animation->Play("RIDE_STAND");
		_vehicle->SetisMove(false);
	
	}

	
	if (isMove)
	{
		if (isCamerafov)
		{
			//PLAYER_CAMERA->SetFov(RAD(90), RAD(90), 0);
		}
		else
		{
			PLAYER_CAMERA->SetFov(RAD(60), RAD(90), 1, RAD(90));
			isCamerafov = true;
			
		}

	}
	else
	{
		if (isCamerafov)
		{
			PLAYER_CAMERA->SetFov(RAD(90), RAD(60), 1, RAD(60));
			isCamerafov = false;
		}
		
	}
		
}
void Player::RideJump()
{
	
}

void Player::BowAttackCommand()
{
	if (_bowAttack->End())
	{
		float damage = PLAYER_MGR->GetPlayerTotalStatus().Atk / 2;
		MONSTER_MGR->PlayerCollision(this, damage, pTransform, _bowBound);
		//_bowAttack->Stop();

		LOG_F(_bowAttack->GetTime());
		//Stand(0, true);
	}

	if (IsState(PS_BOWATTACK) && !IsPlayAnimation())
	{
		Stand(0.2f, true);
		return;
	}

	if (KEY_MGR->IsStayDown('F'))
	{
		if (!CanState(PS_BOWATTACK)) return;

		SOUND_MGR->play("bow", 0.2f);
		pTransform->SetRotateWorld(0, RAD(PLAYER_CAMERA->GetY()), 0);
		animation->Stop();
		BowAttack();

		_bowAttack->Start(0.35);

		_hitMonster.clear();
	}
}

bool Player::Command()
{
	std::vector<Skill*> skills = SkillManager::GetInstance()->GetSkills();

	for (int i = 0; i < skills.size(); i++)
	{
		if(_state < (skills[i]->GetState()))
		ActionToCommand(skills[i]->GetCommand(), skills[i]->GetAction());
	}

	return false;
}

bool Player::SetState(PLAYER_STATE state)
{
	bool result = IsState(state);

	_state = state;

	return result;
}

bool Player::IsState(PLAYER_STATE state)
{
	return (_state == state);
}

bool Player::CanState(PLAYER_STATE state)
{
	switch (state)
	{
	case PS_STAND: //서있는 상태거나 아무 상태가 아닐때 가능
		if (_state <= PS_BACKWALK && _state != PS_STAND) return true;
		break;
	case PS_RUN: //달리는 상태 이하 가능
		SOUND_MGR->isPlay("foot_step_normal01");
		if (_state < PS_BACKWALK && _state != PS_RUN) return true;
		
		break;
	case PS_SPRINT: //전력질주 상태 이하 가능
		SOUND_MGR->stop("foot_step_normal01");
		SOUND_MGR->isPlay("FootStep_Dash_01");
		if (_state < PS_BACKWALK && _state != PS_SPRINT) return true;
		break;
	case PS_BACKWALK:
		SOUND_MGR->isPlay("foot_step_normal01");
		if (_state < PS_BACKWALK) return true;
		break;
	case PS_ROLLING: // 점프, 넉백, 넉다운, 라이드 상태 이외 가능
		if (_state != PS_KNOCKBACK &&
			_state != PS_KNOCKDOWN &&
			_state != PS_RIDE &&
			_state != PS_JUMP) return true;
		break;
	case PS_JUMP: //뒷 걸음 이하 가능
		if (_state <= PS_BACKWALK) return true;
		break;
	case PS_ATTACK: //어택 상태가 아니고, 활 공격 미만 가능
		if (_state != PS_ATTACK &&
			_state < PS_BOWATTACK) return true;
		break;
	case PS_WALKATTACK: //전진 공격 상태가 아니고 , 활공격 미만 가능
		if (_state != PS_WALKATTACK &&
			_state < PS_BOWATTACK) return true;
		break;
	case PS_SIDEATTACK:
		if (_state != PS_SIDEATTACK &&
			_state < PS_BOWATTACK) return true;
		break;
	case PS_SHIELDATTACK:
		if (_state != PS_SHIELDATTACK &&
			_state < PS_BOWATTACK) return true;
		break;
	case PS_BOWATTACK:
		if (_state < PS_BOWATTACK) return true;
		break;
	case PS_KNOCKBACK:
		if (_state < PS_KNOCKBACK) return true;
		break;
	case PS_KNOCKDOWN:
		if (_state < PS_KNOCKDOWN) return true;
		break;
	case PS_RIDE:
		if (_state <= PS_STAND && _battleState == CBS_NORMAL) return true;
		break;
	default:
		return _state <= state;
	}

	return false;
}

void Player::Stand(float fadeTime, bool mustly)
{
	if (mustly)
	{
		if (IsState(PS_STAND)) return;
	}
	else
	{
		if (!CanState(PS_STAND))
			return;
	}

	//animation->Play("BSTAND", fadeTime);
	if (_battleState == CBS_NORMAL)
	{
		animation->Play("NSTAND", fadeTime);
	}
	else
	{
		animation->Play("BSTAND", fadeTime);
	}

	SetState(PS_STAND);
}

void Player::Run(float fadeTime)
{
	if (!CanState(PS_RUN))
		return;
	
	//SOUND_MGR->play("foot_step_normal01", 0.2f);
	animation->Play("NRUN", fadeTime);

	SetState(PS_RUN);
}

void Player::Sprint(float fadeTime)
{
	if (!CanState(PS_SPRINT))
		return;

	animation->Play("BRUN", fadeTime);

	SetState(PS_SPRINT);
}

void Player::BackWalk(float fadeTime)
{
	if (!CanState(PS_BACKWALK))
		return;
	
	animation->Play("BACK_WALK", fadeTime);	

	SetState(PS_BACKWALK);
}

void Player::RollingLeft(float fadeTime)
{
	if (!CanState(PS_ROLLING))
		return;
	SOUND_MGR->play("thumble", 0.2f);
	animation->Stop();
	animation->PlayOneShotAfterHold("ROLL_LEFT", fadeTime);

	SetState(PS_ROLLING);
}

void Player::RollingRight(float fadeTime)
{
	if (!CanState(PS_ROLLING))
		return;
	SOUND_MGR->play("thumble", 0.2f);
	animation->Stop();
	animation->PlayOneShotAfterHold("ROLL_RIGHT", fadeTime);

	SetState(PS_ROLLING);
}

void Player::RollingBack(float fadeTime)
{
	if (!CanState(PS_ROLLING))
		return;

	SOUND_MGR->play("thumble", 0.2f);
	animation->Stop();
	animation->PlayOneShotAfterHold("ROLL_BACK", fadeTime);

	SetState(PS_ROLLING);
}

UINT Player::Attack(float fadeTime)
{
	if (!CanState(PS_ATTACK))
		return _attackNum;

	switch (_attackNum)
	{
	case 0: default:
		animation->PlayOneShotAfterHold("ATK_COMBO1", fadeTime);
		_attackNum = 1;
		SOUND_MGR->play("sword_hit1",0.2f);
		SOUND_MGR->play("보이스1", 0.2f);
		break;
	case 1:
		animation->PlayOneShotAfterHold("ATK_COMBO2", fadeTime);
		_attackNum = 2;
		SOUND_MGR->play("sword_hit2", 0.2f);
		SOUND_MGR->play("보이스2", 0.2f);
		break;
	case 2:
		animation->PlayOneShotAfterHold("ATK_COMBO3", fadeTime);
		_attackNum = 3;
		SOUND_MGR->play("sword_hit3", 0.2f);
		SOUND_MGR->play("보이스2", 0.2f);
		break;
	case 3:
		animation->PlayOneShotAfterHold("ATK_COMBO4", fadeTime);
		SOUND_MGR->play("sword_hit4", 0.2f);
		SOUND_MGR->play("보이스3", 0.2f);
		_attackNum = 0;
		break;
	}

	_battleState = CBS_BATTLE;
	SetState(PS_ATTACK);

	return _attackNum;
}

UINT Player::AttackStop()
{
	switch (_attackNum)
	{
	case 1: default:
		animation->Play("ATK1_STOP");
		break;
	case 2:
		animation->Play("ATK2_STOP");
		break;
	case 3:
		animation->Play("ATK3_STOP");
		break;
	case 0:
		animation->Play("ATK4_STOP");
		break;
	}

	SetState(PS_NONE);

	return _attackNum;
}

UINT Player::WalkAttack(float fadeTime)
{
	if (!CanState(PS_WALKATTACK))
		return _walkAttackNum;

	switch (_walkAttackNum)
	{
	case 0: default:
		animation->PlayOneShotAfterHold("WALK_ATTACK1", fadeTime);
		SOUND_MGR->play("sword_hit1", 0.2f);
		_walkAttackNum = 1;
		
		break;
	case 1:
		animation->PlayOneShotAfterHold("WALK_ATTACK2", fadeTime);
		SOUND_MGR->play("sword_hit2",0.2f);
		_walkAttackNum = 0;
		break;
	}

	SOUND_MGR->play("swordFire",0.2f);
	_battleState = CBS_BATTLE;
	SetState(PS_WALKATTACK);

	return _walkAttackNum;
}

UINT Player::WalkAttackStop()
{
	switch (_walkAttackNum)
	{
	case 1: default:
		animation->Play("WALK_ATTACK1_STOP");
		break;
	case 0:
		animation->Play("ATK2_STOP");
		break;
	}

	SetState(PS_NONE);

	return _walkAttackNum;
}

void Player::SideAttackLeft(float fadeTime)
{
	if (!CanState(PS_SIDEATTACK))
		return;

	animation->Stop();
	animation->PlayOneShotAfterHold("RSIDE_ATTACK", fadeTime);

	SOUND_MGR->play("sword_hit1", 0.2f);
	SOUND_MGR->play("보이스1", 0.2f); SOUND_MGR->play("sword_hit1", 0.2f);
	SOUND_MGR->play("보이스1", 0.2f);
	_battleState = CBS_BATTLE;

	SetState(PS_SIDEATTACK);
}

void Player::SideAttackRight(float fadeTime)
{
	if (!CanState(PS_SIDEATTACK))
		return;

	animation->Stop();
	animation->PlayOneShotAfterHold("LSIDE_ATTACK", fadeTime);
	SOUND_MGR->play("sword_hit1", 0.2f);
	SOUND_MGR->play("보이스1", 0.2f);
	_battleState = CBS_BATTLE;

	SetState(PS_SIDEATTACK);
}

void Player::JumpStart(float fadeTime)
{
	if (!CanState(PS_JUMP)) return;
	
	animation->PlayOneShotAfterHold("JUMP_START", fadeTime);
	_jumpState = PJS_START;

	SetState(PS_JUMP);
}

void Player::Jumping(float fadeTime)
{
	if (_jumpState != PJS_START)
		return;

	animation->Play("JUMP_LOOP", fadeTime);
	_jumpState = PJS_JUMPING;

	SetState(PS_JUMP);
}

void Player::JumpEnd(float fadeTime)
{
	//if (_jumpState != PJS_JUMPING)
	//	return;

	animation->PlayOneShotAfterHold("JUMP_END", fadeTime);
	_jumpState = PJS_END;

	//SetState(PS_JUMP);
}
void Player::ShieldAttack(float fadeTime)
{
	if (!CanState(PS_SHIELDATTACK)) 
		return;

	animation->PlayOneShotAfterHold("SHILED_ATTACK", fadeTime);
	SOUND_MGR->play("shield_hit",0.2f);
	SOUND_MGR->play("보이스1", 0.5f);
	SetState(PS_SHIELDATTACK);
}
void Player::Ultimate(float fadeTime)
{
	if (!CanState(PS_ATTACK))
		return;

	animation->PlayOneShotAfterHold("JMUP_ATTACK", fadeTime);
	SOUND_MGR->play("sword_hit1", 0.2f);
	SOUND_MGR->play("보이스1", 0.2f);
	SOUND_MGR->play("Attack_Ground_01", 0.4f);
	_battleState = CBS_BATTLE;
	SetState(PS_ATTACK);
}
void Player::BowAttack(float fadeTime)
{
	if (!CanState(PS_BOWATTACK))
		return;

	animation->PlayOneShotAfterHold("BOW", fadeTime);

	SetState(PS_BOWATTACK);
}
void Player::KnockBack(float fadeTime)
{
	animation->Stop();
	animation->PlayOneShotAfterHold("KNOCKBACK", fadeTime);

	SetState(PS_KNOCKBACK);
}
/*
void Player::Jump(float fadeTime)
{
	if (SetState(PS_JUMP))
		return;

	animation->PlayOneShotAfterHold("JUMP", fadeTime);
	LOG_MGR->AddLog("점프!");
}
*/

bool Player::IsPlayAnimation()
{
	return animation->IsPlay();
}

float Player::GetPlayTime()
{
	return animation->GetPlayTime(animation->GetPlayName());
}

void Player::SetPositionByRoot()
{
	D3DXVECTOR3 temp = rootBoneTransform->GetWorldPosition();
	temp.y = _jumpHeight;// pTransform->GetWorldPosition().y;
	pTransform->SetWorldPosition(temp);

	//SetHeight(_height);
}

bool Player::GetAttackTransform(OUT Transform ** transArr, OUT int * arrLength)
{
	//static bool isGetTransform = false;
	//if (isGetTransform) return true;

	ZeroMemory(transArr, sizeof(Transform*) * 2);

	transArr[0] = new Transform;
	transArr[1] = new Transform;

	animation->AddBoneTransform("Bip002_R_Hand", transArr[0]);
	animation->AddBoneTransform("Bip002_L_Hand", transArr[1]);

	if(arrLength) *arrLength = 2;

	//isGetTransform = true;
	return true;
}

bool Player::GetHitTransform(OUT Transform ** transArr, OUT int * arrLength)
{
	ZeroMemory(transArr, sizeof(Transform*));

	transArr[0] = new Transform;

	animation->AddBoneTransform("Bip002_Spine1", transArr[0]);

	if (arrLength) *arrLength = 1;

	return true;
}

Transform* Player::GetAttackBound()
{
	//cBoundBox result;
	
	if (!IsState(PS_SHIELDATTACK))
	{
		//result.SetBound(&_attackTransform[0]->GetWorldPosition(), &D3DXVECTOR3(1, 1, 1));
		return _attackTransform[0];
	}
	else
	{
		//result.SetBound(&_attackTransform[1]->GetWorldPosition(), &D3DXVECTOR3(1, 1, 1));
		return _attackTransform[1];
	}

	return _attackTransform[0];
}

Transform* Player::GetHitBound()
{
	//cBoundBox result;

	//result.SetBound(&_hitTransform[0]->GetWorldPosition(), &D3DXVECTOR3(1, 1, 1));
	
	//return result;
	return _hitTransform[0];
}

//맞은놈이 이미 있으면  true, 없으면 넣어주고 false
bool Player::IsAttackMonster(Monster* mon)
{
	bool result;

	result = _hitMonster.find(mon) != _hitMonster.end();

	if (!result)
	{
		_hitMonster.insert(mon);
	}

	return result;
}

void Player::ClearAttackMonster()
{
	_hitMonster.clear();
}

void Player::Down()
{
	SetState(PS_KNOCKDOWN);
	animation->Stop();
	animation->PlayOneShotAfterHold("DOWN", 0.0f);
}
void Player::Ride()
{
	SetState(PS_RIDE);
	
	_vehicle->SetActive(true);
}

void Player::Hit(float damage, float knockChance)
{
	//float dam = max(0, damage - _charInfo.Def);
	SOUND_MGR->play("hit_metal01", 0.2f); 
	_charInfo.Hp -= damage;
	_Damage.Start(0.3, false);
	_Damage2.Start(0.3, false);
	KnockBack();
	//if (RandomFloatRange(0, 100) < knockChance)
}

CharacterInfo& Player::GetTotalStatus()
{
	return _charInfo;
	/*
	if (_equip == NULL) return _charInfo;

	CharacterInfo result = _charInfo;
	ItemInfo temp = _equip->GetTotalStatus();

	result.Atk += temp.Atk;
	result.Def += temp.Def;
	result.MaxHp += temp.MaxHp;
	result.MaxMp += temp.MaxMp;

	return result;
	*/
}

float Player::GetSkillDamage()
{
	ActionPlayer* ac = static_cast<ActionPlayer*>(_action);

	if (ac)
	{
		return ac->Damage;
	}

	return 1.0f;
}

void Player::EffectSetup()
{
	
	_Storm = new Storm;
	_Storm->Setup();
	_Storm->SetActive(true);

	_Storm2 = new Storm;
	_Storm2->Setup();
	_Storm2->SetActive(true);
	
	_SideFX = new HitRing;
	_SideFX->Setup();
	_SideFX->SetActive(true);

	_Uskill04FX = new Uskill_DustFX;
	_Uskill04FX->Setup();
	_Uskill04FX->SetActive(true);

	_JumpFX = new Around_Dust;
	_JumpFX->Setup();
	_JumpFX->SetActive(true);

	_StormDust = new StormDust;
	_StormDust->Setup();
	_StormDust->SetActive(true);

	_StormDust2 = new StormDust;
	_StormDust2->Setup();
	_StormDust2->SetActive(true);

	
	_StormPt = new StormPt;
	_StormPt->Setup();
	_StormPt->SetActive(true);

	_StormPt2 = new StormPt;
	_StormPt2->Setup();
	_StormPt2->SetActive(true);

	SEParam param1;
	//param.MinSpeed = D3DXVECTOR3(0, 1, 0);
	//param.MaxSpeed = D3DXVECTOR3(0, 1, 0);
	param1.MinRot = D3DXVECTOR3(0, RAD(90), 0);
	param1.MaxRot = D3DXVECTOR3(0, RAD(90), 0);


	_SwordFX.push_back(EFFECT_MGR->CreateEffect(
		"FireSword",
		RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/fire03.png")),
		51, 1,
		PLAYER_MGR->GetPlayer()->GetSwordTransform(),
		D3DXVECTOR3(0.1, 1.1, 0.1),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1.5, 1.2, 1.5), true, &param1));
	_SwordFX.Start(1.0, true);

	_SwordFX2.push_back(EFFECT_MGR->CreateEffect(
		"FireSword2",
		RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/fire04.png")),
		51, 1,
		PLAYER_MGR->GetPlayer()->GetSwordTransform(),
		D3DXVECTOR3(0.1, 1.1, 0.1),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1.5, 1.2, 1.5), true, &param1));
	_SwordFX2.Start(1.0, true);

	_SwordFX3.push_back(EFFECT_MGR->CreateEffect(
		"FireSword3",
		RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/fire05.png")),
		51, 1,
		PLAYER_MGR->GetPlayer()->GetSwordTransform(),
		D3DXVECTOR3(0.1, 1.1, 0.1),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1.5, 1.2, 1.5), true, &param1));
	_SwordFX3.Start(1.0, true);


	LPDIRECT3DTEXTURE9 tex1 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/SWeffect02.png"));

	SEParam Param;
	Param.MinRot = D3DXVECTOR3(0, 9, 0);
	Param.MaxRot = D3DXVECTOR3(0,9, 0);
	Param.MinSpeed = D3DXVECTOR3(0, 35, 0);
	Param.MaxSpeed = D3DXVECTOR3(0, 35, 0);
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param.Scales.push_back(D3DXVECTOR3(5, 5, 5));

	_SideAtkFX.push_back(EFFECT_MGR->CreateEffect("SideAtk", tex1, 1, 1, pTransform,
		D3DXVECTOR3(0, 0, 0), //이동
		D3DXVECTOR3(RAD(90), 0, 0),  //회전
		D3DXVECTOR3(20, 20, 20), false, &Param)); //크기));

	LPDIRECT3DTEXTURE9 tex2 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/FireRing.png"));

	SEParam Param2;
	Param2.MinRot = D3DXVECTOR3(0, 0, 0);
	Param2.MaxRot = D3DXVECTOR3(0,0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param2.Colors.push_back(D3DXCOLOR(1, 1, 1, 0));
	Param2.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param2.Scales.push_back(D3DXVECTOR3(4, 4, 4));

	_MoveAtk01FX.push_back(EFFECT_MGR->CreateEffect("MoveAtk01", tex2, 1, 1, pTransform,
		D3DXVECTOR3(0, 25, 0), //이동
		D3DXVECTOR3(0, RAD(135), 0),  //회전
		D3DXVECTOR3(50, 50, 50), false, &Param2)); //크기));


	LPDIRECT3DTEXTURE9 tex3 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/EarthQuake.png"));

	SEParam Param3;
	Param3.MinRot = D3DXVECTOR3(0, 0, 0);
	Param3.MaxRot = D3DXVECTOR3(0, 0, 0);
	Param3.MinSpeed = D3DXVECTOR3(0, 0, 0);
	Param3.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(5, 5, 5));

	_UskillFX.push_back(EFFECT_MGR->CreateEffect("USkill", tex3, 1, 1, pTransform,
		D3DXVECTOR3(0, 0, 0), //이동
		D3DXVECTOR3(RAD(90), 0, 0),  //회전
		D3DXVECTOR3(80, 80, 80), false, &Param3)); //크기));


	LPDIRECT3DTEXTURE9 tex4 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/explosion02.png"));

	SEParam Param4;
	Param4.MinRot = D3DXVECTOR3(0, 0, 0);
	Param4.MaxRot = D3DXVECTOR3(0, 0, 0);
	Param4.MinSpeed = D3DXVECTOR3(0, 0, 0);
	Param4.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param4.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param4.Scales.push_back(D3DXVECTOR3(4, 4, 4));

	_Uskill02FX.push_back(EFFECT_MGR->CreateEffect("USkill2", tex4, 15, 1, pTransform,
		D3DXVECTOR3(0, 0, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(120, 120, 120), false, &Param4)); //크기));


	LPDIRECT3DTEXTURE9 tex6 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Wind.png"));

	SEParam Param6;
	//Param6.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param6.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param6.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param6.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param6.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param6.Scales.push_back(D3DXVECTOR3(4, 4, 4));

	_Uskill05FX.push_back(EFFECT_MGR->CreateEffect("USkill5", tex6, 20, 1, pTransform,
		D3DXVECTOR3(0, 3, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(50, 50, 50), false, &Param6)); //크기));


	LPDIRECT3DTEXTURE9 tex5 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/FireRing.png"));

	SEParam Param5;
	Param5.MinRot = D3DXVECTOR3(0, 0, 0);
	Param5.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param5.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param5.Colors.push_back(D3DXCOLOR(1, 1, 1, 0));
	Param5.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param5.Scales.push_back(D3DXVECTOR3(8, 8, 8));

	_Uskill03FX.push_back(EFFECT_MGR->CreateEffect("USkill3", tex5, 1, 1, pTransform,
		D3DXVECTOR3(0, 25, 0), //이동
		D3DXVECTOR3(RAD(90),0, 0),  //회전
		D3DXVECTOR3(50, 50, 50), false, &Param5)); //크기));


	LPDIRECT3DTEXTURE9 tex7 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/PC_DamageFX.png"));

	SEParam Param7;
	Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_Damage.push_back(EFFECT_MGR->CreateEffect("PC_Damage", tex7, 12, 1, pTransform,
		D3DXVECTOR3(0, 3, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(5, 5, 5), false, &Param7)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex8 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/hit02.png"));

	SEParam Param8;
	//Param8.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param8.MaxRot = D3DXVECTOR3(0, 0, 0);
	///Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param8.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param8.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param8.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param8.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_Damage2.push_back(EFFECT_MGR->CreateEffect("PC_Damage2", tex8, 1, 1, pTransform,
		D3DXVECTOR3(0, 15, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(2, 2, 2), false, &Param8)->SetLookCam(true)); //크기));
			
	_effects.push_back(EFFECT_MGR->CreateParticleEmitter<Dust>("PC_RUN"));
//	_SwordFX.push_back(EFFECT_MGR->CreateParticleEmitter<Fire>("FireFX"));
	_RideFX.push_back(EFFECT_MGR->CreateParticleEmitter<RideFX>("Ride"));
	//_JumpFX.push_back(EFFECT_MGR->CreateParticleEmitter<Around_Dust>("Jump"));
	//_UskillFX.push_back(EFFECT_MGR->CreateQuadParticleEmitter<effect_02>("Uskill"));
	//_UskillFX.push_back(EFFECT_MGR->CreateQuadParticleEmitter<PC_UFX>("Uskill02"));
//	_UskillFX.push_back(EFFECT_MGR->CreateParticleEmitter<PCU_Around_Dust>("Uskill03"));
	//_SideAtkFX.push_back(EFFECT_MGR->CreateQuadParticleEmitter<HitRing>("SideAtk"));
	_SideAtkPtFX.push_back(EFFECT_MGR->CreateParticleEmitter<Sword_effect>("SideAtkPt"));
	//_MoveAtk01FX.push_back(EFFECT_MGR->CreateQuadParticleEmitter<MoveAttackFX>("MoveAtk01FX"));
	//	_BoosFXTest.push_back(EFFECT_MGR->CreateQuadParticleEmitter<BossAtkFX>("Test"));
	_RollingFX.push_back(EFFECT_MGR->CreateParticleEmitter<RollingDust>("RollingFX"));
	_ShiledAtkFX.push_back(EFFECT_MGR->CreateQuadParticleEmitter<ShiledFX>("ShiledFX"));
	_ShiledAtkFX.push_back(EFFECT_MGR->CreateParticleEmitter<ShiledPtFX>("ShiledPtFX"));
	//_SideAtkFX.SetTransform(pTransform);
	//_UskillFX.SetTransform(pTransform);
	
	_effects.SetTransform(pTransform);
	_RideFX.SetTransform(pTransform);
	//_MoveAtk01FX.SetTransform(pTransform);
	_BoosFXTest.SetTransform(pTransform);
	_RollingFX.SetTransform(pTransform);
	_ShiledAtkFX.SetTransform(GetShieldTransform());
	_SideAtkPtFX.SetTransform(GetSwordTransform());

	_SwordFX.SetTransform(GetSwordTransform());
	_SwordFX2.SetTransform(GetSwordTransform());
	_SwordFX3.SetTransform(GetSwordTransform());
}
void Player::EffectUpdate(float timeDelta)
{
	_Storm->Update(timeDelta);
	_Storm->pTransform->MovePositionLocal(-pTransform->GetForward()*timeDelta*7);
	_Storm2->Update(timeDelta);
	_Storm2->pTransform->MovePositionLocal(-pTransform->GetForward()*timeDelta*4);
	_StormDust->Update(timeDelta);
	_StormDust->pTransform->SetWorldPosition(_Storm->pTransform->GetWorldPosition());
	_StormDust2->Update(timeDelta);
	_StormDust2->pTransform->SetWorldPosition(_Storm2->pTransform->GetWorldPosition());
	_StormPt->Update(timeDelta);
	_StormPt->pTransform->SetWorldPosition(_Storm->pTransform->GetWorldPosition());
	_StormPt2->Update(timeDelta);
	_StormPt2->pTransform->SetWorldPosition(_Storm2->pTransform->GetWorldPosition());
	_Uskill05FX.Update(timeDelta);

	//_SideFX->Update(timeDelta);
	//_SideFX->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
	//*
	_Damage.Update(timeDelta);
	_Damage2.Update(timeDelta);
	_UskillFX.Update(timeDelta);
	_Uskill02FX.Update(timeDelta);
	_Uskill03FX.Update(timeDelta);
	_Uskill04FX->Update(timeDelta);
	_Uskill04FX->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
	_effects.Update(timeDelta);
	_SwordFX.Update(timeDelta);
	_SwordFX2.Update(timeDelta);
	_SwordFX3.Update(timeDelta);
	//_RideFX.Update(timeDelta);
	_JumpFX->Update(timeDelta);
	_JumpFX->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
	_SideAtkFX.Update(timeDelta*2);
	_SideAtkPtFX.Update(timeDelta*2);
	_MoveAtk01FX.Update(timeDelta);
	_RollingFX.Update(timeDelta);
	//_ShiledAtkFX.Update(timeDelta);
	//_BoosFXTest.Update(timeDelta);
	
	//*/
}
void Player::EffectRender()
{
	//*
	_Storm->Render();
	_Storm2->Render();
	_StormDust->Render();
	_StormDust2->Render();
	//_SideFX->Render();
	_StormPt->Render();
	_StormPt2->Render();

	_effects.Render();
	_SwordFX.Render();
	_SwordFX2.Render();
	_SwordFX3.Render();
	//_RideFX.Render();
	_JumpFX->Render();
	_UskillFX.Render();
	_SideAtkFX.Render();
	_SideAtkPtFX.Render();
	_MoveAtk01FX.Render();
	_Uskill02FX.Render();
	_Uskill03FX.Render();
	_Uskill04FX->Render();
	_Uskill05FX.Render();
	_RollingFX.Render();
	_Damage.Render();
	//_Damage2.Render();
	//	_ShiledAtkFX.Render();
	//_BoosFXTest.Render();
}