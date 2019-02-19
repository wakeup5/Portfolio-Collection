#include "stdafx.h"
#include "Skill.h"


Skill::Skill()
{
	_type = ST_SKILL;
}

Skill::Skill(std::vector<KeyType>& command, std::string name, PLAYER_STATE state)
{
	_type = ST_SKILL;
	Setup(command, name, state);
}

Skill::Skill(std::vector<KeyType>& command, SkillInfo & info)
{
	_type = ST_SKILL;
	Setup(command, info);
}


Skill::~Skill()
{
	_command.clear();
}

void Skill::Setup(std::vector<KeyType>& command, std::string name, PLAYER_STATE state)
{
	_command.assign(command.begin(), command.end());
	_action = ActionPlayer::GetAction(name);
	_action->Damage = 1;
	_info.State = state;
}

void Skill::Setup(std::vector<KeyType>& command, SkillInfo & info)
{
	_command.assign(command.begin(), command.end());
	_action = ActionPlayer::GetAction(info.ActionName);
	_info = info;
	_action->Damage = _info.Damage;
	SetIcon(info.IconPath);
}
