#pragma once
#include "Singleton.h"
#include "Monster.h"
#include "Player.h"
#include "Boss.h"
#include "cTerrain.h"
#include "MiddleBoss.h"
#include "Muspel.h"
#include "Pumpkin.h"
#include "SkullWarrior.h"
#include "Golem.h"

typedef struct tagMonsterInfo : public tagCharacterInfo
{
	const char* ResourcePath;
	const char* Name;
	UINT MonNum;
	RECT Area;
	bool IsBoss;
	bool IsMiddleBoss = false;

	tagMonsterInfo() {}
	tagMonsterInfo(
		const char* resourcePath,
		const char* name,
		UINT monNum,
		RECT area,
		float mhp = 0.0f,
		float mmp = 0.0f,
		float atk = 0.0f,
		float def = 0.0f,
		float atkSpeed = 1.0f,
		float walkSpeed = 2.0f,
		float runSpeed = 3.0f,
		float atkRange = 2.0f,
		float respawnTime = 10.0f,
		float atkScale = 1.5f)
	{
		//ResourcePath = resourcePath;
		ResourcePath = resourcePath;
		Name = name;
		MonNum = monNum;
		Area = area;
		MaxHp = Hp = mhp;
		MaxMp = Mp = mmp;
		Atk = atk;
		Def = def;
		AtkSpeed = atkSpeed;
		WalkSpeed = walkSpeed;
		RunSpeed = runSpeed;
		AtkRange = atkRange;
		RespwanTime = respawnTime;
		AtkScale = atkScale;
	}

} MonsterInfo;

struct DeadMonSort
{
	bool operator () (Monster* m1, Monster* m2) const
	{
		return m1->GetPlayerDistance() < m2->GetPlayerDistance();
	}

};

class MonsterManager : public Singleton < MonsterManager >
{
private:
	typedef std::vector<Monster*> Monsters;
	//typedef std::map<std::string, Monsters> TotalMonster;
	//typedef std::map<std::string, MonsterInfo> MapMonsterInfo;

public:
	Monsters _vecTotalMonster;

private:
	Player* _player;

	//TotalMonster _mapMonster;
	//MapMonsterInfo _mapMonsterInfo;
	cTerrain* _ter;

	//죽은 후 루팅 안된 몬스터
	std::set<Monster*, DeadMonSort> _deadMonster;
	Monster* _deadNearMonster = NULL;
	NameTag _deadTag;
public:
	static std::map<std::string, MonsterInfo> MapMonster;
public:
	MonsterManager();
	virtual ~MonsterManager();



	void Setup();
	void Release();
	void Update(float timeDelta);
	void Render();
	void NameTagRender();

	void PlayerCollision(Player* player);
	void PlayerCollision(Player * player, float damage, Transform* trans, cBoundBox& bound);
	void TerrainCollision(cTerrain* terrain);

	bool CreateMonster(MonsterInfo& info);
	bool CreateBossMonster(MonsterInfo & info);
	bool CreateMiddleBossMonster(MonsterInfo &info);

	static void LoadMonsters();
	static void SetPlayer(Player* player);

	Monster* GetNearMonster(D3DXVECTOR3 pos, float dist);

	void DeadUpdate();

	void AddDeadMonster(Monster* mon) { _deadMonster.insert(mon); }
	void EraseDeadMonster(Monster* mon) { _deadMonster.erase(mon); }
private:
	void SpawnMonster(MonsterInfo& info);

	

};

#define MONSTER_MGR MonsterManager::GetInstance()