#include "stdafx.h"
#include "MapDataSaveLoad.h"

void MapDataSaveLoad::SaveData(std::string filePath)
{
	//if (_objData.size() == 0) return;

	D3DXVECTOR3 pos, scale, forward;
	Transform* trans;

	CMarkup xml;
	xml.SetDoc("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n\n"); //XML 인코딩 설정입니다.

	//////////////////////////////오브젝트////////////////////////////////////
	xml.AddElem("OBJECTS");
	xml.IntoElem();

	for (int i = 0; i < _objData.size(); i++)
	{
		trans = _objData[i].second->pTransform;

		xml.AddElem("OBJECT");
		xml.AddChildElem("RESOURCENAME", _objData[i].first + "");

		pos = trans->GetWorldPosition();
		xml.AddChildElem("POS");
		xml.IntoElem();
		xml.AddChildElem("X", std::to_string(pos.x));
		xml.AddChildElem("Y", std::to_string(pos.y));
		xml.AddChildElem("Z", std::to_string(pos.z));
		xml.OutOfElem();

		scale = trans->GetScale();
		xml.AddChildElem("SCALE");
		xml.IntoElem();
		xml.AddChildElem("X", std::to_string(scale.x));
		xml.AddChildElem("Y", std::to_string(scale.y));
		xml.AddChildElem("Z", std::to_string(scale.z));
		xml.OutOfElem();

		forward = trans->GetForward();
		xml.AddChildElem("FORWARD");
		xml.IntoElem();
		xml.AddChildElem("X", std::to_string(forward.x));
		xml.AddChildElem("Y", std::to_string(forward.y));
		xml.AddChildElem("Z", std::to_string(forward.z));
		xml.OutOfElem();
	}

	//////////////////////////////플레이어////////////////////////////////////
	xml.ResetPos();
	xml.AddElem("PLAYER");

	pos = _playerData.GetWorldPosition();

	xml.AddChildElem("POS");
	xml.IntoElem();
	xml.AddChildElem("X", std::to_string(pos.x));
	xml.AddChildElem("Y", std::to_string(pos.y));
	xml.AddChildElem("Z", std::to_string(pos.z));
	xml.OutOfElem();

	//////////////////////////////몬스터////////////////////////////////////
	xml.ResetPos();
	xml.AddElem("MONSTERS");
	xml.IntoElem();

	for (int i = 0; i < _monsterData.size(); i++)
	{
		xml.AddElem("MONSTER");
		xml.AddChildElem("RESOURCENAME", _monsterData[i].first + "");

		pos = _monsterData[i].second->pTransform->GetWorldPosition();

		xml.AddChildElem("POS");
		xml.IntoElem();
		xml.AddChildElem("X", std::to_string(pos.x));
		xml.AddChildElem("Y", std::to_string(pos.y));
		xml.AddChildElem("Z", std::to_string(pos.z));
		xml.OutOfElem();
	}

	//////////////////////////////NPC////////////////////////////////////
	xml.ResetPos();
	xml.AddElem("NPCS");
	xml.IntoElem();

	for (int i = 0; i < _npcData.size(); i++)
	{
		trans = _npcData[i].second->pTransform;

		xml.AddElem("NPC");
		xml.AddChildElem("RESOURCENAME", _npcData[i].first + "");

		pos = trans->GetWorldPosition();
		xml.AddChildElem("POS");
		xml.IntoElem();
		xml.AddChildElem("X", std::to_string(pos.x));
		xml.AddChildElem("Y", std::to_string(pos.y));
		xml.AddChildElem("Z", std::to_string(pos.z));
		xml.OutOfElem();

		scale = trans->GetScale();
		xml.AddChildElem("SCALE");
		xml.IntoElem();
		xml.AddChildElem("X", std::to_string(scale.x));
		xml.AddChildElem("Y", std::to_string(scale.y));
		xml.AddChildElem("Z", std::to_string(scale.z));
		xml.OutOfElem();

		forward = trans->GetForward();
		xml.AddChildElem("FORWARD");
		xml.IntoElem();
		xml.AddChildElem("X", std::to_string(forward.x));
		xml.AddChildElem("Y", std::to_string(forward.y));
		xml.AddChildElem("Z", std::to_string(forward.z));
		xml.OutOfElem();
	}

	xml.Save(filePath);
}

void MapDataSaveLoad::LoadData(std::string filePath)
{
	LOG_MGR->AddLog("MapDataSaveLoad : 기존 데이터 초기화");
	ClearData();

	cBaseObject* temp;
	D3DXVECTOR3 pos, scale, forward;

	CMarkup xml;
	xml.Load(filePath);

	//////////////////////////////오브젝트////////////////////////////////////
	LOG_MGR->AddLog("MapDataSaveLoad : 오브젝트 로딩 생성");
	std::string name;
	xml.FindElem("OBJECTS");

	while (xml.FindChildElem("OBJECT"))
	{
		xml.IntoElem();

		xml.FindChildElem("RESOURCENAME"); name = xml.GetChildData();

		xml.FindChildElem("POS");
		xml.IntoElem();
		xml.FindChildElem("X"); pos.x = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Y"); pos.y = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Z"); pos.z = atof(xml.GetChildData().c_str());
		xml.OutOfElem();

		xml.FindChildElem("SCALE");
		xml.IntoElem();
		xml.FindChildElem("X"); scale.x = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Y"); scale.y = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Z"); scale.z = atof(xml.GetChildData().c_str());
		xml.OutOfElem();

		xml.FindChildElem("FORWARD");
		xml.IntoElem();
		xml.FindChildElem("X"); forward.x = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Y"); forward.y = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Z"); forward.z = atof(xml.GetChildData().c_str());
		xml.OutOfElem();

		xml.OutOfElem();

		temp = new cBaseObject;
		temp->SetMesh(RESOURCE_STATICXMESH->GetResource(name));
		temp->pTransform->SetWorldPosition(pos);
		temp->pTransform->SetScale(scale);
		temp->pTransform->LookDirection(forward, D3DXVECTOR3(0, 1, 0));
		temp->SetActive(true);

		_objData.push_back(make_pair(name, temp));
	}

	///////////////////////////////플레이어////////////////////////////////////////
	LOG_MGR->AddLog("MapDataSaveLoad : 플레이어 로딩");
	xml.ResetPos();
	xml.FindElem("PLAYER");

	xml.FindChildElem("POS");
	xml.IntoElem();
	xml.FindChildElem("X"); pos.x = atof(xml.GetChildData().c_str());
	xml.FindChildElem("Y"); pos.y = atof(xml.GetChildData().c_str());
	xml.FindChildElem("Z"); pos.z = atof(xml.GetChildData().c_str());
	xml.OutOfElem();

	_playerData.SetWorldPosition(pos);

	///////////////////////////////몬스터/////////////////////////////////////////
	LOG_MGR->AddLog("MapDataSaveLoad : 몬스터 로딩");
	xml.ResetPos();
	xml.FindElem("MONSTERS");

	while (xml.FindChildElem("MONSTER"))
	{
		name.clear();

		xml.IntoElem();

		xml.FindChildElem("RESOURCENAME"); name = xml.GetChildData();

		xml.FindChildElem("POS");
		xml.IntoElem();
		xml.FindChildElem("X"); pos.x = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Y"); pos.y = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Z"); pos.z = atof(xml.GetChildData().c_str());
		xml.OutOfElem();

		xml.OutOfElem();

		temp = new cBaseObject;
		temp->SetMesh(RESOURCE_SKINNEDXMESH->GetResource(MonsterManager::MapMonster.find(name.c_str())->second.ResourcePath));
		temp->pTransform->SetWorldPosition(pos);
		temp->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(20, 2, 20));
		temp->SetActive(true);

		_monsterData.push_back(std::make_pair(name, temp));
	}

	//////////////////////////////NPC////////////////////////////////////
	LOG_MGR->AddLog("MapDataSaveLoad : NPC 로딩");
	xml.ResetPos();
	xml.FindElem("NPCS");

	while (xml.FindChildElem("NPC"))
	{
		xml.IntoElem();

		xml.FindChildElem("RESOURCENAME"); name = xml.GetChildData();

		xml.FindChildElem("POS");
		xml.IntoElem();
		xml.FindChildElem("X"); pos.x = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Y"); pos.y = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Z"); pos.z = atof(xml.GetChildData().c_str());
		xml.OutOfElem();

		xml.FindChildElem("SCALE");
		xml.IntoElem();
		xml.FindChildElem("X"); scale.x = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Y"); scale.y = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Z"); scale.z = atof(xml.GetChildData().c_str());
		xml.OutOfElem();

		xml.FindChildElem("FORWARD");
		xml.IntoElem();
		xml.FindChildElem("X"); forward.x = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Y"); forward.y = atof(xml.GetChildData().c_str());
		xml.FindChildElem("Z"); forward.z = atof(xml.GetChildData().c_str());
		xml.OutOfElem();

		xml.OutOfElem();

		temp = new cBaseObject;
		temp->SetMesh(RESOURCE_SKINNEDXMESH->GetResource(NPCManager::MapNPC.find(name.c_str())->second.ResourcePath));
		temp->pTransform->SetWorldPosition(pos);
		temp->pTransform->SetScale(scale);
		temp->pTransform->LookDirection(forward, D3DXVECTOR3(0, 1, 0));
		temp->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(20, 40, 20));
		temp->SetActive(true);

		_npcData.push_back(make_pair(name, temp));
	}

	LOG_MGR->AddLog("MapDataSaveLoad : 로딩 완료");
}

void MapDataSaveLoad::LoadData(std::string filePath, OUT StrObjVector* vecObj)
{
	LoadData(filePath);
	GetObjectData(vecObj);
}

void MapDataSaveLoad::CreateMonster()
{
	for (int i = 0; i < _monsterData.size(); i++)
	{
		MonsterInfo mInfo;
		RECT area;
		D3DXVECTOR3 pos;

		if (MonsterManager::MapMonster.find(_monsterData[i].first) == MonsterManager::MapMonster.end()) continue;

		mInfo = MonsterManager::MapMonster[_monsterData[i].first];
		//mInfo.ResourcePath = _monsterData[i].first.c_str();
		pos = _monsterData[i].second->pTransform->GetWorldPosition();
		area.left = pos.x - 10; area.top = pos.z - 10; area.right = pos.x + 10; area.bottom = pos.z + 10;
		mInfo.Area = area;

		MonsterManager::GetInstance()->CreateMonster(mInfo);
	}
}

void MapDataSaveLoad::CreateNpc()
{
	for (int i = 0; i < _npcData.size(); i++)
	{
		NpcInfo nInfo;
		D3DXVECTOR3 pos;

		if (NPCManager::MapNPC.find(_npcData[i].first) == NPCManager::MapNPC.end()) continue;

		nInfo = NPCManager::MapNPC[_npcData[i].first];
		nInfo.Pos = _npcData[i].second->pTransform->GetWorldPosition();
		nInfo.Scale = _npcData[i].second->pTransform->GetScale();
		nInfo.Forward = _npcData[i].second->pTransform->GetForward();

		NPCManager::GetInstance()->AddNPC(nInfo);
	}
}

void MapDataSaveLoad::ClearData()
{
	for (int i = 0; i < _monsterData.size(); i++)
	{
		delete _monsterData[i].second;
	}

	_monsterData.clear();

	for (int i = 0; i < _objData.size(); i++)
	{
		delete _objData[i].second;
	}

	_objData.clear();

	for (int i = 0; i < _npcData.size(); i++)
	{
		delete _npcData[i].second;
	}

	_npcData.clear();
}
