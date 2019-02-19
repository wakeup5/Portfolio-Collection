#include "stdafx.h"
#include "SoundManager.h"



SoundManager::~SoundManager()
{
}

HRESULT SoundManager::initialize(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//채널수
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드와 채널 동적할당
	//_sound = new Sound*[TOTALSOUNDBUFFER];
	//_channel = new Channel*[TOTALSOUNDBUFFER];

	//멤셋~~
	//memset(_sound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	//memset(_channel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	_soundMap.clear();
	_channelMap.clear();

	return S_OK;
}

void SoundManager::release(void)
{
	//사운드 삭제한당~
	/*
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound != NULL) _sound[i]->release();
			}
		}
	}

	//메모리 지운다
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);
	*/

	for (SoundMapIter i = _soundMap.begin(); i != _soundMap.end();)
	{
		i->second->release();
		i = _soundMap.erase(i);
	}

	for (ChannelMapIter i = _channelMap.begin(); i != _channelMap.end();)
	{
		i->second->stop();
		i = _channelMap.erase(i);
	}

	//시스템 닫는다
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}

	_soundMap.clear();
	_channelMap.clear();
}


void SoundManager::loadDefaultSound(){
	// 인벤토리
	addSound("ring", "../Resources/bgm/inventory_sound_effects/ring_inventory.wav", false, false);
	addSound("armor", "../Resources/bgm/inventory_sound_effects/cloth-inventory.wav", false, false);
	addSound("weapon", "../Resources/bgm/inventory_sound_effects/leather_inventory.wav", false, false);
	addSound("sell_buy_item", "../Resources/bgm/inventory_sound_effects/sell_buy_item.wav", false, false);
	addSound("inventory_open", "../Resources/bgm/inventory_sound_effects/Inventory_Open_00.wav", false, false);

	// 타격
	addSound("sword_hit1", "../Resources/bgm/hit/Socapex - new_hits_1.wav", false, false);
	addSound("sword_hit2", "../Resources/bgm/hit/Socapex - new_hits_2.wav", false, false);
	addSound("sword_hit3", "../Resources/bgm/hit/Socapex - new_hits_3.wav", false, false);
	addSound("sword_hit4", "../Resources/bgm/hit/Socapex - new_hits_4.wav", false, false);

	// 타격음(가죽)
	addSound("hit_leather01", "../Resources/bgm/타격음/가죽/Be Attack Leather_01.mp3", false, false);
	addSound("hit_leather02", "../Resources/bgm/타격음/가죽/Be Attack Leather_02.mp3", false, false);
	addSound("hit_leather03", "../Resources/bgm/타격음/가죽/Be Attack Leather_03.mp3", false, false);

	// 타격음(금속)
	addSound("hit_metal01", "../Resources/bgm/타격음/금속/Be Attack Metal_01.mp3", false, false);
	addSound("hit_metal02", "../Resources/bgm/타격음/금속/Be Attack Metal_02.mp3", false, false);
	addSound("hit_metal03", "../Resources/bgm/타격음/금속/Be Attack Metal_03.mp3", false, false);
	addSound("hit_metal04", "../Resources/bgm/타격음/금속/Be Attack Metal_04.mp3", false, false);

	// 보스 지면 어택
	addSound("Attack_Ground_01", "../Resources/bgm/보스지면어택/ultimate_ground.mp3", false, false);
	addSound("Attack_Ground_03", "../Resources/bgm/보스지면어택/Attack_Ground_03.mp3", false, false);

	// 방패스킬
	addSound("shield_hit", "../Resources/bgm/skil/방패/04_riven_skin05_feint_shield_oc_.wav", false, false);

	// 불
	addSound("fire01", "../Resources/bgm/skil/불/Great_Straight_02_Fire.mp3", false, false);
	addSound("fire02", "../Resources/bgm/skil/불/Great_Straight_03_Fire.mp3", false, false);
	addSound("fire03", "../Resources/bgm/skil/불/Great_Straight_04_Fire.mp3", false, false);
	addSound("fire04", "../Resources/bgm/skil/불/Great_Straight_05_Fire.mp3", false, false);
	addSound("swordFire", "../Resources/bgm/skil/불/swordFire.mp3", false, false);

	// 구르기
	addSound("sand", "../Resources/bgm/skil/구르기/sand.mp3", false, false);
	addSound("thumble", "../Resources/bgm/skil/구르기/thumble.mp3", false, false);

	// 독
	addSound("poison", "../Resources/bgm/skil/독/poison_spell.mp3", false, false);

	// 오크 맞는 소리
	addSound("orc_hit", "../Resources/bgm/middle_boss/orc_hit.mp3", false, false);
	// 오크 공격시 보이스
	addSound("orc_voice", "../Resources/bgm/middle_boss/Boss_1_05_OrcWarrior_Melee_V_Atk_01.mp3", false, false);

	// 스윙
	addSound("swing", "../Resources/bgm/skil/스윙/swing.mp3", false, false);

	// 활
	addSound("bow", "../Resources/bgm/bow/bow.mp3", false, false);

	//버프
	addSound("buff", "../Resources/bgm/skil/buff/buff.mp3", false, false);

	// 레벨업
	addSound("levelUp", "../Resources/bgm/levelup/levelup.wav", false, false);

	// 퀘스트 완료
	addSound("questComplete", "../Resources/bgm/quest/questComplete.mp3", false, false);
	// 퀘스트 수락
	addSound("quest", "../Resources/bgm/quest/quest.mp3", false, false);
	// 퀘스트 상세 넘기기
	addSound("questNext", "../Resources/bgm/quest/bookNext.mp3", false, false);

	// 보스 샤우팅
	addSound("boss_shooting", "../Resources/bgm/boss/boss_shooting.mp3", false, false);

	// 플레이어 보이스
	addSound("보이스1", "../Resources/bgm/플레이어보이스/Bango_V_Atk_Long_Shot_03.mp3", false, false);
	addSound("보이스2", "../Resources/bgm/플레이어보이스/Bango_V_Atk_Long_Shot_05.mp3", false, false);
	addSound("보이스3", "../Resources/bgm/플레이어보이스/Bango_V_Atk_Long_Shot_06.mp3", false, false);

	// 이동
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);
	addSound("foot_step01", "../Resources/bgm/footStep/Footstep_Dirt_00.wav", false, false);

	// 일반이동
	addSound("foot_step_normal01", "../Resources/bgm/footStep/normal/FootStep_01.wav", false, false);
	addSound("foot_step_normal02", "../Resources/bgm/footStep/normal/FootStep_02.wav", false, false);
	addSound("foot_step_normal03", "../Resources/bgm/footStep/normal/FootStep_03.wav", false, false);
	addSound("foot_step_normal04", "../Resources/bgm/footStep/normal/FootStep_04.wav", false, false);

	// 대시이동
	addSound("FootStep_Dash_01", "../Resources/bgm/footStep/dash/FootStep_Dash_01.wav", false, false);
	addSound("foot_step_normal02", "../Resources/bgm/footStep/normal/FootStep_02.wav", false, false);
	addSound("foot_step_normal03", "../Resources/bgm/footStep/normal/FootStep_03.wav", false, false);
	addSound("foot_step_normal04", "../Resources/bgm/footStep/normal/FootStep_04.wav", false, false);

	// 탈것
	addSound("horseRiding", "../Resources/bgm/riding/horseRiding.mp3", false, false);
	addSound("move", "../Resources/bgm/riding/move.mp3", false, false);

	// 날씨음
	addSound("weather rain", "../Resources/bgm/날씨음/rainning.mp3", false, false);
	addSound("weather thunder", "../Resources/bgm/날씨음/Thunderstorm+sound+effect+-+close.mp3", false, false);
	//addSound("thunder_strong_effect_close", "../Resources/bgm/날씨음/Thunderstorm+sound+effect+-+close.mp3", false, false);

	// bgm
	addSound("bgm", "../Resources/bgm/Black+Desert+Online+OST+-+케플란++Keplan.mp3", true, true);
	addSound("bossBgm", "../Resources/bgm/검은사막+BGM002.mp3", true, true);
	addSound("ridingBgm", "../Resources/bgm/hh.mp3", true, true);
	//addSound("middleBossBgm", "../Resources/bgm/검은사막+BGM002.mp3", true, true);
}


//사운드 추가한다
void SoundManager::addSound(string keyName, string soundName,
	bool background, bool loop)
{
	//뺑뻉이냐?
	if (loop)
	{
		//브금이냐?
		if (background)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound);
		}
	}
	else //뺑뺑이가 아니면..
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound);
	}

	//맵에 사운드를 키값과 함께 넣어준다
	_soundMap.insert(make_pair(keyName, _sound));
}

//사운드 플레이~~
void SoundManager::play(string keyName, float volume)
{
	SoundMapIter iter = _soundMap.find(keyName);
	ChannelMapIter cIter = _channelMap.find(keyName);

	if (iter != _soundMap.end())
	{
		{
			_system->playSound(FMOD_CHANNEL_FREE, iter->second, false, &_channel);

			_channelMap.insert(make_pair(keyName, _channel));
		}
		_channel->setVolume(volume);
	}

}

//현재 재생중인 사운드를 일시정지 시킴
void SoundManager::pause(string keyName)
{
	ChannelMapIter iter = _channelMap.find(keyName);

	if (iter != _channelMap.end())
	{
		iter->second->setPaused(true);
	}

}

//일시정지된 사운드를 다시 재생한당..
void SoundManager::resume(string keyName)
{
	ChannelMapIter iter = _channelMap.find(keyName);

	if (iter != _channelMap.end())
	{
		iter->second->setPaused(false);
	}

}

void SoundManager::stop(string keyName)
{
	ChannelMapIter iter = _channelMap.find(keyName);

	if (iter != _channelMap.end())
	{
		iter->second->stop();
		_channelMap.erase(iter);
	}

}

void SoundManager::update(void)
{
	//사운드 시스템을 계속 업데이트한다..
	_system->update();

	bool isPlay;
	for (ChannelMapIter i = _channelMap.begin(); i != _channelMap.end();)
	{
		isPlay = false;
		i->second->isPlaying(&isPlay);

		if (!isPlay)
		{
			i = _channelMap.erase(i);
		}
		else
		{
			i++;
		}
	}
}


bool SoundManager::isPlay(string keyName){
	bool isPlay = false;
	ChannelMapIter iter = _channelMap.find(keyName);

	if (iter != _channelMap.end())
	{
		iter->second->isPlaying(&isPlay);
		//_channelMap.erase(iter);
	}
	else{
		play(keyName, 0.2f);
	}

	return isPlay;
}


bool SoundManager::isPlay(string keyName, float volume){
	bool isPlay = false;
	ChannelMapIter iter = _channelMap.find(keyName);

	if (iter != _channelMap.end())
	{
		iter->second->isPlaying(&isPlay);
		//_channelMap.erase(iter);
	}
	else{
		play(keyName, volume);
	}

	return isPlay;
}



bool SoundManager::isBossBgm(){
	bool isPlay = false;
	ChannelMapIter iter = _channelMap.find("bossBgm");

	if (iter != _channelMap.end())
	{
		iter->second->isPlaying(&isPlay);
		isPlay = true;
		//_channelMap.erase(iter);
	}


	return isPlay;
}