#include "stdafx.h"
#include "GameConfig.h"


GameConfig::GameConfig()
{
}


GameConfig::~GameConfig()
{
}

void GameConfig::Update()
{
	GAME_CONFIG_MEMBER_INPUT_KEY_ON_OFF(VK_F1, Dof);
}
