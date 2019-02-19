#pragma once
#include "Scene.h"

class UISkill;
class TestScene : public Scene
{
private:
	EffectVector _effect;
	UISkill* _ui;
public:
	TestScene();
	virtual ~TestScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

