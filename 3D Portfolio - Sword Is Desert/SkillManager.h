#pragma once
#include "Singleton.h"

class Skill;
class SkillManager :
	public Singleton < SkillManager >
{
private:
	std::map<std::string, Skill*> _skills;
	std::vector<Skill*> _vecSkill;
public:
	SkillManager();
	virtual ~SkillManager();

	void Setup();
	void Release();

	void AddSkill(const char* name, Skill* skill);
	Skill* GetSkill(const char* name);

	std::vector<Skill*>& GetSkills() { return _vecSkill; }

	void SkillSetting();
};

#define SKILL_MGR SkillManager::GetInstance()