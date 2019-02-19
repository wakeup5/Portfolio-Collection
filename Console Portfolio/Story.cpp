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
	printf("%s", "¶");
	_getch();
	printf("\b\b  \b\b");
}

void Story::opening()
{
	Sound::playBGM(SOUND_BGM_OPENING1);
	Display::image(Image::get(CG_OPENING));

	Display::dialogClear();
	dialogLine("Watcher ~the console\n", 100);
	dialogLine("본 게임은 125*65 해상도에 래스터 글꼴 크기 6*13에 최적화 되어 있습니다.\n 한글 좀 깨지더라도 참으세요", 0);
	pause();
}

void Story::intro()
{
	Sound::playBGM(SOUND_BGM_QUESTION);
	Display::image(Image::get(CG_YEUN_NORMAL));
	name("[ 연 ]");
	dialogLine("나는야 연이.");
	dialogLine("나는야 연이.");
	dialogLinePause("나는야 연이.");
	dialogLine("나는야 연이.");
	pause();

	int b = Display::choise("* 뭐라고 말을 해야 할까?", new char*[]{ "\"너는 누구냐!\"", "\"내가 연이인데?\"" }, 2);

	name("[ 연 ]");
	if (b == 0)
	{
		dialogLinePause("연이라고 말 했잖아 거지야");
	}
	else
	{
		dialogLinePause("거짓말 하지마 거지야");
	}

	Display::image(Image::get(CG_WONJANG_NORMAL));
	name("[ 원장 ]");
	dialogLine("나는야 원장.");
	pause();

	Display::image(Image::get(CG_LINA_NORMAL));
	name("[ 리나 ]");
	dialogLine("나는야 리나.");
	pause();

	system("cls");
	Sound::stop();
}