#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Inventory.h"
#include "Equipment.h"
#include "Mercenary.h"
#include "Pet.h"
#include "Around_Dust.h"
#include "Dust.h"

PlayerManager::PlayerManager()
{
}


PlayerManager::~PlayerManager()
{
}

HRESULT PlayerManager::Setup()
{
	ITEM_MGR->LoadItemInfo();

	_player = new Player;
	_player->Setup();
	_player->SetActive(true);

	_inven = new Inventory;
	
	_equip = new Equipment;

	//활 메시 
	D3DXMATRIXA16 scale;
	D3DXMatrixScaling(&scale, 0.07f, 0.07f, 0.07f);
	_bowMesh = new cXMesh_Static;
	_bowMesh->Init(RESOURCE("Weapon/Bow.X"), &scale);
	//활 메시 end

	SKILL_MGR->Setup();
	SKILL_MGR->SkillSetting();

	_mercenary = new Mercenary;
	_mercenary->Setup();
	_mercenary->SetActive(true);
	_mercenary->SetPcTrans(_player->pTransform);

	_pet = new Pet;
	_pet->Setup();
	_pet->SetActive(true);
	_pet->SetPcTrans(_player->pTransform);

	//_inven->SetItem(ITEM_MGR->CreateItem("sword", _player->GetSwordTransform()), 50);
	//_inven->SetItem(ITEM_MGR->CreateItem("shield", _player->GetShieldTransform()), 51);
	//_inven->SetItem(ITEM_MGR->CreateItem("bow", _player->GetShieldTransform()), 52);
	//_equip->Equip(ITEM_MGR->CreateItem("엘쉬 활", _player->GetShieldTransform()));
	_equip->Equip(ITEM_MGR->CreateItem("민병대 장검", _player->GetSwordTransform()));
	_equip->Equip(ITEM_MGR->CreateItem("로사르 방패", _player->GetShieldTransform()));

	return S_OK;
}

void PlayerManager::Release()
{
	_player->Release();
	SAFE_DELETE(_player);

	SAFE_DELETE(_inven);

	SAFE_DELETE(_equip);

	SAFE_DELETE(_pet);

	SAFE_DELETE(_mercenary);
}

void PlayerManager::Update(float timeDelta)
{
	if (_player->Buff == PB_THUNDER)
	{
		_player->Update(timeDelta * 1.2f);
	}
	else
	{
		_player->Update(timeDelta);
	}

	//_mercenary->Update(timeDelta);
	//_pet->Update(timeDelta);

	//_mercenary->SetSpeed(4);
	//_pet->SetSpeed(4);

	PLAYER_CAMERA->SetPlayerPosition(_player->pTransform->GetWorldPosition());

	//몬스터 충돌처리
	//충돌처리할 아이템
	if (_player->IsAttackTime())
	{
		float damage = GetPlayerTotalStatus().Atk * _player->GetSkillDamage();

		if (_player->IsState(PS_SHIELDATTACK))
		{
			Item* sheild = _equip->GetItem(IT_SHEILD);
			if (sheild != NULL)
			{
				sheild->ComputeBoundBox();
				MONSTER_MGR->PlayerCollision(_player, damage, sheild->pTransform, sheild->BoundBox);
			}
		}
		else
		{
			Item* sword = _equip->GetItem(IT_SWORD);
			if (sword != NULL)
			{
				sword->ComputeBoundBox();
				MONSTER_MGR->PlayerCollision(_player, damage, sword->pTransform, sword->BoundBox);
			}
		}
	}

	/*
	if (KEY_MGR->IsOnceDown(VK_F3))
	{
		_player->GetCharacterInfo().Hp = 1;
		_player->Stand(0, true);
	}
	*/
}

void PlayerManager::Render()
{
	_player->Render();

			//_equip->SubWeaponRender();
	if (_player->GetBattleState() == Character::CBS_BATTLE &&
		!_player->IsState(PS_RIDE))
	{
		if (_player->IsState(PS_BOWATTACK))
		{
			_bowMesh->Render(_player->GetShieldTransform());
		}
		else
		{
			_equip->MainWeaponRender();
			//_equip->GetItem(IT_SWORD)->pTransform;
		}
	}

	//_mercenary->Render();
	//_pet->Render();
}

bool PlayerManager::MonsterCollision(Monster * monster)
{
	CharacterInfo info = monster->GetCharacterInfo();

	UINT damage = max(0, info.Atk - GetPlayerTotalStatus().Def);

	float atkScale = info.AtkScale * 2;
	cBoundSphere atkBox;
	atkBox.SetBound(&D3DXVECTOR3(0, 1.5f, 0), &D3DXVECTOR3(1, 1, atkScale * 2));

	if (PHYSICS_MGR->IsOverlap(_player->GetHitBound(), &(cBoundSphere)_player->BoundBox, monster->pTransform, &atkBox))
	{
		_player->Hit(damage);// -_equip->GetTotalStatus().Def);
		//LOG_MGR->AddLog("타격PM");
		
		SOUND_MGR->play("hit_metal01", 0.2f);

		
		//CAMERA->ShakePos(0.2, 0.5);
		CAMERA->SetFov(RAD(60), RAD(55), 0.2);

		return true;
	}

	return false;
}

CharacterInfo PlayerManager::GetPlayerTotalStatus()
{
	CharacterInfo result, p;
	ItemInfo e;

	p = _player->GetCharacterInfo();
	e = _equip->GetTotalStatus();

	result.Atk = p.Atk + e.Atk;
	result.Def = p.Def + e.Def;
	
	if (_player->Buff == PB_FIRE) result.Atk *= 1.2;
	if (_player->Buff == PB_ICE) result.Def *= 1.2;

	return result;
}
