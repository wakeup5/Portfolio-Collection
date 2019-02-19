#pragma once
//#include <thread>
#include <process.h>
#include <conio.h>
#include "GameDisplay.h"
#include "Unit.h"

class Battle
{
private:

public:
	Player* _player;
	Enemy* _enemy;

	int _playerPosition;
	bool _isPlayerMove;
	bool _isPlayerAttack;
	bool _isPlayerAttackPrev;

	int _enemyPosition;
	bool _isEnemyMove;
	bool _isEnemyAttack;
	bool _isEnemyAttackPrev;

	bool _isUseHpPotion;
	bool _isUseMpPotion;

	void print();

	Battle(Player*, Enemy*);
	~Battle();

	bool startBattle();
};

