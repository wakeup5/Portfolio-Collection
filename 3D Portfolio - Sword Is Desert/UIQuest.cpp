#include "stdafx.h"
#include "UIQuest.h"
#include "UIButton.h"
#include "UIAlram.h"


UIWindow* content;
std::vector<UITextBox*> _vecQuestBox;
std::vector<UIButton*> _vecUiBtn;
UITextBox* questInfoBox;
UIQuestSocketDeligate* sd;
UIQuest::UIQuest()
{
}


UIQuest::~UIQuest()
{
}

void UIQuest::Setup()
{
	UIWindow::Setup();

	_playerInfo = PlayerManager::GetInstance()->GetPlayer();

	this->SetPosition(WINSIZE_X - 428, 100);
	this->SetSize(400, 600);

	defaultDeligate = new UIDeligateDefault;

	this->SetBackground("../Resources/Textures/ui/ui_background.png");
	this->SetDeligate(defaultDeligate);
	this->SetHiddenState(true);
	this->SetTagNum(UITag::UI_INVENTORY);

	UITextBox* name = new UITextBox;
	name->Setup();
	name->AttachTo(this)
		->AttackToWindow(this)
		->SetSize(400, 20)
		->SetPosition(0, 30);

	name->SetText("퀘  스  트");
	name->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);

	content = new UIWindow;
	content->Setup();
	content
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/ui_background_small.png")
		->SetSize(350, 500)
		->SetPosition(25, 75);

	UIWindow* oneLine = new UIWindow;
	oneLine->Setup();
	oneLine
		->AttachTo(this)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/ui/drawLine.png")
		->SetSize(350, 2)
		->SetPosition(25, 400);

	questInfoBox = new UITextBox;
	questInfoBox->Setup();
	questInfoBox
		->AttachTo(content)
		->AttackToWindow(this)
		->SetSize(350, 100)
		->SetPosition(0, 350);
	questInfoBox->SetFontFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);
	questInfoBox->SetColor(D3DCOLOR_XRGB(185, 181, 181));
	questInfoBox->SetAlphaBlend(200);

	sd = new UIQuestSocketDeligate;
//	sd->init();

	UIButton* giveUpBtn = new UIButton;
	giveUpBtn->Setup();
	giveUpBtn->SetButtonTexture(
		"../Resources/Textures/icon/button/포기.png",
		"../Resources/Textures/icon/button/포기.png",
		"../Resources/Textures/icon/button/포기_클릭.png");
	giveUpBtn
		->AttachTo(content)
		->AttackToWindow(this)
		->SetBackground("../Resources/Textures/icon/button/포기.png")
		->SetSize(93, 26)
		->SetTagNum(UITag::UI_DIALOG_QUEST_GIVE_UP)
		->SetDeligate(sd)
		->SetPosition(120, 450);
	giveUpBtn->SetAlphaBlend(160);

	/*
	UIAlram* alram = UIAlram::GetInstance();
	alram->Setup();
	alram->AttachTo(this)
		->AttackToWindow(this);
	alram->SetAlphaBlend(255);
	//alram->SetText(QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questName + " 퀘스트를 완료하였습니다.");
	alram->SetPosition(-500, 10);*/
	UIAlram* alram = UIAlram::GetInstance();
	alram->Setup();
	alram->AttachTo(this)
		->AttackToWindow(this);
	alram->SetPosition(-480, 15);
	alram->SetHiddenState(false);
}

void UIQuest::Release()
{
	UIWindow::Release();
}

void UIQuest::Update()
{
	UIWindow::Update();
	
	
	int userQuestLength = QuestManager::GetInstance()->GetLength();
	if (KEY_MGR->IsOnceDown('L'))
	{
		questInfoBox->SetText("");
		Toggle();
		if (userQuestLength > 0){
			LoadQuest();
		}

		if (!_isOpenQuest){
			_isOpenQuest = true;
		}
		else{
			_isOpenQuest = false;
			isInitDialog = true;
		}
	}

	/*
	if (KEY_MGR->IsOnceDown(VK_F8))
	{
		LOG_MGR->AddLog("퀘스트 수행중");
		//QuestManager::GetInstance()->GetPlayerQuestList().begin()->second.questInProgressQty += 1;
		QuestManager::GetInstance()->QuestProgressPlus(_clickQuestIndex);
		LoadQuestInfo();
	}

	if (KEY_MGR->IsOnceDown(VK_F7))
	{
		LOG_MGR->AddLog("퀘스트 수행중");
		//QuestManager::GetInstance()->GetPlayerQuestList().begin()->second.questInProgressQty += 1;
		//_clickQuestIndex = QuestManager::GetInstance()->QuestProgressPlus("오크");
		QuestManager::GetInstance()->SetTargetName("Muspel");
		if (QuestManager::GetInstance()->GetProgressQuestNo() != -1){
			_clickQuestIndex = QuestManager::GetInstance()->GetProgressQuestNo();
			LoadQuestInfo();
		}
	
	}
	if (KEY_MGR->IsOnceDown(VK_F6))
	{
		LOG_MGR->AddLog("퀘스트 안받음");
		//QuestManager::GetInstance()->GetPlayerQuestList().begin()->second.questInProgressQty += 1;
		//_clickQuestIndex = QuestManager::GetInstance()->QuestProgressPlus("오크");
		QuestManager::GetInstance()->SetTargetName("해골");
		if (QuestManager::GetInstance()->GetProgressQuestNo() != -1){
			_clickQuestIndex = QuestManager::GetInstance()->GetProgressQuestNo();
			LoadQuestInfo();
		}
	
	}
	*/

	// 퀘스트몹 존재시 업데이트
	if (QuestManager::GetInstance()->GetProgressQuestNo() != -1){
		_clickQuestIndex = QuestManager::GetInstance()->GetProgressQuestNo();
		LoadQuestInfo();
	}

	/*
	if (_clickQuestIndex != -1 && KEY_MGR->IsOnceDown('Z') ){
		QuestManager::GetInstance()->DeletePlayerQuest(_clickQuestIndex);
		LoadQuest();
		_clickQuestIndex = -1;
		
	}

	if(_clickQuestIndex != -1 && KEY_MGR->IsOnceDown('X')){
		//::GetInstance()->DeletePlayerQuest(_clickQuestIndex);
		//LoadQuest();
		
		if (QuestManager::GetInstance()->GetLength() > 0){
			std::string df = QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questText;
			int sdfsd = 0;
			questInfoBox->SetText(
				QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questText
				);
		}
		_clickQuestIndex = -1;
	}
	*/
	//UIAlram::GetInstance()->Update(0.08f);
}

void UIQuest::Render(LPD3DXSPRITE sprite)
{
	UIWindow::Render(sprite);
	
}


void UIQuest::DeleteQuest(){
	if (QuestManager::GetInstance()->GetLength() <= 0)return;
	QuestManager::GetInstance()->GiveUpPlayerQuest(_clickQuestIndex);
	_clickQuestIndex = -1;
	questInfoBox->SetText("");
	LoadQuest();
}

void GiveUpQuest(){
	//QuestManager::GetInstance()->GiveUpPlayerQuest
}


void UIQuest::LoadQuest(){
	
	UISocket* socket;
	UITextBox* tb;
	UIButton* uiQuestBackground;
	int numIndex = 0;
	QuestManager* questMg = QuestManager::GetInstance();

	if (_vecQuestBox.size() > 0){
		for (auto i = _vecQuestBox.begin(); i != _vecQuestBox.end(); ++i){
			(*i)->SetPosition(MOVE_NULL_PT_X, MOVE_NULL_PT_Y);
		}
	}
	if (_vecUiBtn.size() > 0){
		for (auto i = _vecUiBtn.begin(); i != _vecUiBtn.end(); ++i){
			(*i)->SetPosition(MOVE_NULL_PT_X, MOVE_NULL_PT_Y);
		}
	}
	
	
	std::map<UINT, PLAYER_QUEST> loadQuestList = questMg->GetPlayerQuestList();

	int dsf = 0;
	for (auto i = loadQuestList.begin(); i != loadQuestList.end(); ++i){
		//if (!i->second.npcQuestStatus == UIQUEST::QUEST_IN_PROGRESS)continue;

		if (i->second.npcQuestStatus == UIQUEST::QUEST_STATUS::QUEST_IN_PROGRESS){
			LOG_MGR->AddLog("퀘스트중");
		}
		else if (i->second.npcQuestStatus == UIQUEST::QUEST_STATUS::QUEST_BEGIN){
			LOG_MGR->AddLog("퀘스트 begin");
		}
		else{
			LOG_MGR->AddLog("퀘스트 End");
		}

		uiQuestBackground = new UIButton;
		uiQuestBackground->Setup();
		uiQuestBackground
			->AttachTo(this)
			->AttackToWindow(this)
			//->SetBackground("../Resources/Textures/ui/button.png")
			->SetSize(330, 40)
			->SetDeligate(sd)
			->SetTagNum((*i).second.questNo)
			->SetPosition(35, (45 * (numIndex +1)) + 40 );
		uiQuestBackground->SetDrawBoundingState(false);
		uiQuestBackground->SetAlphaBlend(50);
		uiQuestBackground->SetButtonTexture(
			"../Resources/Textures/ui/button.png",
			"../Resources/Textures/ui/button.png",
			"../Resources/Textures/ui/button_down.png"
			);
		_vecUiBtn.push_back(uiQuestBackground);
	
		tb = new UITextBox;
		tb->Setup();
		tb
			->AttachTo(content)
			->AttackToWindow(this)
			->SetSize(200, 38)
			->SetPosition(30, 20 + (numIndex) * 45)
			->SetDeligate(sd)
			;
		tb->SetDrawBoundingState(false);
		tb->SetText((*i).second.questName);
		tb->SetColor(D3DCOLOR_XRGB(81, 93, 20));
		tb->SetFontFormat(DT_TOP | DT_WORDBREAK);
		numIndex++;
		_vecQuestBox.push_back(tb);

	}
	
}


// 퀘스트 상세정보 로드
void UIQuest::LoadQuestInfo(){

	if (_clickQuestIndex < 1150)return;

	if (QuestManager::GetInstance()->GetLength() > 0){
		UINT tf = _clickQuestIndex;	
		if (QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).npcQuestStatus == UIQUEST::QUEST_STATUS::QUEST_END){
		//if (QuestManager::GetInstance()->GetNpcQuestStatus(_clickQuestIndex) == UIQUEST::QUEST_STATUS::QUEST_END){
			UIAlram* alram = UIAlram::GetInstance();
			//alram->Setup();
			
			alram->SetHiddenState(false);
			alram->SetAlphaBlend(255);
			alram->SetText("\n" + QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questName +  " 퀘스트를 완료하였습니다.");
			SOUND_MGR->play("questComplete",0.3f);
			QuestComplete();
			return;
		}

		std::string strQuestInfo = QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questText;
		strQuestInfo.append("\n");
		strQuestInfo.append(std::to_string(QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questInProgressQty));
		strQuestInfo.append(" / ");
		strQuestInfo.append(std::to_string(QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questQty));

		//strQuestInfo = QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questText;
		questInfoBox->SetText(
			//QuestManager::GetInstance()->GetPlayerQuest(_clickQuestIndex).questText
			strQuestInfo
			);
	}
	//_clickQuestIndex = -1;
}


// 클릭한곳의 퀘스트 번호 설정과 동시에 퀘스트 정보 로드
void UIQuest::SetClickQuestIndex(int clickIndex){
	_clickQuestIndex = clickIndex;
	QuestManager::GetInstance()->SetProgressQuestNo(clickIndex);
	LoadQuestInfo();
}


void UIQuest::QuestComplete(){
	if (QuestManager::GetInstance()->GetLength() <= 0)return;
	QuestManager::GetInstance()->QuestComplete(_clickQuestIndex);
	QuestManager::GetInstance()->SetProgressQuestNo(-1);
	_clickQuestIndex = -1;
	questInfoBox->SetText("");
	LoadQuest();
}