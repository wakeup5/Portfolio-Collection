#pragma once
#include "Singleton.h"
#include "stdafx.h"

#define GAME_CONFIG_MEMBER_SETTER_GETTER(type, name, initData) private: type m_v##name = initData; public: void Set##name(type var){m_v##name = var;} type Get##name() { return m_v##name; }
#define GAME_CONFIG_MEMBER_INPUT_KEY_TOGGLE(key, name) { if(KEY_MGR->IsOnceDown(key)) { m_v##name = !m_v##name; } }

class GameConfig : public Singleton < GameConfig >
{
	GAME_CONFIG_MEMBER_SETTER_GETTER(bool, IsDof, false);
	GAME_CONFIG_MEMBER_SETTER_GETTER(bool, IsPrintBound, false);

	//test
	GAME_CONFIG_MEMBER_SETTER_GETTER(bool, IsTimeFast, false);
public:
	inline GameConfig() {}
	inline virtual ~GameConfig() {}

	inline void Update()
	{
		GAME_CONFIG_MEMBER_INPUT_KEY_TOGGLE(VK_F10, IsDof);
		//GAME_CONFIG_MEMBER_INPUT_KEY_TOGGLE(VK_F5, IsPrintBound);

		GAME_CONFIG_MEMBER_INPUT_KEY_TOGGLE(VK_F9, IsTimeFast);
	}
};

#define GAMECONFIG GameConfig::GetInstance()