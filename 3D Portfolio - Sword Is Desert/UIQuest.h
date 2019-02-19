#pragma once
#include "UIWindow.h"
#include "UISocket.h"
#include "Player.h"

#define MOVE_NULL_PT_X 10000
#define MOVE_NULL_PT_Y 10000
class UIQuest : public UIWindow
{
private:
	UIDeligateDefault* defaultDeligate;
	Player* _playerInfo;
	int _clickQuestIndex = -1;
	bool _isOpenQuest = false;

public:
	UIQuest();
	virtual ~UIQuest();

	void Setup();
	void Release();
	void Update();
	void Render(LPD3DXSPRITE sprite);
	void LoadQuest();
	void LoadQuestInfo();
	void SetClickQuestIndex(int clickIndex);
	void DeleteQuest();
	void GiveUpQuest();
	void QuestComplete();
	
};



class UIQuestSocketDeligate : public UISocketDeligate
{
public:
	//inline virtual UINT OnMouseOver(UIWindow* caller, UIWindow* root) override;
	inline virtual UINT OnMouseOver(UIWindow* caller, UIWindow* root) override
	{
	//	((UIDialog*)(caller->GetTop()))->SetUiUpdateIndex(caller->GetTagNum());

		return RETURN_DEFAULT;
	}

	inline virtual UINT OnClickDown(UIWindow* caller, UIWindow* root) override
	{
		UISocket* cs = (UISocket*)caller;
		UISocket* ms = (UISocket*)root->GetWindowByTag(UITag::UI_MOUSE_SOCKET);
		// 퀘스트포기
		if (cs->GetTagNum() == UITag::UI_DIALOG_QUEST_GIVE_UP){
			((UIQuest*)(caller->GetTop()))->DeleteQuest();
			return RETURN_DEFAULT;
		}
		//퀘스트를 받으면
		if (cs->GetTagNum() >= 1150 && cs->GetTagNum() < 1200){
			LOG_MGR->AddLog("%d", cs->GetTagNum());
			((UIQuest*)(caller->GetTop()))->SetClickQuestIndex(cs->GetTagNum());
		}
		return RETURN_DEFAULT;
	}
};
