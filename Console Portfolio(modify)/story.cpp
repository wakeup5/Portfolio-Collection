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
	printf("%s", "��");
	_getch();
	printf("\b\b  \b\b");
}

void Story::opening()
{
	GameSound::playBGM(SOUND_BGM_OPENING1);
	GameDisplay::image(GameImage::get(CG_OPENING));

	GameDisplay::dialogClear();
	dialogLine("Watcher ~the console\n", 100);
	dialogLine("�� ������ 125*65 �ػ󵵿� ������ �۲� ũ�� 6*13�� ����ȭ �Ǿ� �ֽ��ϴ�.\n �ѱ� �� �������� ��������", 0);
	pause();
	//GameSound::stop();
}

void Story::intro()
{
	GameSound::playBGM(SOUND_BGM_QUESTION);
	system("cls");
	name(" ");
	dialogLinePause("������������!");
	dialogLinePause("������������!");

	GameDisplay::image(GameImage::get(CG_K17));
	name(" ");
	dialogLinePause("�˶��Ҹ��� ���� ������.");
	dialogLinePause("���̰� �� ������ �ڰ� �־���.");
	dialogLinePause("���� ���̸� ���� ������.");

	name(" [ k17 ]");
	dialogLinePause("�Ͼ, ����.");

	name(" [ �� ]");
	dialogLinePause("���..");

	name(" ");
	dialogLinePause("��� ��ô�̴��� �ܿ� ���� ����.");

	GameDisplay::image(GameImage::get(CG_YEUN_NORMAL));
	name(" [ �� ]");
	dialogLinePause("��.. �� ���° K��?");

	name(" ");
	dialogLinePause("�׷��� ����� ������ �ѷ������� ������� ������.");

	name(" [ �� ]");
	dialogLinePause("��, ���� ����?");
	dialogLinePause("���� �б��� �ƴϳ�..?");

	name(" [ k17 ]");
	dialogLinePause("�¾�. ���� �б��� �ƴϾ�.");
	dialogLinePause("�� 17��° K��, �츰 �б��� Ż���߾�.");
	dialogLinePause("���� �Ƚ��ص� ��. ����.");

	name(" [ �� ]");
	dialogLinePause("��..");

	name(" [ k17 ]");
	dialogLinePause("� �Ͼ.");
	dialogLinePause("�츰 ���� �������� ���ž�.");

	name(" [ �� ]");
	dialogLinePause("����?");

	name(" [ k17 ]");
	dialogLinePause("������ �ʸ� �����־��� ���� �־�.");
	dialogLinePause("�������ʹ� �ű⿡�� �����ž�.");
	dialogLinePause("�Ƹ� �װ��� ���� �� ���� ���� �������ž�.");

	name(" ");
	dialogLinePause("�׷�. ������ �ִ� �����̶�� �����̳��� ������ ���� ����������.");
	dialogLinePause("������ ���� ������ ���� ���� �ڸ� �� �� �Ϸ��� ����� �Ҵ� ���̾���.");

	name(" ");
	dialogLinePause("���̴� �Ϸ���̿���.");
	dialogLinePause("...");

	system("cls");
}

void Story::hospital()
{
	system("cls");
	GameDisplay::image(GameImage::get(CG_WONJANG_NORMAL));
	name(" [ ���� ]");
	dialogLinePause("���. ����.");
	dialogLinePause("�������̽ó׿�.");

	name(" [ K17 ]");
	dialogLinePause("�������� ���⼭ �����ǵ� �����پ�?");

	name(" [ ���� ]");
	dialogLinePause("��������.");
	dialogLinePause("������ �׷��� ���� �� ��ڴ�ϴ�.");
	dialogLinePause("���̴� ���� �����帮���� ����.");

	name(" [ ���� ]");
	dialogLinePause("�׷��� ����.");
	dialogLinePause("�������� ��� �Ǽ˳���?");

	name(" [ K17 ]");
	dialogLinePause("�������� ������ �����Ұž�.");

	name(" [ ���� ]");
	dialogLinePause("����.");
	dialogLinePause("�� ������ �η����� �׷��� ���Բ��� ��ġ�°� ���� ���� �ʽ��ϴ�.");
	dialogLinePause("�ε� �����Ͻñ� �ٶ��Կ�.");
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
	dialogLinePause("�̰� ������ ����?");

	name("[ ���� ������ ]");
	dialogLinePause("��ǰ �ݰ�.");

	name("[ k17 ]");
	dialogLinePause("��?");

	name("[ ���� ������ ]");
	dialogLinePause("��ǰ �ݰ� �̶��.");

	name("[ k17 ]");
	dialogLine("�ʹ� �Ѱ� �ƴѰ���?");
	dialogLinePause("�̷��Ƶ� ���� ��ǰ�̶󱸿�.");

	name("[ ���� ������ ]");
	dialogLine("�װ� �󸶳� ������ ��� �˾�?");
	dialogLinePause("�ݰ� �ƴϸ� �Ȼ�Ŵ� �˾Ƽ� �Ͻö��.");

	name("[ k17 ]");
	dialogLinePause("(�������)");

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
	dialogLinePause("�����ΰ�..");

	name(" ");
	dialogLinePause("��ӵ��� �α� I.C�ⱸ�� ���� �����.");
	dialogLinePause("��� �ٶ��� ���� ���� �� �տ� �ݹ߸Ӹ��� �ҳడ ��Ÿ����.");

	GameDisplay::image(GameImage::get(CG_LINA_NORMAL));
	name(" [ ���� ]");
	dialogLinePause("������. K17 ����.");

	name(" [ k17 ]");
	dialogLinePause("������ ��� ����?");

	name(" [ ���� ]");
	dialogLinePause("�������� �����µ� �� ���� �ܿ� �װž�?");

	name(" ");
	dialogLinePause("�ƹ��� ���� �׳ฦ �ܴ�����.");

	name(" [ ���� ]");
	dialogLinePause("...");
	dialogLinePause("�˾Ҿ�. �������ٰ�.");
	dialogLinePause("������ ������ �ƴϾ�.");
	dialogLinePause("���� ������ ��Ʈ�� �ξ���. ���� �̰����� ����.");

	name(" [ ���� ]");
	dialogLinePause("������ ������. ������ �뺴���� �� ��Ű�� ������.");

	name(" ");
	dialogLinePause("������ �׷� ���� ������ ������ �������.");
}


