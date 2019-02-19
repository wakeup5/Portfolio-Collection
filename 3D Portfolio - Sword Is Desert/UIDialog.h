#pragma once
#include "UIWindow.h"
#include "UIButton.h"
#include "UIAlram.h"
// 이상하게 렌더가 안지워져서 수동으로 처리.. 화면의 안보이는영역에 배치
#define MOVE_NULL_WINDOW_X 2000
#define MOVE_NULL_WINDOW_Y 10000

class UIShop;
class UIInventory;

class UIDialog :
	public UIWindow
{
private:
	cBaseObject* _npc = NULL;

	UITextBox* _name;
	UITextBox* _answer;

	UIShop* _shop;
	UIInventory* _inven;
	
	int questTextIndex = 0;
	std::string _selectQuestName;
	std::vector<UIButton*> _vecQuestBtn;

	// 해당 UI NPC가 가지고 있는 퀘스트 정보
	NpcInfo::quest questInfo;
	UINT _questTextIndex = 0; // 해당 퀘스트 텍스트의 인덱스 번호(퀘스트 계속을 위한)
	UINT _questIndex = 0; // 해당 퀘스트 텍스트의 인덱스 번호(퀘스트 계속을 위한)
	UINT _uiUpdateIndex = 0;

	bool _isOpenShop = false;
	bool _isNpcSceneMode = false;


public:

	bool isSelect = false;

	UIDialog();
	virtual ~UIDialog();

	void Setup();
	void Update();
	void Render(LPD3DXSPRITE sprite);
	void SetShop(UIShop* shop);
	void SetInventory(UIInventory* inven);
	void SetNPC(cBaseObject* npc) 
	{
		_npc = npc;

		if (_npc != NULL && !_isNpcSceneMode)
		{
			_isNpcSceneMode = true;
			Enable();
		}
		else
		{
			_isNpcSceneMode = false;
			Disible();
		}
	}

	bool IsExistNPC() { return _npc != NULL; }

	void SetQuest();
	void SelectQuest(UINT idx);

	void Enable();
	void Disible();

	void SetUiUpdateIndex(UINT idx){
		_uiUpdateIndex = idx;
	}

	void NextQuestText(); // 퀘스트 계속
	void NoQuest(); // 퀘스트 거절
	void AcceptQuest(); // 퀘스트 수락
	void OpenShop(); //  상점 열기

};

class UIDialogDeligate : public UIDeligate
{
public:
	inline virtual UINT OnClickDown(UIWindow* caller, UIWindow* root) override
	{
		UINT t = caller->GetTagNum();
		int sdf = 0;

		if (caller->GetTagNum() >= 1150 && caller->GetTagNum() <= 1199){
			((UIDialog*)(caller->GetTop()))->SelectQuest(caller->GetTagNum());
			((UIDialog*)(caller->GetTop()))->isSelect = true;
			return RETURN_DEFAULT;
		}

		switch (caller->GetTagNum())
		{
		case UITag::UI_DIALOG_CLOSE:
			((UIDialog*)(caller->GetTop()))->SetNPC(NULL);
			break;
	
		case UITag::UI_DIALOG_BEGIN:
			LOG_MGR->AddLog("처음으로");
			((UIDialog*)(caller->GetTop()))->NoQuest();
			break;

		case UITag::UI_DIALOG_QUEST:
			((UIDialog*)(caller->GetTop()))->SetQuest();
			break;

		case UITag::UI_DIALOG_QUEST_SOCKET :
			LOG_MGR->AddLog("퀘스트 세렉트 클릭 전");
			((UIDialog*)(caller->GetTop()))->SelectQuest(1);
			break;
		
		case UITag::UI_DIALOG_QUEST_NEXT :
			LOG_MGR->AddLog("넥스트 클릭 전");
			((UIDialog*)(caller->GetTop()))->NextQuestText();
			break;

		case UITag::UI_DIALOG_QUEST_ACCEPT:
			((UIDialog*)(caller->GetTop()))->AcceptQuest();
			break;

		case UITag::UI_DIALOG_QUEST_NO:
			((UIDialog*)(caller->GetTop()))->NoQuest();
			break;

		case UITag::UI_SHOP:{
			int dsfsd = 0;
			((UIDialog*)(caller->GetTop()))->OpenShop();
		}
			break;

		default:
			break;
		}

		return RETURN_DEFAULT;
	}
	inline virtual UINT OnMouseOver(UIWindow* caller, UIWindow* root) override
	{
		((UIDialog*)(caller->GetTop()))->SetUiUpdateIndex(caller->GetTagNum());

		return RETURN_DEFAULT;
	}
};