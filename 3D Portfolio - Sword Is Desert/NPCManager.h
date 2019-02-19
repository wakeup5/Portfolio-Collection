#pragma once
#include "Singleton.h"
#include "NPC.h"
#include "Player.h"
#include "CircleLine.h"
#include "NameTag.h"
//#include "Line.h"
#include "cTerrain.h"

class UIDialog;

class NPCManager : public Singleton < NPCManager >
{
public:
	static std::map<std::string, NpcInfo> MapNPC;
	
	std::vector<NPC*> _data;
private:
	Player* _player;

	CircleLine _pCircle;
	CircleLine _nCircle;
	CircleLine _nBigCircle;
	//Line _line;

	UIDialog* _ui;

	NameTag* _nametag;

	cTerrain* _terrain;

	NameTag _npcPos;
public:
	NPCManager();
	virtual ~NPCManager();

	void Setup();
	void Release();
	void Update(float timeDelta);
	void Render();
	void RenderNameTag();
	void MapRender();

	void AddNPC(NpcInfo &info);
	void SetUI(UIDialog* ui){ _ui = ui; }
	void SetPlayer(Player* player) { _player = player; NPC::PlayerTrans = player->pTransform; }
	void SetTerrain(cTerrain* terrain) { _terrain = terrain; }

	
	static std::map<std::string, NpcInfo> GetNpcData(){
		return MapNPC;
	}

	static void SetNpcStatus(std::string name){
		MapNPC.find(name)->second.questList.find(1150)->second.npcQuestStatus = UIQUEST::QUEST_STATUS::QUEST_BEGIN;
		int sdfsd = 0;
	}

	static void LoadNPC();
};

#define NPC_MGR NPCManager::GetInstance()