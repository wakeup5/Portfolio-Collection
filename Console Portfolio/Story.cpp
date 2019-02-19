#include "Story.h"

using namespace std;

Story::Story()
{

}


Story::~Story()
{

}

void Story::name(char* name = NULL)
{
	Display::dialogClear();
	printf(" ");
	if (name != NULL)
	{
		_name = name;
	}
	cout << /*" [ " <<*/ _name /*<< " ] " */ << endl;
}

void Story::dialogLine(char* dialog, int speed, int turm)
{
	//Display::gotoY(56);
	cout << endl;
	printf(" ");
	for (int i = 0; i < (int)strlen(dialog); i++)
	{
		cout << dialog[i];
		Sleep(speed);
	}
	Sleep(turm);
}

void Story::dialogLinePause(char* dialog, int speed, int turm)
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
	Sound::playBGM(SOUND_BGM_OPENING1);
	Display::image(Image::get(CG_OPENING));

	Display::dialogClear();
	dialogLine("Watcher ~the console\n", 100);
	dialogLine("�� ������ 125*65 �ػ󵵿� ������ �۲� ũ�� 6*13�� ����ȭ �Ǿ� �ֽ��ϴ�.\n �ѱ� �� �������� ��������", 0);
	pause();
}

void Story::intro()
{
	Sound::playBGM(SOUND_BGM_QUESTION);
	Display::image(Image::get(CG_YEUN_NORMAL));
	name("[ �� ]");
	dialogLine("���¾� ����.");
	dialogLine("���¾� ����.");
	dialogLinePause("���¾� ����.");
	dialogLine("���¾� ����.");
	pause();

	int b = Display::choise("* ����� ���� �ؾ� �ұ�?", new char*[]{ "\"�ʴ� ������!\"", "\"���� �����ε�?\"" }, 2);

	name("[ �� ]");
	if (b == 0)
	{
		dialogLinePause("���̶�� �� ���ݾ� ������");
	}
	else
	{
		dialogLinePause("������ ������ ������");
	}

	Display::image(Image::get(CG_WONJANG_NORMAL));
	name("[ ���� ]");
	dialogLine("���¾� ����.");
	pause();

	Display::image(Image::get(CG_LINA_NORMAL));
	name("[ ���� ]");
	dialogLine("���¾� ����.");
	pause();

	system("cls");
	Sound::stop();
}