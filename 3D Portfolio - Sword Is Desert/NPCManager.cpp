#include "stdafx.h"
#include "NPCManager.h"
#include "UIDialog.h"

std::map<std::string, NpcInfo> NPCManager::MapNPC;

bool npcSort(NPC* a, NPC* b)
{
	float distA, distB;
	distA = D3DXVec3Length(&(a->pTransform->GetWorldPosition() - a->PlayerTrans->GetWorldPosition()));
	distB = D3DXVec3Length(&(b->pTransform->GetWorldPosition() - b->PlayerTrans->GetWorldPosition()));

	return distA < distB;
}

NPCManager::NPCManager()
{
}


NPCManager::~NPCManager()
{
}

void NPCManager::Setup()
{
	_data.clear();

	_pCircle.Setup(60, 1.5f);
	_nCircle.Setup(60, 1.5f);
	_nBigCircle.Setup(90, 2.5f);
	//_line.Setup();

	_nametag = new NameTag;

	_nametag->SetTexture(RESOURCE("Textures/icon/quest.png"));
	_nametag->SetPosition(0, 0, 0);
	_nametag->SetRotate(0, 0, 0);
	_nametag->SetScale(0.2, 0.2, 0.2);

	//NPC 위치
	_npcPos.SetTexture(RESOURCE("Textures/Icon/npc.png"));
}

void NPCManager::Release()
{
	for (NPC* n : _data)
	{
		n->Release();
		delete n;
	}

	if(_nametag) _nametag->Release();
	SAFE_DELETE(_nametag);
}

void NPCManager::Update(float timeDelta)
{
	if (_data.size() <= 0 || _player == NULL) return;

	std::sort(_data.begin(), _data.end(), npcSort);

	for (NPC* n : _data)
	{
		n->Update(timeDelta);
	}

	D3DXVECTOR3 pPos = _player->pTransform->GetWorldPosition();
	D3DXVECTOR3 nPos = _data[0]->pTransform->GetWorldPosition();
	if (D3DXVec3Length(&(pPos - nPos)) < 3)
	{
		if (KEY_MGR->IsOnceDown('R'))
		{
			_ui->SetNPC(_data[0]);
		}
	}
	nPos.y += 2.5f;
	_nametag->SetPosition(nPos);
	_nametag->Look(CAMERA->GetForward(), CAMERA->GetUp());

	//_line.SetStart(pPos);
	//_line.SetEnd(nPos);
}

void NPCManager::Render()
{
	if (_data.size() <= 0 || _player == NULL) return;

	for (NPC* n : _data)
	{
		n->Render();
	}
}

void NPCManager::MapRender()
{
	if (_data.size() <= 0 || _player == NULL) return;

	for (NPC* n : _data)
	{
		_npcPos.SetPosition(n->pTransform->GetWorldPosition());
		_npcPos.RenderToSprite(SPRITE, CAMERA);
	}
}

void NPCManager::RenderNameTag()
{
	if (_ui->IsExistNPC()) return;

	float dist = D3DXVec3Length(&(_player->pTransform->GetWorldPosition() - _data[0]->pTransform->GetWorldPosition()));

	_nBigCircle.SetAlpha(1 - dist / 6);
	_pCircle.SetAlpha(1 - dist / 6);
	_nCircle.SetAlpha(1 - dist / 6);

	if (dist < 3)
	{
		D3DXVECTOR3 nPos = _data[0]->pTransform->GetWorldPosition();
		D3DXVECTOR3 pPos = _player->pTransform->GetWorldPosition();
		D3DXVECTOR3 bPos = (nPos + pPos) / 2;

		Figure::Begin();
		_nBigCircle.pTransform.SetWorldPosition(bPos.x, bPos.y + 1, bPos.z);
		_nBigCircle.Render();
		Figure::End();

		_nametag->Render();
	}
	else if (dist < 6)
	{
		D3DXVECTOR3 nPos = _data[0]->pTransform->GetWorldPosition();
		_nCircle.pTransform.SetWorldPosition(nPos.x, nPos.y + 1, nPos.z);

		D3DXVECTOR3 pPos = _player->pTransform->GetWorldPosition();
		_pCircle.pTransform.SetWorldPosition(pPos.x, pPos.y + 1, pPos.z);

		Figure::Begin();
		_nCircle.Render();
		_pCircle.Render();
		Figure::End();
	}
}

void NPCManager::AddNPC(NpcInfo & info)
{
	NPC* n = new NPC;

	n->Setup(info);
	n->SetActive(true);

	_data.push_back(n);

	D3DXVECTOR3 pos = n->pTransform->GetWorldPosition();
	n->pTransform->SetWorldPosition(pos.x, _terrain->GetHeight(pos.x, pos.z), pos.z);

	n->pTransform->LookDirection(info.Forward);
	n->pTransform->SetScale(info.Scale);
}

void NPCManager::LoadNPC()
{
	MapNPC.clear();

	NpcInfo info;

	NpcInfo::quest quest;
	
	

	info = NpcInfo(RESOURCE("Meshes/NPC/NPC01.X"), "Tom", "안녕하신가.", D3DXVECTOR3(0, 0, 0));

	quest.questText.clear();
	quest.questText.push_back("사악한 마법사가 마을의 밭에 마법을 걸었습니다.");
	quest.questText.push_back("그 마법의 여파로 마을의 호박들이 괴물로 변해 사람들을 해치고있습니다.");
	quest.questText.push_back("호박들을 제거해주세요.");
	quest.questName = "펌프킨 퇴치";
	quest.questTargetName = "Pumpkin";
	quest.questQTY = 10;
	info.questList.insert({ 1150, quest }); // 퀘스트 소켓번호 시작

	MapNPC.insert(std::make_pair(info.Name, info));

	info = NpcInfo(RESOURCE("Meshes/NPC/NPC02.X"), "Jin", "안녕하신가.", D3DXVECTOR3(0, 0, 0));
	quest.questText.clear();
	quest.questText.push_back("숫자가 크게 늘어난 Muspel들이 마을의 위험요소가 되었습니다.");
	quest.questText.push_back("Muspel의 숫자를 줄여주세요.");
	quest.questText.push_back("부탁드립니다. 모험가님.");
	quest.questName = "Muspel 퇴치";
	quest.questTargetName = "Muspel";
	quest.questQTY = 3;
	info.questList.insert({ 1160, quest }); // 퀘스트 소켓번호 시작
	
	quest.questText.clear();
	quest.questText.push_back("한달전부터 시체들이 일어나기 시작했습니다.");
	quest.questText.push_back("세계의 이치를 거스르는 이러한 시체들을 제거해주세요.");
	quest.questText.push_back("부탁드립니다. 모험가님.");
	quest.questName = "해골 퇴치";
	quest.questTargetName = "SkullWarrior";
	quest.questQTY = 3;
	info.questList.insert({ 1161, quest });
	info.Button = UIDB::SHOP;
	MapNPC.insert(std::make_pair(info.Name, info));

	info = NpcInfo(RESOURCE("Meshes/NPC/NPC03.X"), "Kings", "안녕하신가.", D3DXVECTOR3(0, 0, 0));
	quest.questText.clear();
	quest.questText.push_back("근처 숲에 오크들의 족장 가로쉬가 주변 오크들을 결집시키고있습니다.  ");
	quest.questText.push_back("가로쉬를 제거하고 오크들을 분산시켜주세요.");
	quest.questName = "가로쉬 퇴치";
	quest.questTargetName = "Orc";
	quest.questQTY = 1;
	info.questList.insert({ 1170, quest }); // 퀘스트 소켓번호 시작

	MapNPC.insert(std::make_pair(info.Name, info));

	info = NpcInfo(RESOURCE("Meshes/NPC/NPC04.X"), "Bob", "안녕하신가.", D3DXVECTOR3(0, 0, 0));
	quest.questText.clear();
	quest.questText.push_back("그동안 모든 일들의 배후에는 강대한 괴물 Seth가 있었던것이");
	quest.questText.push_back("밝혀졌습니다. Seth로 가는 길을 확보하기위해 골렘들을 쓰러드리세요.");
	quest.questName = "골렘 퇴치";
	quest.questTargetName = "Golem";
	quest.questQTY = 10;
	info.questList.insert({ 1180, quest }); // 퀘스트 소켓번호 시작
	MapNPC.insert(std::make_pair(info.Name, info));

	info = NpcInfo(RESOURCE("Meshes/NPC/NPC05.X"), "Dan", "안녕하신가.", D3DXVECTOR3(0, 0, 0));
	quest.questText.clear();
	quest.questText.push_back("그동안의 악행에 최후를 가할때가 되었습니다.");
	quest.questText.push_back("Seth를 처치하고 오세요.");
	quest.questText.push_back("부탁드립니다. 모험가님.");
	quest.questName = "Seth 퇴치";
	quest.questTargetName = "Seth";
	quest.questQTY = 1;
	info.questList.insert({ 1190, quest });
	info.Button = UIDB::QUEST;
	MapNPC.insert(std::make_pair(info.Name, info));
}
