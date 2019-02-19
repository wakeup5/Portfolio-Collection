#include "stdafx.h"
#include "TestScene.h"
#include "MonsterManager.h"
#include "SkillManager.h"
#include "UISkill.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

HRESULT TestScene::Setup()
{
	
	_effect.push_back(EFFECT_MGR->CreateParticleEmitter<Dust>("테스트용"));
	_effect.Start(50, false);

	_ui = new UISkill;
	_ui->Setup();
	return S_OK;
}

void TestScene::Release()
{

}

void TestScene::Update(float timeDelta)
{
	_ui->Update();

	if (KEY_MGR->IsOnceDown(VK_F1))
	{
		_ui->Toggle();
		_ui->SetHiddenState(true);
	}
}

void TestScene::Render()
{
	EFFECT_MGR->Render();
	_ui->Render(SPRITE);
}
