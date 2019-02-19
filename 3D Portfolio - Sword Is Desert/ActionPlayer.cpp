#include "stdafx.h"
#include "ActionPlayer.h"
//#include "Player.h"

//********** ActionPlayerDeligate ***********//
void ActionPlayerDeligate::OnActionFinish()
{
	OnActionFinish(0);
}

void ActionPlayerDeligate::OnActionFinish(float cTime)
{
	if (_player)
	{
		_player->Stand(cTime, true);
		_player->SetAction(NULL);
	}
}

//********** ActionPlayer ***********//
Player* ActionPlayer::_player = NULL;
std::map<std::string, ActionPlayer*> ActionPlayer::_actionName;

ActionPlayer::~ActionPlayer()
{
	for (auto iter = _actionName.begin(); iter != _actionName.end(); iter++)
	{
		delete iter->second;
	}

	_actionName.clear();
}

void ActionPlayer::Start(void* lpvoid)
{
	_player->pTransform->SetRotateLocal(0, RAD(PLAYER_CAMERA->GetY()), 0);
	_tempPos = _object->pTransform->GetWorldPosition();

	_player->ClearAttackMonster();

	_current = 0;
	_player->SetAttackTimeState(false);

	_timer->Start(INT_MAX);
}

void ActionPlayer::Update()
{
	if (!_player->IsPlayAnimation())
	{
		if (_deligate) _deligate->OnActionFinish();
	}
}

void ActionPlayer::SetDestPosition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 temp = _player->pTransform->GetWorldPosition();
	_destPos = temp + pos;
}

ActionPlayer* ActionPlayer::GetAction(std::string name)
{
	if(_actionName.size() == 0) ReadyToActionPlayer(_player);

	auto iter = _actionName.find(name);

	if (iter == _actionName.end())
	{
		iter = _actionName.find("STANDARD");

		if (iter == _actionName.end())
		{
			ActionPlayer* ap = new ActionPlayer;
			_actionName.insert(std::make_pair("STANDARD", ap));

			return ap;
		}
	}

	return iter->second;
}

void ActionPlayer::ReadyToActionPlayer(Player* player)
{
	static bool flag = false;

	if (flag) return;
	flag = true;

	ActionPlayerDeligate* deligate = new ActionPlayerDeligate;
	deligate->SetPlayer(player);

	(new ActionPlayerAttack)->SetDeligate(deligate);
	(new ActionPlayerRollingBack)->SetDeligate(deligate);
	(new ActionPlayerRollingLeft)->SetDeligate(deligate);
	(new ActionPlayerRollingRight)->SetDeligate(deligate);
	(new ActionPlayerSideAttackLeft)->SetDeligate(deligate);
	(new ActionPlayerSideAttackRight)->SetDeligate(deligate);
	(new ActionPlayerWalkAttack)->SetDeligate(deligate);
	(new ActionPlayerSheildAttack)->SetDeligate(deligate);
	(new ActionPlayerUltimate)->SetDeligate(deligate);
}

void ActionPlayer::SetAction(std::string name)
{
	_actionName.insert(make_pair(name, this));
}

D3DXVECTOR3 ActionPlayer::GetTempPos(float s)
{
	_tempPos = _tempPos + (_destPos - _tempPos) / s;
	return _tempPos;
}

bool ActionPlayer::isTimeInRange(float start, float end)
{
	if (_current > start && _current < end) return true;
	return false;
}

void ActionPlayer::UpdatePlayerAttackTime(float start, float end)
{
	_current += TIME_MGR->GetFrameDeltaSec();

	_player->SetAttackTimeState(isTimeInRange(start, end));
}

//********** ActionPlayerRollingBack ***********//
ActionPlayerRollingBack::ActionPlayerRollingBack()
{
	ActionPlayer::SetAction("RollingBack");
}

void ActionPlayerRollingBack::Start(void * lpvoid)
{
	ActionPlayer::Start(lpvoid);
	SetDestPosition(_player->pTransform->GetForward() * 7);
	_player->RollingBack();
}

void ActionPlayerRollingBack::Update()
{
	_player->pTransform->SetWorldPosition(GetTempPos(22));
	//ActionPlayer::Update();
	if (!_player->IsPlayAnimation()) if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
}

//********** ActionPlayerRollingLeft ***********//
ActionPlayerRollingLeft::ActionPlayerRollingLeft()
{
	ActionPlayer::SetAction("RollingLeft");
}

void ActionPlayerRollingLeft::Start(void *lpvoid)
{
	ActionPlayer::Start(lpvoid);
	SOUND_MGR->play("thumble", 0.2f);
	SetDestPosition(_player->pTransform->GetRight() * 6.5);
	_player->RollingLeft();
}

void ActionPlayerRollingLeft::Update()
{
	_current += TIME_MGR->GetFrameDeltaSec();
	_player->pTransform->SetWorldPosition(GetTempPos(22));

	if (IsTimeMoment(_current, 0.5f, TIME_MGR->GetFrameDeltaSec()*1.5))
	{
		_player->_RollingFX.Start(0.5, false);
	}

	if(!_player->IsPlayAnimation())
		_player->SetPositionByRoot();
	ActionPlayer::Update();
	if (!_player->IsPlayAnimation()) if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
}

//********** ActionPlayerRollingRight ***********//
ActionPlayerRollingRight::ActionPlayerRollingRight()
{
	ActionPlayer::SetAction("RollingRight");
}

void ActionPlayerRollingRight::Start(void *lpvoid)
{
	ActionPlayer::Start(lpvoid);
	SOUND_MGR->play("thumble", 0.2f);
	SetDestPosition(-_player->pTransform->GetRight() * 6.5);
	_player->RollingRight();
	if (!_player->IsPlayAnimation()) _player->Stand(0, true);
}

void ActionPlayerRollingRight::Update()
{
	_current += TIME_MGR->GetFrameDeltaSec();

	_player->pTransform->SetWorldPosition(GetTempPos(22));

	if (IsTimeMoment(_current, 0.5f, TIME_MGR->GetFrameDeltaSec()*1.5))
	{
		_player->_RollingFX.Start(0.5, false);
	}
	

	if (!_player->IsPlayAnimation())
		_player->SetPositionByRoot();
	if (!_player->IsPlayAnimation()) if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
}

//********** ActionPlayerAttack ***********//
ActionPlayerAttack::ActionPlayerAttack()
{
	_command.clear();
	_command.push_back({ PIK_MAIN_ATTACK, 1 });

	ActionPlayer::SetAction("Attack");
}

void ActionPlayerAttack::Start(void * lpvoid)
{
	ActionPlayer::Start(lpvoid);
	_player->Attack(0.1f);
}

void ActionPlayerAttack::Update()
{
	switch (_player->GetAttackNum())
	{
	case 1: default:
		UpdatePlayerAttackTime(0.3, 0.7);
		_player->_SideAtkPtFX.Start(0.3, false);
		break;
	case 2:
		UpdatePlayerAttackTime(0.2, 0.3);
		_player->_SideAtkPtFX.Start(0.3, false);
		break;
	case 3:
		UpdatePlayerAttackTime(0.1, 0.2);
		_player->_SideAtkPtFX.Start(0.1 ,false);
		break;
	case 0:
		UpdatePlayerAttackTime(0.0, 0.5);
		_player->_SideAtkPtFX.Start(0.0, false);
		break;
	}

	if (!_player->IsPlayAnimation())
	{
		_player->SetAttackTimeState(false);
		_player->AttackStop();

		if (!_player->ActionToCommand(_command, this))
		{
			_player->ClearAttackNum();
			if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
		}
	}
}

//********** ActionPlayerWalkAttack ***********//
ActionPlayerWalkAttack::ActionPlayerWalkAttack()
{
	_command.clear();
	_command.push_back({ PIK_FRONT, 1 });
	_command.push_back({ PIK_MAIN_ATTACK, 1 });

	ActionPlayer::SetAction("WalkAttack");
}

void ActionPlayerWalkAttack::Start(void * lpvoid)
{
	ActionPlayer::Start(lpvoid);
	SetDestPosition(-_player->pTransform->GetForward() * 2);

	_player->WalkAttack(0.1f);
	_timer->Start(INT_MAX);
}

void ActionPlayerWalkAttack::Update()
{
	float damage = PLAYER_MGR->GetPlayerTotalStatus().Atk / 2;

	switch (_player->GetWalkAttackNum())
	{
	case 0: default:
		UpdatePlayerAttackTime(0.5, 0.55);
		_player->_Storm->StartEmission();
		_player->_StormDust->StartEmission();
		_player->_StormPt->StartEmission();
		_player->_SideAtkPtFX.Start(0.5, false);
		
	
		_player->_Storm->pTransform->SetWorldPosition(_player->pTransform->GetWorldPosition());
		_player->_StormDust->pTransform->SetWorldPosition(_player->_Storm->pTransform->GetWorldPosition());
		_player->_StormPt->pTransform->SetWorldPosition(_player->_Storm->pTransform->GetWorldPosition());


		for (int i = 0; i < 5; i++)
		{
			if (_timer->Check(i + 100, 0.2f + 0.2f * (i + 1)))
			{
				_player->_hitMonster.clear();

				MONSTER_MGR->PlayerCollision(_player, damage, _player->_Storm->pTransform, _player->_StormBound);
			}
		}


		break;
	case 1:
		UpdatePlayerAttackTime(0.2, 0.3);
		_player->_Storm2->StartEmission();
		_player->_StormDust2->StartEmission();
		_player->_StormPt2->StartEmission();
		_player->_SideAtkPtFX.Start(0.5, false);
	
		
		_player->_Storm2->pTransform->SetWorldPosition(_player->pTransform->GetWorldPosition());
		_player->_StormDust2->pTransform->SetWorldPosition(_player->_Storm2->pTransform->GetWorldPosition());
		_player->_StormPt2->pTransform->SetWorldPosition(_player->_Storm2->pTransform->GetWorldPosition());	

		for (int i = 0; i < 5; i++)
		{
			if (_timer->Check(i + 200, 0.2f * (i + 1)))
			{
				_player->_hitMonster.clear();

				MONSTER_MGR->PlayerCollision(_player, damage, _player->_Storm2->pTransform, _player->_StormBound2);
			}
		}

		break;
	}

	
	


		
	_player->pTransform->SetWorldPosition(GetTempPos(22));

	if (!_player->IsPlayAnimation())
	{
		_player->SetAttackTimeState(false);
		UINT s = _player->WalkAttackStop();	
		if (s == 0)
		{
			//_player->SetPositionByRoot();
			D3DXVECTOR3 temp = _player->rootBoneTransform->GetWorldPosition();
			temp.y = _player->pTransform->GetWorldPosition().y;// pTransform->GetWorldPosition().y;
			_player->pTransform->SetWorldPosition(temp);
		}

		if (!_player->ActionToCommand(_command, this))
		{
			_player->ClearAttackNum();
			if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish((s == 0)? 0:0.2f);
		}
	}
}

//********** ActionPlayerSideAttackLeft ***********//
ActionPlayerSideAttackLeft::ActionPlayerSideAttackLeft()
{
	_command.clear();
	_command.push_back({ PIK_LEFT, 1 });
	_command.push_back({ PIK_MAIN_ATTACK, 1 });

	ActionPlayer::SetAction("SideAttackLeft");
}

void ActionPlayerSideAttackLeft::Start(void * lpvoid)
{
	ActionPlayer::Start(lpvoid);
	SetDestPosition(_player->pTransform->GetRight() * 5);
	_player->SideAttackLeft(0.1f);
	_player->ClearAttackNum();
	_timer->Start(INT_MAX);

}

void ActionPlayerSideAttackLeft::Update()
{
	UpdatePlayerAttackTime(0.2, 0.55);

	if (_timer->Check(1, 0.2))
	{
		_player->_SideAtkFX.Start(1.1, false);
		_player->_SideAtkPtFX.Start(0.5, false);
	}

	_player->pTransform->SetWorldPosition(GetTempPos(22));
	if (!_player->IsPlayAnimation())
		_player->SetPositionByRoot();
	if (!_player->IsPlayAnimation()) if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
}

//********** ActionPlayerSideAttackRight ***********//
ActionPlayerSideAttackRight::ActionPlayerSideAttackRight()
{
	_command.clear();
	_command.push_back({ PIK_RIGHT, 1 });
	_command.push_back({ PIK_MAIN_ATTACK, 1 });

	ActionPlayer::SetAction("SideAttackRight");
}

void ActionPlayerSideAttackRight::Start(void * lpvoid)
{
	ActionPlayer::Start(lpvoid);
	SetDestPosition(-_player->pTransform->GetRight() * 5);
	_player->SideAttackRight(0.1f);
	_player->ClearAttackNum();

	_timer->Start(INT_MAX);
}

void ActionPlayerSideAttackRight::Update()
{
	UpdatePlayerAttackTime(0.2, 0.55);

	if (_timer->Check(1, 0.2))
	{
		_player->_SideAtkFX.Start(1.1, false);
		_player->_SideAtkPtFX.Start(0.5, false);
	}

	_player->pTransform->SetWorldPosition(GetTempPos(22));
	if (!_player->IsPlayAnimation())
		_player->SetPositionByRoot();
	if (!_player->IsPlayAnimation()) if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
}

ActionPlayerSheildAttack::ActionPlayerSheildAttack()
{
	ActionPlayer::SetAction("SheildAttack");
}

void ActionPlayerSheildAttack::Start(void * lpvoid)
{
	ActionPlayer::Start(lpvoid);
	_player->animation->Stop();
	_player->ShieldAttack(0.0f);
	_player->ClearAttackNum();
}

void ActionPlayerSheildAttack::Update()
{
	UpdatePlayerAttackTime(0.0, 0.3);

	if (!_player->IsPlayAnimation()) if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
}

ActionPlayerUltimate::ActionPlayerUltimate()
{
	ActionPlayer::SetAction("Ultimate");
}

void ActionPlayerUltimate::Start(void * lpvoid)
{
	ActionPlayer::Start(lpvoid);
	_player->animation->Stop();
	_player->Ultimate(0.0f);
	_player->ClearAttackNum();

	_timer->Start(INT_MAX);
	_player->_Uskill05FX.Start(1.2, false);
	//카메라 전환
	//CAMERA_MGR->SetMapView();
	//D3DXVECTOR3 pos = _player->pTransform->GetWorldPosition() - _player->pTransform->GetForward() * 3;
	//MAPVIEW_CAMERA->SetPos(pos + D3DXVECTOR3(0, 5, 10), pos + D3DXVECTOR3(0, 2, -4), 3.5);
}

void ActionPlayerUltimate::Update()
{


	if (_timer->Check(0, 1.85f))
	{
		_player->_UskillFX.Start(1.5, false);
		_player->_Uskill02FX.Start(0.7 , false);
		_player->_Uskill03FX.Start(0.6, false);
		_player->_Uskill04FX->StartEmission();
		//CAMERA_MGR->SetPlayer();
		
		CAMERA->SetFov(RAD(45), RAD(60), 2);
	}


	if (_timer->Check(1, 2.15f))
	{
		_player->_Uskill04FX->StopEmission();
			
		float damage = PLAYER_MGR->GetPlayerTotalStatus().Atk;

		cBoundBox c;
		c.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(120, 120, 120));

		MONSTER_MGR->PlayerCollision(_player, damage * 20, _player->pTransform, c);
	}


	MAPVIEW_CAMERA->SetLook(_player->GetSwordTransform()->GetWorldPosition());
	
	//기본 업데이트
	//UpdatePlayerAttackTime(0.5, 2.5f);

	if (!_player->IsPlayAnimation())
	{
		
		if (_deligate) ((ActionPlayerDeligate*)_deligate)->OnActionFinish(0.2f);
		CAMERA_MGR->SetPlayer();
	}
}
