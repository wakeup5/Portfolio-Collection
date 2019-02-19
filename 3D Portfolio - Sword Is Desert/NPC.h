#pragma once
#include "cBaseObject.h"
#include "cSkinnedAnimation.h"
#include "UICommon.h"

namespace UIQUEST
{
	enum QUEST_STATUS{
		QUEST_BEGIN,
		QUEST_IN_PROGRESS,
		QUEST_END
	};
	enum QUEST_TYPE{
		QUeST_TYPE_KILL,
		QUEST_TYPE_GATHER
	};
}
typedef struct tagNpcInfo
{

	struct quest{
		UIQUEST::QUEST_STATUS npcQuestStatus = UIQUEST::QUEST_STATUS::QUEST_BEGIN;
		UIQUEST::QUEST_TYPE questType;
		int questQTY = 0; // 퀘스트 목표까지 수량
		int questCompensation; // 퀘스트 보상 금액
		std::vector<std::string> questText;
		std::string questName;
		std::string questTargetName;
		
	};

	const char*		ResourcePath;
	char*		Name;
	char*		Answer;
	D3DXVECTOR3	Pos;
	D3DXVECTOR3 Forward;
	D3DXVECTOR3 Scale;
	
	std::map<UINT, quest> questList;
	
	DWORD		Button = UIDB::QUEST | UIDB::SHOP;

	tagNpcInfo() {}
	tagNpcInfo
		(
			char* resourcePath,
			char* name,
			char* answer,
			D3DXVECTOR3 pos
		)
	{
		ResourcePath = resourcePath;
		Name = name;
		Answer = answer;
		Pos = pos;
	}
} NpcInfo;

class NPC :
	public cBaseObject
{

private:
	cSkinnedAnimation* _animation;

	NpcInfo _info;

public:
	static Transform* PlayerTrans;

public:
	NPC();
	~NPC();
	
	virtual void Setup(NpcInfo &info);
	void Release();

	//virtual void BaseObjectEnable();						//BaseObject 가 활성화 될때 실행
	//virtual void BaseObjectDisable();						//BaseObject 가 비활성화 될때 실행
	virtual void BaseObjectUpdate(float timeDelta);			//BaseObject 가 Update 때 실행....
	//virtual void BaseObjectNoActiveUpdate(float timeDelte);	//BaseObject 가 비활성화시 업데이트 실행....
	virtual void BaseObjectRender();

	NpcInfo GetInfo() { return _info; }

	// 퀘스트 상태값 변경
	void SetQuestStatus(UINT idx, UIQUEST::QUEST_STATUS questStat){
		_info.questList.find(idx)->second.npcQuestStatus = questStat;
	}
};

