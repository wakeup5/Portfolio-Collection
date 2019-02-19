#include "stdafx.h"
#include "UIDialog.h"
#include "UIShop.h"
#include "UIInventory.h"

UIButton* nextBtn;
UIButton* _acceptBtn;
UIButton* _noBtn;
UIButton* _shopBtn;
UIDialogDeligate* _UIDialogDeligate;
bool isInitDialog = false;
UIButton* _exitBtn;
UIButton* _beginBtn;
UIButton* _questBtn;
UIDialog::UIDialog()
{
}


UIDialog::~UIDialog()
{
}

void UIDialog::Setup()
{
	UIWindow::Setup();

	SetSize(WINSIZE_X, 300);
	SetBackground("../Resources/Textures/ui/ui_background.png");
	SetPosition(0, WINSIZE_Y - 300);

	_UIDialogDeligate = new UIDialogDeligate;

	_exitBtn = new UIButton;
	_exitBtn->Setup();
	_exitBtn->SetButtonTexture(
		"../Resources/Textures/Icon/Button/dialog_end.png",
		"../Resources/Textures/Icon/Button/dialog_end.png",
		"../Resources/Textures/Icon/Button/dialog_end_down.png");
	_exitBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(151, 29)
		->SetPosition(WINSIZE_X / 2 + 120, 50)
		->SetDeligate(_UIDialogDeligate)
		->SetTagNum(UITag::UI_DIALOG_CLOSE);
	
	_beginBtn = new UIButton;
	_beginBtn->Setup();
	_beginBtn->SetButtonTexture(
		"../Resources/Textures/Icon/Button/begin.png",
		"../Resources/Textures/Icon/Button/begin.png",
		"../Resources/Textures/Icon/Button/begin_down.png");
	_beginBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(151, 29)
		->SetPosition(_exitBtn->GetPosition().x-400, _exitBtn->GetPosition().y)
		->SetDeligate(_UIDialogDeligate)
		->SetTagNum(UITag::UI_DIALOG_BEGIN);

	_questBtn = new UIButton;
	_questBtn->Setup();
	_questBtn->SetButtonTexture(
		"../Resources/Textures/Icon/Button/quest.png",
		"../Resources/Textures/Icon/Button/quest.png",
		"../Resources/Textures/Icon/Button/quest_down.png");
	_questBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(151, 29)
		->SetPosition(_exitBtn->GetPosition().x - 200, _exitBtn->GetPosition().y)
		->SetDeligate(_UIDialogDeligate)
		->SetTagNum(UITag::UI_DIALOG_QUEST);

	nextBtn = new UIButton;
	nextBtn->Setup();
	nextBtn->SetButtonTexture(
		"../Resources/Textures/Icon/Button/questName.png",
		"../Resources/Textures/Icon/Button/questName.png",
		"../Resources/Textures/Icon/Button/questName_down.png");
	nextBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(250, 29)
		->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y)
		->SetDeligate(_UIDialogDeligate)
		->SetTagNum(UITag::UI_DIALOG_QUEST_NEXT);
	nextBtn->SetHiddenState(true);
	nextBtn->SetText("계속..");
	nextBtn->SetFontFormat(DT_CENTER | DT_WORDBREAK);

	_acceptBtn = new UIButton;
	_acceptBtn->Setup();
	_acceptBtn->SetButtonTexture(
		"../Resources/Textures/Icon/Button/questName.png",
		"../Resources/Textures/Icon/Button/questName.png",
		"../Resources/Textures/Icon/Button/questName_down.png");
	_acceptBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(250, 29)
		->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y)
		->SetDeligate(_UIDialogDeligate)
		->SetTagNum(UITag::UI_DIALOG_QUEST_ACCEPT);
	_acceptBtn->SetHiddenState(true);
	_acceptBtn->SetText("수락");
	_acceptBtn->SetFontFormat(DT_CENTER | DT_WORDBREAK);

	_noBtn = new UIButton;
	_noBtn->Setup();
	_noBtn->SetButtonTexture(
		"../Resources/Textures/Icon/Button/questName.png",
		"../Resources/Textures/Icon/Button/questName.png",
		"../Resources/Textures/Icon/Button/questName_down.png");
	_noBtn->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(250, 29)
		->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y)
		->SetDeligate(_UIDialogDeligate)
		->SetTagNum(UITag::UI_DIALOG_QUEST_NO);
	_noBtn->SetHiddenState(true);
	_noBtn->SetText("거절");
	_noBtn->SetFontFormat(DT_CENTER | DT_WORDBREAK);

	
	_name = new UITextBox;
	_name->Setup();
	_name->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(WINSIZE_X, 20)
		->SetPosition(0, 10)
		->SetDeligate(NULL);
	_name->SetFontFormat(DT_CENTER | DT_WORDBREAK);

	_answer = new UITextBox;
	_answer->Setup();
	_answer->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(WINSIZE_X * 0.7, 20)
		->SetPosition(WINSIZE_X * 0.15, 100)
		->SetDeligate(_UIDialogDeligate);

	_answer->SetFontFormat(DT_CENTER | DT_WORDBREAK);

	_isOpenShop = false;
}

void UIDialog::Update()
{
	UIWindow::Update();
	if (_npc)
	{
		D3DXVECTOR3 lookPos = _npc->pTransform->GetWorldPosition();
		D3DXVECTOR3 pos = (-_npc->pTransform->GetForward() * 2);
		pos.y += 0.5;
		lookPos.y += 1.5;
		pos.x += 0.5f;
		NPC_CAMERA->SetWorldPosition(lookPos + pos);
		NPC_CAMERA->LookPosition(lookPos);
		//NPC_CAMERA->IsHiddenCursor = false;


		for (auto i = _vecQuestBtn.begin(); i != _vecQuestBtn.end(); ++i){
			if ((*i)->GetTagNum() == _uiUpdateIndex && KEY_MGR->IsStayDown(VK_LBUTTON)){
				
				(*i)->SetClickTexture();
			}
			else{
				(*i)->SetNormalTexture();
			}
		}

		
		//if (isTextHidden){
		if (isSelect){
			for (auto i = _vecQuestBtn.begin(); i != _vecQuestBtn.end(); ++i){
				(*i)->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y);
			}
			isSelect = false;
		}

		if (KEY_MGR->IsOnceDown('N')){
			NextQuestText();
		}

		if (_shop != NULL)_shop->Update();
		if (_inven != NULL)_inven->Update();
	}
	else
	{
	//	GetRoot()->GetWindowByTag(UITag::UI_SCREEN)->SetHiddenState(false);
	//	SetHiddenState(true);
	//	((UIShop*)GetRoot())->SetHiddenState(true);
	//	CAMERA->SetPlayer();
	}
}

void UIDialog::Enable()
{

	
	SPRITE_MGR->FadeInBlack(0.2f);

	NpcInfo info = ((NPC*)_npc)->GetInfo();

	_name->SetText(info.Name);
	_answer->SetText(info.Answer);

	if (info.Button == UIDB::SHOP){
		
		_shopBtn = new UIButton;
		_shopBtn->Setup();
		_shopBtn->SetButtonTexture(
			"../Resources/Textures/Icon/Button/shop.png",
			"../Resources/Textures/Icon/Button/shop.png",
			"../Resources/Textures/Icon/Button/shop_down.png");
		_shopBtn->AttachTo(this)
			->AttackToWindow(this)
			->SetSize(151, 29)
			->SetPosition(_exitBtn->GetPosition().x - 100, _exitBtn->GetPosition().y)
			->SetDeligate(_UIDialogDeligate)
			->SetTagNum(UITag::UI_SHOP);
		_shopBtn->SetHiddenState(true);
		_shopBtn->SetFontFormat(DT_CENTER | DT_WORDBREAK);
		_shopBtn->SetDrawBoundingState(false);


		_exitBtn->SetPosition(_exitBtn->GetPosition().x + 100, _exitBtn->GetPosition().y);
		_questBtn->SetPosition(_exitBtn->GetPosition().x - 400, _exitBtn->GetPosition().y);
		_beginBtn->SetPosition(_exitBtn->GetPosition().x - 600, _exitBtn->GetPosition().y);
		
	}
	GetRoot()->SetHiddenState(true);
	SetHiddenState(false);
	CAMERA_MGR->SetNPC();
}


// 퀘스트 셋팅
void UIDialog::SetQuest(){
	
	NpcInfo info = ((NPC*)_npc)->GetInfo();
	int index = 0;
	//__questBtn.resize(info.questList.size());
	nextBtn->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y);
	for (auto i = info.questList.begin(); i != info.questList.end(); ++i){
		
		if (i->second.npcQuestStatus == UIQUEST::QUEST_STATUS::QUEST_IN_PROGRESS 
			|| i->second.npcQuestStatus == UIQUEST::QUEST_STATUS::QUEST_END)continue;

		UIButton* questNameBtn = new UIButton;
		questNameBtn->Setup();
		questNameBtn->SetButtonTexture(
			"../Resources/Textures/Icon/Button/questName.png",
			"../Resources/Textures/Icon/Button/questName.png",
			"../Resources/Textures/Icon/Button/questName_down.png");
		questNameBtn->AttachTo(this)
			->AttackToWindow(this)
			->SetSize(250, 29)
			->SetPosition(WINSIZE_X / 2 - 120, (index * 50) + 150)
			->SetDeligate(_UIDialogDeligate)
			->SetTagNum(i->first);
		questNameBtn->SetDrawBoundingState(false);
		questNameBtn->SetText(i->second.questName);
		questNameBtn->SetFontFormat(DT_CENTER | DT_WORDBREAK);
		_vecQuestBtn.push_back(questNameBtn);

		index++;
	}
	int sdfsd = 0;
}



void UIDialog::SelectQuest(UINT idx){
	
	NpcInfo info = ((NPC*)_npc)->GetInfo();
	_questIndex = idx;
	questInfo = info.questList.find(idx)->second;

	_answer->SetText(*(questInfo.questText.begin()));
	_questTextIndex = 0;
	nextBtn->SetPosition(WINSIZE_X / 2 - 120, 150);
}


void UIDialog::Disible()
{
	SPRITE_MGR->FadeInBlack(0.2f);
	_vecQuestBtn.clear();
	this->SetBackground("../Resources/Textures/ui/ui_background.png");
	isInitDialog = true;

	GetRoot()->SetHiddenState(true);
	GetRoot()->GetWindowByTag(UITag::UI_SCREEN)->SetHiddenState(false);
	SetHiddenState(true);
	CAMERA_MGR->SetPlayer();
}


// 다음 퀘스트 내용 출력
void UIDialog::NextQuestText(){
	_questTextIndex++;
	if ((questInfo.questText.begin() + _questTextIndex) != questInfo.questText.end()){
		_answer->SetText(*(questInfo.questText.begin() + _questTextIndex));
		SOUND_MGR->isPlay("questNext", 0.4f);
	}
	else{
		_questTextIndex = 0;
		nextBtn->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y);
		_acceptBtn->SetPosition(WINSIZE_X / 2 - 120, 150);
		_noBtn->SetPosition(WINSIZE_X / 2 - 120, 200);
	}
}


void UIDialog::NoQuest(){
	NpcInfo info = ((NPC*)_npc)->GetInfo();
	_answer->SetText(info.Answer);
	_acceptBtn->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y);
	_noBtn->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y);
}


void UIDialog::AcceptQuest(){
	NpcInfo info = ((NPC*)_npc)->GetInfo();
	_answer->SetText(info.Answer);

	_acceptBtn->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y);
	_noBtn->SetPosition(MOVE_NULL_WINDOW_X, MOVE_NULL_WINDOW_Y);
	((NPC*)_npc)->SetQuestStatus(_questIndex, UIQUEST::QUEST_STATUS::QUEST_IN_PROGRESS);
	
	UIAlram* alram = UIAlram::GetInstance();
	alram->Setup();
	alram->AttachTo(this)
		->AttackToWindow(this);
	alram->SetHiddenState(false);
	alram->SetAlphaBlend(255);
	alram->SetPosition(WINSIZE_X / 2 - 220, -400);
	alram->SetText("\n퀘스트를 수락하였습니다.");
	PLAYER_QUEST userQuest;
	userQuest.npcQuestStatus = UIQUEST::QUEST_STATUS::QUEST_IN_PROGRESS;
	userQuest.questName = questInfo.questName;
	std::string questAllText = "";
	SOUND_MGR->play("quest", 0.3f);
	for (auto i = questInfo.questText.begin(); i != questInfo.questText.end(); i++){
		questAllText.append((*i));
		questAllText.append("\n");
	}


	userQuest.questText = questAllText;
	userQuest.questNo = _questIndex;
	userQuest.questQty = questInfo.questQTY;

	NPC** tempNN = new NPC*;
	*tempNN = ((NPC*)_npc);
	userQuest.questNpc = tempNN;

	//(*userQuest.questNpc)->GetInfo();
//	(*userQuest.questNpc)->SetQuestStatus(_questIndex,UIQUEST::QUEST_STATUS::QUEST_BEGIN);
	userQuest.nInfo = &((NPC*)_npc)->GetInfo();
	userQuest.questNpcName = info.Name;
	userQuest.questTargetName = questInfo.questTargetName;
	//userQuest.questTargetName = 

	QuestManager::GetInstance()->AddPlayerQuest(_questIndex, userQuest);

}


void UIDialog::OpenShop(){
	if (_shop != NULL){
		//_shop->Toggle();
		//this->Render();
		_isOpenShop = true;
	//	_shop->Toggle();
		//_shop->SetHiddenState(false);
		//_shop->Toggle();
		//((UIShop*)GetRoot())->SetHiddenState(false);
//		((UIShop*)GetRoot())->Toggle();
		//_shop->SetHiddenState(false);
		//GetRoot()->GetWindowByTag(UITag::UI_SHOP_OPEN)->SetHiddenState(false);
		//GetRoot()->GetWindowByTag(UITag::UI_SHOP_OPEN)->Toggle();
		_shop->Toggle();   
		_inven->Toggle();
	}
}




void UIDialog::SetShop(UIShop* shop){
	_shop = shop;
	//_shop = new UIShop;
	//_shop->Setup();
}


void UIDialog::SetInventory(UIInventory* inven){
	_inven = inven;
}


void UIDialog::Render(LPD3DXSPRITE sprite){
	UIWindow::Render(sprite);
//	if (_isOpenShop && _shop != NULL)
	//	_shop->Render(sprite);
}