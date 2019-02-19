#include "stdafx.h"
#include "SkillManager.h"
#include "Skill.h"

SkillManager::SkillManager()
{
}


SkillManager::~SkillManager()
{
}

void SkillManager::Setup()
{

}

void SkillManager::Release()
{
	for (auto iter = _vecSkill.begin(); iter != _vecSkill.end(); iter++)
	{
		delete *iter;
	}

	_skills.clear();
	_vecSkill.clear();
}

void SkillManager::AddSkill(const char* name, Skill * skill)
{
	if (skill == NULL) return;

	_skills.insert(std::make_pair(name, skill));
	_vecSkill.push_back(skill);
}

Skill* SkillManager::GetSkill(const char * name)
{
	auto i = _skills.find(name);
	if (i == _skills.end()) return NULL;

	return i->second;
}

void SkillManager::SkillSetting()
{
	Release();

	//스킬 셋팅
	SkillInfo info;
	std::vector<KeyType> command;
	//좌 구르기
	command.push_back({ PIK_LEFT, 0 });
	command.push_back({ PIK_LEFT, 0 });

	info.Name = "좌 구르기";
	info.Disc = "좌로 한번 굴러 적의 공격을 회피한다. \n 'A', 'A'";
	info.IconPath = "../Resources/Textures/icon/skill/rolling.png";
	info.ActionName = "RollingLeft";
	info.State = PS_ROLLING;

	AddSkill("RollingLeft", new Skill(command, info));
	command.clear();

	//우 구르기
	command.push_back({ PIK_RIGHT, 0 });
	command.push_back({ PIK_RIGHT, 0 });

	info.Name = "우 구르기";
	info.Disc = "우로 한번 굴러 적의 공격을 회피한다. \n 'D', 'D'";
	info.IconPath = "../Resources/Textures/icon/skill/rolling.png";
	info.ActionName = "RollingRight";
	info.State = PS_ROLLING;

	AddSkill("RollingRight", new Skill(command, info));
	command.clear();

	//뒤 구르기
	command.push_back({ PIK_BACK, 0 });
	command.push_back({ PIK_BACK, 0 });

	info.Name = "뒤 구르기";
	info.Disc = "뒤로 한번 굴러 적의 공격을 회피한다. \n 'S', 'S'";
	info.IconPath = "../Resources/Textures/icon/skill/rolling.png";
	info.ActionName = "RollingBack";
	info.State = PS_ROLLING;

	AddSkill("RollingBack", new Skill(command, info));
	command.clear();

	//궁극기
	command.push_back({ PIK_BACK, 0 });
	command.push_back({ PIK_MAIN_ATTACK, 0 });

	info.Name = "지면 찌르기";
	info.Disc = "힘을 모으고 크게 도약을 한 다음에 지면을 강하게 내려 찍는다. \n 'S', Click";
	info.IconPath = "../Resources/Textures/icon/skill/attack_normal.png";
	info.ActionName = "Ultimate";
	info.State = PS_ATTACK;
	info.Damage = 20;

	AddSkill("Ultimate", new Skill(command, info));
	command.clear();

	//기본 공격
	command.push_back({ PIK_MAIN_ATTACK, 1 });

	info.Name = "일반 공격";
	info.Disc = "검을 4번 연속으로 휘두르는 기본 공격. \n Click";
	info.IconPath = "../Resources/Textures/icon/skill/Attack.png";
	info.ActionName = "Attack";
	info.State = PS_ATTACK;
	info.Damage = 1;

	AddSkill("Attack", new Skill(command, info));
	command.clear();

	//전진 공격
	command.push_back({ PIK_FRONT, 1 });
	command.push_back({ PIK_MAIN_ATTACK, 1 });

	info.Name = "전진 공격";
	info.Disc = "전진하며 두번 벤다. \n 'W', Click";
	info.IconPath = "../Resources/Textures/icon/skill/WalkAttack.png";
	info.ActionName = "WalkAttack";
	info.State = PS_WALKATTACK;
	info.Damage = 1.5;

	AddSkill("WalkAttack", new Skill(command, info));
	command.clear();

	//왼 사이드 공격
	command.push_back({ PIK_LEFT, 1 });
	command.push_back({ PIK_MAIN_ATTACK, 1 });

	info.Name = "왼 휘두르기";
	info.Disc = "왼쪽으로 검을 휘두르며 공격한다. \n 'A', Click";
	info.IconPath = "../Resources/Textures/icon/skill/SideAttackLeft.png";
	info.ActionName = "SideAttackLeft";
	info.State = PS_SIDEATTACK;
	info.Damage = 2;

	AddSkill("SideAttackLeft", new Skill(command, info));
	command.clear();

	//오른 사이드 공격
	command.push_back({ PIK_RIGHT, 1 });
	command.push_back({ PIK_MAIN_ATTACK, 1 });

	info.Name = "오른 휘두르기";
	info.Disc = "오른쪽으로 검을 휘두르며 공격한다. \n 'D', Click";
	info.IconPath = "../Resources/Textures/icon/skill/SideAttackRight.png";
	info.ActionName = "SideAttackRight";
	info.State = PS_SIDEATTACK;
	info.Damage = 2;

	AddSkill("SideAttackRight", new Skill(command, info));
	command.clear();

	//방패 공격
	command.push_back({ PIK_SUB_ATTACK, 0 });

	info.Name = "방패 치기";
	info.Disc = "방패를 휘둘러 적을 넉백 시킨다. \n RClick";
	info.IconPath = "../Resources/Textures/icon/skill/sAttack.png";
	info.ActionName = "SheildAttack";
	info.State = PS_ATTACK;
	info.Damage = 2.5;

	AddSkill("SheildAttack", new Skill(command, info));
	command.clear();
}
