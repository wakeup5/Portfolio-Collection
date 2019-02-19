#pragma once
#include "Singleton.h"
#include "UIAlram.h"

struct PLAYER_QUEST{
	UIQUEST::QUEST_STATUS npcQuestStatus = UIQUEST::QUEST_STATUS::QUEST_BEGIN;
	std::string questText;
	std::string questName;
	std::string questNpcName;
	std::string questTargetName;
	int questNo;
	int npcIndex;
	NPC** questNpc;
	NpcInfo* nInfo;
	int questQty = 0;
	int questInProgressQty = 0;
};
class QuestManager : public Singleton<QuestManager>
{
private:
	

	std::map<UINT, PLAYER_QUEST> _questList;
	std::string _targetName;
	int _progressQuestNo = -1;

public:
	QuestManager();
	virtual ~QuestManager();
	
	void AddPlayerQuest(UINT questIndex, PLAYER_QUEST playerQuestInfo);
	void DeletePlayerQuest(UINT questIndex); // 퀘스트 완료 후 삭제
	void GiveUpPlayerQuest(UINT questIndex); // 퀘스트 포기
	// 퀘스트 진행중 업데이트
	void QuestProgressPlus(UINT questIndex){
		PLAYER_QUEST* progressQuest = &_questList.find(questIndex)->second;
		_questList.find(questIndex)->second.questInProgressQty++;
		if (progressQuest->questInProgressQty == progressQuest->questQty){
			progressQuest->npcQuestStatus = UIQUEST::QUEST_STATUS::QUEST_END;
		}

	}

	void QuestProgressPlus(std::string questTargetName){

		PLAYER_QUEST* target;
		int targetNo = -1;
		for (auto i = _questList.begin(); i != _questList.end(); ++i){
			if ((i)->second.questTargetName == questTargetName){
				target = &(i)->second;
				_progressQuestNo = target->questNo;
				break;
			}
			else{
				_progressQuestNo = -1;
			}
		
		}
		if (_progressQuestNo != -1){
			target->questInProgressQty++;
			
			UIAlram* alram = UIAlram::GetInstance();
			alram->SetHiddenState(false);
			std::string concats = "\n" + QuestManager::GetInstance()->GetPlayerQuest(_progressQuestNo).questTargetName + "  " +
				std::to_string(QuestManager::GetInstance()->GetPlayerQuest(_progressQuestNo).questInProgressQty) + "  / " +
				std::to_string(QuestManager::GetInstance()->GetPlayerQuest(_progressQuestNo).questQty);
			alram->SetText(concats
				);
			alram->SetAlphaBlend(255);
			if (target->questInProgressQty == target->questQty){
				target->npcQuestStatus = UIQUEST::QUEST_STATUS::QUEST_END;
			}
		}
		else{
			_progressQuestNo = -1;
		}
		
		
	}

	void SetTargetName(std::string name){
		_targetName = name;
		QuestProgressPlus(_targetName);
	}

	int GetProgressQuestNo(){
		return _progressQuestNo;
	}

	void SetProgressQuestNo(int questIdx){
		_progressQuestNo = questIdx;
	}

	// 퀘스트 완료
	void QuestComplete(UINT questIndex);
	PLAYER_QUEST GetPlayerQuest(UINT questIndex);
	std::map<UINT, PLAYER_QUEST> GetPlayerQuestList();
	int GetLength(){ return _questList.size(); }

	UIQUEST::QUEST_STATUS GetNpcQuestStatus(UINT idx){
		return _questList.find(idx)->second.npcQuestStatus;
	}
};

