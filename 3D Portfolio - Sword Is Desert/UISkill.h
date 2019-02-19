#pragma once
#include "UIWindow.h"
#include "UISocket.h"
#include "UITextBox.h"

class Skill;

class UISkill : public UIWindow
{
private:
	Skill* _selectSkill;
	UITextBox* disc;
public:
	UISkill();
	virtual ~UISkill();

	void Setup();
	void Release();
	void Update();
	void Render(LPD3DXSPRITE sprite);
};

class UISkillSocketDeligate : public UISocketDeligate
{
public:
	UIWindow* DiscWindow = NULL;
public:
	inline virtual UINT OnMouseOver(UIWindow* caller, UIWindow* root) override;
	inline virtual UINT OnMouseOut(UIWindow* caller, UIWindow* root) override;
};