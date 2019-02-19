#include "stdafx.h"
#include "MonsterManager.h"

std::map<std::string, MonsterInfo> MonsterManager::MapMonster;

bool monsterSort(Monster* a, Monster* b)
{
	Monster* mona = (Monster*)a;
	Monster* monb = (Monster*)b;

	return  mona->GetPlayerDistance() < monb->GetPlayerDistance();
	/*
	if (mona->GetPlayerDistance() < monb->GetPlayerDistance()) return -1;
	if (mona->GetPlayerDistance() == monb->GetPlayerDistance()) return 0;
	if (mona->GetPlayerDistance() > monb->GetPlayerDistance()) return 1;
	*/
}

MonsterManager::MonsterManager()
{

}


MonsterManager::~MonsterManager()
{
}

void MonsterManager::Setup()
{
	_deadTag.SetTexture(RESOURCE("Textures/Icon/R.png"));
	return;
}

void MonsterManager::Release()
{
	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		_vecTotalMonster[i]->Release();
		SAFE_DELETE(_vecTotalMonster[i]);
	}

	_vecTotalMonster.clear();
}

void MonsterManager::Update(float timeDelta)
{
	std::sort(_vecTotalMonster.begin(), _vecTotalMonster.end(), monsterSort); //몬스터 플레이어 거리를 기준으로 정렬

	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		_vecTotalMonster[i]->Update(timeDelta);

		//몬스터 네임텍 출력?
	}
	//몬스터 끼리 충돌 안됨
	//삭제 NO
	cBoundBox mHitBound1, mHitBound2;
	Transform *mHitTrans1, *mHitTrans2;
	mHitBound1.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	mHitBound2.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		if (_vecTotalMonster[i]->IsState(Character::CS_DOWN)) continue;

		mHitTrans1 = _vecTotalMonster[i]->pTransform;
		
		for (int j = 0; j < _vecTotalMonster.size(); j++)
		{
			if (i == j) continue;
			if (_vecTotalMonster[j]->IsState(Character::CS_DOWN)) continue;

			mHitTrans2 = _vecTotalMonster[j]->pTransform;

			PHYSICS_MGR->IsBlocking(mHitTrans1, &mHitBound1, mHitTrans2, &mHitBound2, 0.1f);
		}
	}

	DeadUpdate();
}

void MonsterManager::Render()
{
	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		_vecTotalMonster[i]->Render();
	}

	cBoundBox mHitBound;
	Transform** mHitTrans = NULL;
	int mHitTransNum;

	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		_vecTotalMonster[i]->GetHitBound(&mHitTrans, &mHitTransNum);
		mHitBound.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 1, 1));

		for (int j = 0; j < mHitTransNum;j++)
		mHitBound.RenderGizmo(mHitTrans[j]);
	}
/*
	if (_deadNearMonster && CAMERA->Frustum.IsInFrustum(_deadNearMonster))
	{
		_deadTag.RenderToSprite(SPRITE, CAMERA);
	}
*/
}

void MonsterManager::NameTagRender()
{
	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		_vecTotalMonster[i]->PrintNameTag();
	}

	if (_deadNearMonster && CAMERA->Frustum.IsInFrustum(_deadNearMonster))
	{
		_deadTag.RenderToSprite(SPRITE, CAMERA);
	}
}

void MonsterManager::PlayerCollision(Player * player)
{
	return;
	cBoundBox pAttackBound;
	Transform* pAttackTrans = player->GetAttackBound();
	cBoundBox mHitBound;
	Transform** mHitTrans;
	int mHitTransNum;

	pAttackBound.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.2, 0.2, 0.2));
	
	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		//플레이어 어택타임이 아니면 지나감
		if (!player->IsAttackTime()) continue;
		//몬스터 상태가 넉다운이면 지나감
		if (_vecTotalMonster[i]->IsState(Character::CS_DOWN)) continue;
		
		//몬스터의 피격 바운드 박스들 가져옴
		_vecTotalMonster[i]->GetHitBound(&mHitTrans, &mHitTransNum);

		for (int j = 0; j < mHitTransNum; j++)
		{
			//바운드 박스 셋팅 후,
			mHitBound.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 1, 1));

			//충돌처리
			if (PHYSICS_MGR->IsOverlap(pAttackTrans, &pAttackBound, mHitTrans[j], &mHitBound))
			{
				//이미 맞은 공격이 아니면 들어감
				if (player->IsAttackMonster(_vecTotalMonster[i]))
				{
					_vecTotalMonster[i]->Hit(player->GetCharacterInfo().Atk);
					
					CAMERA->ShakePos(0.1f, 0.2f);
					break;
				}
				SOUND_MGR->play("hit_metal01", 0.2f);
			}
		}
	}
}

void MonsterManager::PlayerCollision(Player * player, float damage, Transform * trans, cBoundBox & bound)
{
	cBoundBox mHitBound;
	Transform** mHitTrans;
	int mHitTransNum;

	//바운드 박스 셋팅
	mHitBound.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 1, 1));

	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		//몬스터 상태가 넉다운이면 지나감
		if (_vecTotalMonster[i]->IsState(Character::CS_DOWN)) continue;

		//몬스터의 피격 바운드 박스들 가져옴
		_vecTotalMonster[i]->GetHitBound(&mHitTrans, &mHitTransNum);

		for (int j = 0; j < mHitTransNum; j++)
		{
			//바운드 박스 셋팅 후,
			mHitBound.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 1, 1));

			//충돌처리
			if (PHYSICS_MGR->IsOverlap(trans, &bound, mHitTrans[j], &mHitBound) &&
				!player->IsAttackMonster(_vecTotalMonster[i])) //맞았을때 호출하여 한번 호출하면 내부의 벡터에 몬스터를 넣는다. 이미 있을 경우 true
			{
				//피격 이펙트 트랜스폼을 현재트랜스폼으로 바꿈

				if (_vecTotalMonster[i]->_isBoss)
				{
					_vecTotalMonster[i]->_MonsterDamage.SetOrigin(mHitTrans[j]);
					_vecTotalMonster[i]->_MonsterDamage2.SetOrigin(mHitTrans[j]);
					_vecTotalMonster[i]->_MonsterDamage3.SetOrigin(mHitTrans[j]);
					_vecTotalMonster[i]->_MonsterDamage4.SetOrigin(mHitTrans[j]);
				}

				_vecTotalMonster[i]->Hit(damage);
	
				
				LOG_MGR->AddLog("MM 타격");
				SOUND_MGR->play("hit_leather01", 0.3f);
				CAMERA->ShakePos(0.1f, 0.2f);

				//몬스터의 상태가 공격이 아니면 바꿈
				if (_vecTotalMonster[i]->_battleState == Character::CBS_NORMAL)
				{
					_vecTotalMonster[i]->_battleState = Character::CBS_BATTLE;
					_vecTotalMonster[i]->_PrevPos = _vecTotalMonster[i]->pTransform->GetWorldPosition();
				}


				break; //이 몬스터는 맞았으니 다른 바운드 박스 처리할 필요 없음
			}
		}
	}
}

void MonsterManager::TerrainCollision(cTerrain * terrain)
{
	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		D3DXVECTOR3 pos = _vecTotalMonster[i]->pTransform->GetWorldPosition();
		_vecTotalMonster[i]->SetHeight(terrain->GetHeight(pos.x, pos.z));
	}
}

bool MonsterManager::CreateMonster(MonsterInfo & info)
{
	if (info.IsBoss) 
		return CreateBossMonster(info);

	if (info.IsMiddleBoss)
	{
		return CreateMiddleBossMonster(info);
	}

	
	Monster* mon;
	for (int i = 0; i < info.MonNum; i++)
	{
		if (std::string(info.Name) == "Muspel")		mon = new Muspel;
		else if (std::string(info.Name) == "Pumpkin")		mon = new Pumpkin;
		else if (std::string(info.Name) == "SkullWarrior") mon = new SkullWarrior;
		else if (std::string(info.Name) == "Golem") mon = new Golem;
		//else if ()Golem
		else
		mon = new Monster;

		mon->Setup(info.ResourcePath, info.Area, info);

		_vecTotalMonster.push_back(mon);
		mon->SetActive(true);
		mon->SetName(info.Name);
	}

	return true;
}
bool MonsterManager::CreateBossMonster(MonsterInfo & info)
{
	Boss* boss;
	for (int i = 0; i < info.MonNum; i++)
	{
		boss = new Boss;
		boss->Setup(info.ResourcePath, info.Area, info);

		_vecTotalMonster.push_back(boss);
		boss->SetActive(true);
		boss->SetName(info.Name);
		boss->_isBoss = true;
	}

	return true;
}

bool MonsterManager::CreateMiddleBossMonster(MonsterInfo & info)
{
	MiddleBoss* boss;
	for (int i = 0; i < info.MonNum; i++)
	{
		boss = new MiddleBoss;
		boss->Setup(info.ResourcePath, info.Area, info);

		_vecTotalMonster.push_back(boss);
		boss->SetActive(true);
		boss->SetName(info.Name);

		boss->_isMiddleBoss = true;
	}

	return true;
}

void MonsterManager::LoadMonsters()
{
	MapMonster.clear();

	RECT temp = { 0, 0, 0, 0 };
	
	MonsterInfo info = MonsterInfo("../Resources/Meshes/Monster/muspel/muspel.X", "Muspel", 1, temp, 100, 0, 50, 5, 0.5, 4, 6, 3);
	info.MonNum = 1;
	info.IsBoss = false;
	info.Level = 10;
	MapMonster.insert(std::make_pair(info.Name, info));

	info = MonsterInfo("../Resources/Meshes/Monster/orc/orc.X", "Orc", 1, temp, 500, 0, 50, 5, 0.5, 5, 6, 3);
	info.MonNum = 1;
	info.IsBoss = false;
	info.IsMiddleBoss = true;
	info.Level = 10;
	MapMonster.insert(std::make_pair(info.Name, info));
	

	info = MonsterInfo("../Resources/Meshes/Monster/pumpkin/pumpkin.X", "Pumpkin", 1, temp, 100, 0, 50, 5, 0.5, 3, 5, 3);
	info.MonNum = 1;
	info.IsBoss = false;
	info.Level = 1;
	MapMonster.insert(std::make_pair(info.Name, info));

	info = MonsterInfo("../Resources/Meshes/Monster/skullWarrior/skullWarrior.X", "SkullWarrior", 1, temp, 100, 0, 50, 5, 0.5, 4, 4, 3);
	info.MonNum = 1;
	info.IsBoss = false;
	info.Level = 5;
	MapMonster.insert(std::make_pair(info.Name, info));


	info = MonsterInfo("../Resources/Meshes/Monster/Golem/Golem.X", "Golem", 1, temp, 100, 0, 50, 5, 0.5, 4, 4, 3);
	info.MonNum = 1;
	info.IsBoss = false;
	info.Level = 6;
	MapMonster.insert(std::make_pair(info.Name, info));
	
	 info =
		MonsterInfo(
			"../Resources/Meshes/Monster/Seth/Seth.X", //리소스
			"Seth", //이름
			1, //스폰 갯수
			temp, //스폰 범위
			5000, //체력
			0, //마나
			50, //공격
			20, //방어
			2, //공격 속도
			3.5, //걷는 속도
			8, //달리는 속도
			30 //공격 범위
			);
	info.MonNum = 1;
	info.IsBoss = true;
	info.Level = 30;
	MapMonster.insert(std::make_pair(info.Name, info));
}

void MonsterManager::SetPlayer(Player * player)
{
	Monster::SetPlayerTrans(player->pTransform);
}

Monster* MonsterManager::GetNearMonster(D3DXVECTOR3 pos, float dist)
{
	Monster* result = NULL;
	float distance = dist, temp;

	for (int i = 0; i < _vecTotalMonster.size(); i++)
	{
		temp = D3DXVec3Length(&(pos - _vecTotalMonster[i]->pTransform->GetWorldPosition()));
		if (distance < temp)
			continue;
		
		result = _vecTotalMonster[i];
		distance = temp;
	}

	return result;
}

void MonsterManager::DeadUpdate()
{
	if (_deadMonster.size() < 1)
	{
		_deadNearMonster = NULL;
		return;
	}

	//std::sort(_deadMonster.begin(), _deadMonster.end(), monsterSort);

	if ((*_deadMonster.begin())->GetPlayerDistance() < 6) _deadNearMonster = (*_deadMonster.begin());
	else _deadNearMonster = NULL;

	if (_deadNearMonster)
	{
		D3DXVECTOR3 pos = _deadNearMonster->pTransform->GetWorldPosition();
		pos.y += 3;

		_deadTag.SetPosition(pos);

		D3DXVECTOR3 dir = CAMERA->GetWorldPosition() - pos;

		_deadTag.Look(-dir, D3DXVECTOR3(0, 1, 0));

		if (KEY_MGR->IsOnceDown('R'))
		{
			EraseDeadMonster(_deadNearMonster);
			SOUND_MGR->play("sell_buy_item",0.2f);
			//루팅 코드 넣으면 됨

		}
	}
}

void MonsterManager::SpawnMonster(MonsterInfo &info)
{
	/*
	_mapMonster;
	_mapMonsterInfo;

	if (_mapMonsterInfo.find(info.resourcePath) != _mapMonsterInfo.end())
	{
		return;
	}

	TotalMonster::iterator iter = _mapMonster.find(info.resourcePath);

	Monster* newMonster = new Monster;
	newMonster->Setup(info.resourcePath, info.area, info);

	iter->second.push_back(newMonster);
	_vecTotalMonster.push_back(newMonster);
	*/
}
