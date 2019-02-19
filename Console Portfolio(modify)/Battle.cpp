#include "Battle.h"

struct threadArg2
{
	Battle* battle;
	Item* item;
};

CRITICAL_SECTION battlePrintCriticalSection;

//int playerThread(Battle*);
unsigned int WINAPI playerThread(void* arg);
unsigned int WINAPI playerMoveThread(void* arg);
unsigned int WINAPI playerAttackThread(void* arg);

unsigned int WINAPI enemyThread(void* arg);
unsigned int WINAPI enemyMoveThread(void* arg);
unsigned int WINAPI enemyAttackThread(void* arg);

void skillUseThread(Battle*, Skill*);
//void skillCoolTimeThread(Skill*);
void itemUseThread(Battle*, Item*);
//void itemCoolTimeThread(int);

Battle::Battle(Player* player, Enemy* enemy)
{
	_player = player;
	_enemy = enemy;
}

Battle::~Battle()
{
}
unsigned int _PT, _PMT, _PAT, _ET, _EMT, _EAT;
bool Battle::startBattle()
{
	_PT = _PMT = _PAT = _ET = _EMT = _EAT = 0;
	InitializeCriticalSection(&battlePrintCriticalSection);

	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0, playerThread, this, 0, &_PT);
	HANDLE hThread2 = (HANDLE)_beginthreadex(NULL, 0, enemyThread, this, 0, &_ET);

	while (true)
	{
		if (_player->getCurrentHp() < 1)
		{
			printf("전투에서 패배하였습니다.\n");
			return false;
		}
		else if (_enemy->getCurrentHp() < 1)
		{
			//_endthreadex(_PT);// , _endthreadex(_PMT), _endthreadex(_PAT),
			//_endthreadex(_ET);// , _endthreadex(_EMT), _endthreadex(_EAT);
			
			Item* item = new Item(_enemy->dropItem());
			_player->addItem(item);

			Sleep(2000);

			GameDisplay::dialogClear();
			printf("전투에서 이겼습니다.\n");
			printf("%s 아이템 획득!\n", item->getName());
			printf("%d만큼 경험치 획득!\n", _enemy->getLevel() * 100);
			_player->addExp(_enemy->getLevel() * 100);

			Sleep(3000);

			return true;
		}
	}

	DeleteCriticalSection(&battlePrintCriticalSection);

	//CloseHandle(hThread1);
	//CloseHandle(hThread2);
}
unsigned int WINAPI playerThread(void *arg)
{
	Battle* battle = (Battle*)arg;
	int input;
	int inputArrow;
	battle->print();
	while (true)
	{
		
		input = _getch();

		if (input == 224)
		{
			if (battle->_isPlayerMove) continue;
			inputArrow = _getch();
			

			//printf("***%d***", battle->_isPlayerMove);

			if (inputArrow == 75)//좌
			{
				if (battle->_playerPosition <= 0) continue; // battle->_playerPosition = 2;
				battle->_playerPosition--;
			}
			else if (inputArrow == 77)//우
			{
				if (battle->_playerPosition >= 2) continue; // battle->_playerPosition = 0;
				battle->_playerPosition++;
			}
			battle->print();

			_beginthreadex(NULL, 0, playerMoveThread, battle, 0, &_PMT);

		}
		else if (input == 32)//스페이스 바 - 공격
		{
			if (battle->_isPlayerAttack || battle->_isPlayerAttackPrev) continue;
			_beginthreadex(NULL, 0, playerAttackThread, battle, 0, &_PAT);
		}
		else if (false)//숫자 1, 2, 3, 4 - 물약
		{

		}
		else if (false)//qwerty.. 스킬
		{
			if (battle->_isPlayerAttack || battle->_isPlayerAttackPrev) continue;

		}

		if (battle->_player->getCurrentHp() < 1 || battle->_enemy->getCurrentHp() < 1)
		{
			break;
		}
	}

	return 0;
}
unsigned int WINAPI playerMoveThread(void *arg)
{
	//printf("%d", sizeof(arg));
	
	Battle* battle = (Battle*)arg;

	battle->_isPlayerMove = true;
	battle->print();
	Sleep(battle->_player->getTotalMoveSpeed());
	battle->_isPlayerMove = false;
	battle->print();

	return 0;
}
unsigned int WINAPI playerAttackThread(void* arg)
{
	Battle* battle = (Battle*)arg;

	battle->_isPlayerAttackPrev = true;
	battle->print();
	Sleep(battle->_player->getTotalAtkSpeed() / 2);

	battle->_isPlayerAttackPrev = false;
	battle->_isPlayerAttack = true;

	if (battle->_playerPosition == battle->_enemyPosition)
	{
		battle->_enemy->setCurrentHp(battle->_enemy->getCurrentHp() - battle->_player->getTotalAtk());
	}

	battle->print();
	Sleep(battle->_player->getTotalAtkSpeed() / 2);

	battle->_isPlayerAttack = false;
	battle->print();

	return 0;
}


unsigned int WINAPI enemyThread(void *arg)
{
	Battle* battle = (Battle*)arg;
	int input;
	battle->print();

	int inputKeys[100] = {
		75, 75, 75, 75, 75, 75, 75, 75, 75, 75,
		75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 
		75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 
		75, 75, 75, 75, 75, 75, 75, 75, 75, 75,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 
		77, 77, 77, 77, 77, 75, 75, 75, 75, 75, 
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };

	while (true)
	{
		if (battle->_isEnemyMove || battle->_isEnemyAttack || battle->_isEnemyAttackPrev) continue;
		input = inputKeys[rand() % 100];
		Sleep(battle->_enemy->getTotalMoveSpeed());

		if (input == 75)//좌
		{
			//if (battle->_isEnemyMove) continue;
			if (battle->_enemyPosition <= 0) continue; // battle->_playerPosition = 2;
			battle->_enemyPosition--;
			battle->print();
			_beginthreadex(NULL, 0, enemyMoveThread, battle, 0, &_EMT);

		}
		else if (input == 77)//우
		{
			//if (battle->_isEnemyMove) continue;
			if (battle->_enemyPosition >= 2) continue; // battle->_playerPosition = 0;
			battle->_enemyPosition++;
			battle->print();
			_beginthreadex(NULL, 0, enemyMoveThread, battle, 0, &_EMT);
		}
		else if (input == 32)//스페이스 바 - 공격
		{
			_beginthreadex(NULL, 0, enemyAttackThread, battle, 0, &_EAT);
		}
		else if (false)//qwerty.. 스킬
		{

		}

		if (battle->_player->getCurrentHp() < 1 || battle->_enemy->getCurrentHp() < 1)
		{
			break;
		}
	}

	return 0;
}
unsigned int WINAPI enemyMoveThread(void *arg)
{
	//printf("%d", sizeof(arg));

	Battle* battle = (Battle*)arg;

	battle->_isEnemyMove = true;
	battle->print();
	Sleep(battle->_enemy->getTotalMoveSpeed());
	battle->_isEnemyMove = false;
	battle->print();

	return 0;
}
unsigned int WINAPI enemyAttackThread(void* arg)
{
	Battle* battle = (Battle*)arg;

	battle->_isEnemyAttackPrev = true;
	battle->print();
	Sleep(battle->_enemy->getTotalAtkSpeed() / 2);

	battle->_isEnemyAttackPrev = false;
	battle->_isEnemyAttack = true;

	if (battle->_enemyPosition == battle->_playerPosition)
	{
		battle->_player->setCurrentHp(battle->_player->getCurrentHp() - battle->_enemy->getTotalAtk());
	}

	battle->print();
	Sleep(battle->_enemy->getTotalAtkSpeed() / 2);

	battle->_isEnemyAttack = false;
	battle->print();

	return 0;
}

void skillUseThread(Battle* battle, Skill* skill)
{
	skill->setIsCoolTime(true);
	battle->print();
	Sleep(skill->getCooltime());
	skill->setIsCoolTime(false);
	battle->print();

	return;
}

void itemUseThread(Battle* battle, Item* item)
{
	if (item->getType() == ITEM_TYPE_POTION_HP)
	{
		battle->_player->setCurrentHp(battle->_player->getCurrentHp() + item->getMain());
	}
	else if (item->getType() == ITEM_TYPE_POTION_MP)
	{
		battle->_player->setCurrentMp(battle->_player->getCurrentMp() + item->getMain());
	}
	else
	{
		return;
	}

	battle->_player->removePotion(item);

	battle->_isUseHpPotion = true;
	battle->print();
	Sleep(3000);
	battle->_isUseHpPotion = false;
	battle->print();

	return;
}

void Battle::print()
{
	EnterCriticalSection(&battlePrintCriticalSection);

	system("cls");

	//적 출력
	char enemyImage[] = 
		"      /******\\      \n"
		"     ***    ***     \n"
		"     \\*      */     \n"
		"      \\******/      \n"
		"     _--|  |--_     \n"
		"    /          \\    \n"
		"   /  /!    !\\  \\   \n"
		"   | | |    | | |   \n"
		"    ★ )====( ★    \n"
		"       /    \\       \n"
		"      /  __  \\      \n"
		"     /  /  \\  \\     \n"
		"    !  /    \\  !    \n"
		"  __! !      ! !__  \n"
		"^_____!      !_____^\n";

	if (_enemyPosition == 0)
	{
		GameDisplay::draw(enemyImage, 13, 8);
	}
	else if (_enemyPosition == 1)
	{
		GameDisplay::draw(enemyImage, 52, 8);
	}
	else
	{
		GameDisplay::draw(enemyImage, 92, 8);
	}

	//적 정보
	GameDisplay::gotoY(1);

	int ehpBar = (static_cast<float>(_enemy->getCurrentHp()) / static_cast<float>(_enemy->getMaxHp())) * 10;
	int empBar = (static_cast<float>(_enemy->getCurrentMp()) / static_cast<float>(_enemy->getMaxMp())) * 10;

	printf("\t\t HP %5d /%5d ", _enemy->getCurrentHp(), _enemy->getMaxHp());
	GameDisplay::setColor(DISPLAY_COLOR_DARKRED, DISPLAY_COLOR_GRAY);

	for (int i = 0; i < 10; i++)
	{
		if (i < ehpBar) printf("%s", "■");
		else printf("%s", "□");
	}
	GameDisplay::setColor();

	printf("\t\t\t MP %5d /%5d ", _enemy->getCurrentMp(), _enemy->getMaxMp());
	GameDisplay::setColor(DISPLAY_COLOR_DARKBLUE, DISPLAY_COLOR_GRAY);

	for (int i = 0; i < 10; i++)
	{
		if (i < empBar) printf("%s", "■");
		else printf("%s", "□");
	}
	GameDisplay::setColor();

	if (_isEnemyMove) printf("\n%10s", "이동");
	if (_isEnemyAttackPrev || _isEnemyAttack) printf("\n%10s", "공격");

	//플레이어 출력
	GameDisplay::gotoY(40);

	char playerImage[] =
		"    **^^**    \n"
		"   *  ||  *   \n"
		"  *   ||   *  \n"
		"  *   ||   *  \n"
		" *    ||    * \n"
		" *    ||    * \n"
		"*     ||     *\n"
		"*  ___||___  *\n"
		"|↙   ^^   ↘|\n"
		"|     총     |\n";

	if (_playerPosition == 0)
	{
		GameDisplay::draw(playerImage, 16, 40);
	}
	else if (_playerPosition == 1)
	{
		GameDisplay::draw(playerImage, 55, 40);
	}
	else
	{
		GameDisplay::draw(playerImage, 95, 40);
	}

	//플레이어 정보
	GameDisplay::gotoY(51);

	int hpBar = (static_cast<float>(_player->getCurrentHp()) / static_cast<float>(_player->getMaxHp())) * 10;
	int mpBar = (static_cast<float>(_player->getCurrentMp()) / static_cast<float>(_player->getMaxMp())) * 10;
	
	printf("\t\t HP %5d /%5d ", _player->getCurrentHp(), _player->getMaxHp());
	GameDisplay::setColor(DISPLAY_COLOR_DARKRED, DISPLAY_COLOR_GRAY);

	for (int i = 0; i < 10; i++)
	{
		if (i < hpBar) printf("%s", "■");
		else printf("%s", "□");
	}
	GameDisplay::setColor();

	printf("\t\t\t MP %5d /%5d ", _player->getCurrentMp(), _player->getMaxMp());
	GameDisplay::setColor(DISPLAY_COLOR_DARKBLUE, DISPLAY_COLOR_GRAY);

	for (int i = 0; i < 10; i++)
	{
		if (i < mpBar) printf("%s", "■");
		else printf("%s", "□");
	}
	GameDisplay::setColor();
	if (_isPlayerMove) printf("\n%10s", "이동");
	if (_isPlayerAttackPrev || _isPlayerAttack) printf("\n%10s", "공격");

	//플레이어 공격
	char aim[] =
		"    ++++++    \n"
		" ++^      ^++ \n"
		"+^    ||    ^+\n"
		"+   ==※==   +\n"
		"+.    ||    .+\n"
		" ++.      .++ \n"
		"    ++++++    \n";

	char attack[] =
		"          /|  \n"
		"  ------ / |  \n"
		"  +     /  |  \n"
		"   +       +- \n"
		"    +   +   / \n"
		"   + +  +  /  \n"
		"  +      ++   \n";
	GameDisplay::setColor(DISPLAY_COLOR_RED);
	if (_playerPosition == 0)
	{
		if (_isPlayerAttackPrev) GameDisplay::draw(aim, 16, 12);
		else if (_isPlayerAttack) GameDisplay::draw(attack, 16, 12);
	}
	else if (_playerPosition == 1)
	{
		if (_isPlayerAttackPrev) GameDisplay::draw(aim, 55, 12);
		else if (_isPlayerAttack) GameDisplay::draw(attack, 55, 12);
	}
	else
	{
		if (_isPlayerAttackPrev) GameDisplay::draw(aim, 95, 12);
		else if (_isPlayerAttack) GameDisplay::draw(attack, 95, 12);
	}
	GameDisplay::setColor();

	//적 공격
	GameDisplay::setColor(DISPLAY_COLOR_RED);
	if (_enemyPosition == 0)
	{
		if (_isEnemyAttackPrev) GameDisplay::draw(aim, 16, 38);
		else if (_isEnemyAttack) GameDisplay::draw(attack, 16, 38);
	}
	else if (_enemyPosition == 1)
	{
		if (_isEnemyAttackPrev) GameDisplay::draw(aim, 55, 38);
		else if (_isEnemyAttack) GameDisplay::draw(attack, 55, 38);
	}
	else
	{
		if (_isEnemyAttackPrev) GameDisplay::draw(aim, 95, 38);
		else if (_isEnemyAttack) GameDisplay::draw(attack, 95, 38);
	}
	GameDisplay::setColor();
	//스킬 출력
	//q
	//아이템 출력

	LeaveCriticalSection(&battlePrintCriticalSection);
}

