#include "Story.h"

using namespace std;

Story::Story()
{

}


Story::~Story()
{

}

//StoryItem* Story::_storyItem = new StoryItem();

void Story::name(string name)
{
	GameDisplay::dialogClear();
	printf(" ");
	if (!name.empty())
	{
		_name = name;
	}
	cout << /*" [ " <<*/ _name /*<< " ] " */ << endl;
}

void Story::dialogLine(string dialog, int speed, int turm)
{
	//Display::gotoY(56);
	cout << endl;
	printf(" ");
	for (int i = 0; i < dialog.length(); i++)
	{
		cout << dialog[i];
		Sleep(speed);
	}
	Sleep(turm);
}

void Story::dialogLinePause(string dialog, int speed, int turm)
{
	dialogLine(dialog, speed, turm);
	pause();
}

void Story::pause()
{
	printf("%s", "¶");
	_getch();
	printf("\b\b  \b\b");
}

void Story::opening()
{
	GameSound::playBGM(SOUND_BGM_OPENING1);
	GameDisplay::image(GameImage::get(CG_OPENING));

	GameDisplay::dialogClear();
	dialogLine("Watcher ~the console\n", 100);
	dialogLine("본 게임은 125*65 해상도에 래스터 글꼴 크기 6*13에 최적화 되어 있습니다.\n 한글 좀 깨지더라도 참으세요", 0);
	pause();
	//GameSound::stop();
}

void Story::intro()
{
	GameSound::playBGM(SOUND_BGM_QUESTION);
	system("cls");
	name(" ");
	dialogLinePause("따르르르르릉!");
	dialogLinePause("따르르르르릉!");

	GameDisplay::image(GameImage::get(CG_K17));
	name(" ");
	dialogLinePause("알람소리에 눈이 떠졌다.");
	dialogLinePause("연이가 내 옆에서 자고 있었다.");
	dialogLinePause("나는 연이를 흔들어 깨웠다.");

	name(" [ k17 ]");
	dialogLinePause("일어나, 연아.");

	name(" [ 연 ]");
	dialogLinePause("우웅..");

	name(" ");
	dialogLinePause("잠시 뒤척이더니 겨우 눈을 떳다.");

	GameDisplay::image(GameImage::get(CG_YEUN_NORMAL));
	name(" [ 연 ]");
	dialogLinePause("응.. 넌 몇번째 K야?");

	name(" ");
	dialogLinePause("그렇게 물어보곤 주위를 둘러보더니 놀란듯이 물었다.");

	name(" [ 연 ]");
	dialogLinePause("어, 여긴 어디야?");
	dialogLinePause("여긴 학교가 아니네..?");

	name(" [ k17 ]");
	dialogLinePause("맞아. 여긴 학교가 아니야.");
	dialogLinePause("난 17번째 K고, 우린 학교를 탈출했어.");
	dialogLinePause("이제 안심해도 돼. 연아.");

	name(" [ 연 ]");
	dialogLinePause("응..");

	name(" [ k17 ]");
	dialogLinePause("어서 일어나.");
	dialogLinePause("우린 이제 병원으로 갈거야.");

	name(" [ 연 ]");
	dialogLinePause("병원?");

	name(" [ k17 ]");
	dialogLinePause("예전에 너를 돌봐주었던 곳이 있어.");
	dialogLinePause("이제부터는 거기에서 지낼거야.");
	dialogLinePause("아마 그곳에 가면 네 병도 많이 나아질거야.");

	name(" ");
	dialogLinePause("그래. 원장이 있는 병원이라면 조금이나마 연이의 병이 나아지겟지.");
	dialogLinePause("연이의 병은 유진에 의해 잠을 자면 그 날 하루의 기억을 잃는 병이었다.");

	name(" ");
	dialogLinePause("연이는 하루살이였다.");
	dialogLinePause("...");

	system("cls");
}

void Story::hospital()
{
	system("cls");
	GameDisplay::image(GameImage::get(CG_WONJANG_NORMAL));
	name(" [ 원장 ]");
	dialogLinePause("어머. 고객님.");
	dialogLinePause("오랜만이시네요.");

	name(" [ K17 ]");
	dialogLinePause("이제부터 여기서 지낼건데 괜찮겟어?");

	name(" [ 원장 ]");
	dialogLinePause("물론이죠.");
	dialogLinePause("오히려 그러는 편이 더 기쁘답니다.");
	dialogLinePause("연이는 제가 돌봐드리도록 하죠.");

	name(" [ 원장 ]");
	dialogLinePause("그런데 고객님.");
	dialogLinePause("유진과는 어떻게 되셧나요?");

	name(" [ K17 ]");
	dialogLinePause("이제부터 유진을 추적할거야.");

	name(" [ 원장 ]");
	dialogLinePause("고객님.");
	dialogLinePause("전 유진이 두렵지만 그래도 고객님께서 다치는걸 보고 싶지 않습니다.");
	dialogLinePause("부디 무사하시길 바랄게요.");
}

void Story::getEvent(GAME_EVENT e)
{
	switch (e)
	{
	case GAME_EVENT_SHOP_SELL:
		shopSellEvent();
		break;
	case GAME_EVENT_DUNGEON_1_1:
		dungeon1Event();
		break;
	}
}

void Story::shopSellEvent()
{
	static bool is = false;
	if (is)
	{
		return;
	}
	is = true;

	GameDisplay::image(GameImage::get(CG_MAEJUM));
	name("[ k17 ]");
	dialogLinePause("이거 가격이 얼마죠?");

	name("[ 매점 아지매 ]");
	dialogLinePause("신품 반값.");

	name("[ k17 ]");
	dialogLinePause("네?");

	name("[ 매점 아지매 ]");
	dialogLinePause("신품 반값 이라고.");

	name("[ k17 ]");
	dialogLine("너무 싼거 아닌가요?");
	dialogLinePause("이래뵈도 제법 신품이라구요.");

	name("[ 매점 아지매 ]");
	dialogLine("그거 얼마나 썻는지 어떻게 알어?");
	dialogLinePause("반값 아니면 안살거니 알아서 하시라고.");

	name("[ k17 ]");
	dialogLinePause("(빌어먹을)");

}

void Story::dungeon1Event()
{
	static bool is = false;
	if (is)
	{
		return;
	}
	is = true;

	GameDisplay::imageClear();
	name(" [ K17 ]");
	dialogLinePause("이쯤인가..");

	name(" ");
	dialogLinePause("고속도로 인근 I.C출구에 차를 대었다.");
	dialogLinePause("잠깐 바람을 쐬고 있자 곧 앞에 금발머리의 소녀가 나타났다.");

	GameDisplay::image(GameImage::get(CG_LINA_NORMAL));
	name(" [ 리나 ]");
	dialogLinePause("오랜만. K17 오빠.");

	name(" [ k17 ]");
	dialogLinePause("유진은 어디에 있지?");

	name(" [ 리나 ]");
	dialogLinePause("오랜만에 만났는데 할 말이 겨우 그거야?");

	name(" ");
	dialogLinePause("아무말 없이 그녀를 겨누었다.");

	name(" [ 리나 ]");
	dialogLinePause("...");
	dialogLinePause("알았어. 가르쳐줄게.");
	dialogLinePause("하지만 오늘은 아니야.");
	dialogLinePause("전국 곳곳에 힌트를 두었어. 물론 이곳에도 말야.");

	name(" [ 리나 ]");
	dialogLinePause("하지만 조심해. 유능한 용병들이 잘 지키고 있으니.");

	name(" ");
	dialogLinePause("리나는 그런 말을 남기고는 조용히 사라졌다.");
}


