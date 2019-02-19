#pragma once
#include "Markup.h"

typedef std::pair<std::string, cBaseObject*> StrObjPair;
typedef std::vector<StrObjPair> StrObjVector;

class MapDataSaveLoad
{
private:
	StrObjVector _objData;
	Transform _playerData;
	StrObjVector _monsterData;
	StrObjVector _npcData;

public:
	void SetObjectData(StrObjVector objList) { _objData = objList; }
	void SetPlayerData(cBaseObject* player) { _playerData.SetWorldPosition(player->pTransform->GetWorldPosition()); }
	void SetMonsterData(StrObjVector monList) { _monsterData = monList; }
	void SetNpcData(StrObjVector npcList) { _npcData = npcList; }

	void SaveData(std::string filePath);
	void LoadData(std::string filePath);
	void LoadData(std::string filePath, OUT StrObjVector* vecObj);

	void GetObjectData(OUT StrObjVector* vecObj)
	{
		//vecObj->assign(_objData.begin(), _objData.end());
		//std::copy(_objData.begin(), _objData.end(), vecObj);
		for (int i = 0; i < _objData.size(); i++)
		{
			vecObj->push_back(_objData[i]);
		}
	}
	void GetObjectData(OUT std::vector<cBaseObject*>* vecObj)
	{
		for (int i = 0; i < _objData.size(); i++)
		{
			vecObj->push_back(_objData[i].second);
		}
	}
	void GetPlayerData(OUT cBaseObject* player)
	{
		if (!player) return;

		player->pTransform->SetWorldPosition(_playerData.GetWorldPosition());
	}
	void GetMonsterData(OUT StrObjVector* vecMon)
	{
		//vecMon->assign(_monsterData.begin(), _monsterData.end());
		//std::copy(_monsterData.begin(), _monsterData.end(), vecMon);
		vecMon->clear();
		for (int i = 0; i < _monsterData.size(); i++)
		{
			vecMon->push_back(_monsterData[i]);
		}
	}
	void GetNpcData(OUT StrObjVector* vecNpc)
	{
		vecNpc->clear();
		for (int i = 0; i < _npcData.size(); i++)
		{
			vecNpc->push_back(_npcData[i]);
		}
	}
	
	void CreateMonster();
	void CreateNpc();

	void ClearData();
};

