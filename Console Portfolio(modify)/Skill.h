#pragma once
class Skill
{
private:
	int _skillLevel;
	const float _power;
	const float _levelPower;
	const int _fixedPower;
	const int _levelFixedPower;

	const char* _skillName;
	const char* _brief;

	const int _coolTime;
	bool _isCoolTime;
	bool _isUse;
public:
	Skill(char* name, int level, float power = 2.0, float levelPower = 0.1, int fixedPower = 100, int levelFixedPower = 5,int coolTime = 5000, char* brief = " ");
	~Skill();

	void skillLevelUp();
	int getSkillDamage(int atk);

	int getCooltime(){ return _coolTime; }

	void setIsUse(bool isUse){ _isUse = isUse; }
	void setIsCoolTime(bool isCoolTime){ _isCoolTime = isCoolTime; }
	bool getIsCoolTime(){ return _isCoolTime; }
	
	const char* getName(){ return _skillName; }
	const char* getBrief(){ return _brief; }

	float getPower(){ return _power + _levelPower * _skillLevel; }
	float getFixedPower(){ return _fixedPower + _levelFixedPower * _skillLevel; }
};

