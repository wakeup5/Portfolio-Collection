#include "Skill.h"


Skill::Skill(char* name, int level, float power, float levelPower, int fixedPower, int levelFixedPower, int coolTime, char* brief)
	: _skillName(name)
	, _skillLevel(level)
	, _power(power)
	, _levelPower(levelPower)
	, _fixedPower(fixedPower)
	, _levelFixedPower(levelFixedPower)
	, _brief(brief)
	, _coolTime(coolTime)
{
	//_skillLevel = level;
	_isUse = true;
}


Skill::~Skill()
{
}

void Skill::skillLevelUp()
{
	_skillLevel++;
}
int Skill::getSkillDamage(int atk)
{
	float power = _power + _skillLevel * _levelPower;
	int fixed = _fixedPower + _skillLevel * _levelFixedPower;

	return static_cast<int>(atk * power + fixed);
}