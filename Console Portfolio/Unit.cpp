#include "Unit.h"
#include "Container.h"

Unit::Unit(char* name, int lv, int hp, int mp, int atk, int def, int str, int dex, int luck, int agi, int atkS, int moveS)
{
	_name = name;
	_level = lv;
	_maxHp = hp;
	_maxMp = mp;
	_atk = atk;
	_def = def;
	_str = str;
	_dex = dex;
	_luck = luck;
	_agi = agi;
	_atkSpeed = atkS;
	_moveSpeed = moveS;

	setCurrentHp(getMaxHp());
	setCurrentMp(getMaxMp());
}

Unit::~Unit()
{
}

void Unit::levelUp()
{
	_level++;
	//�ɷ�ġ ����
	/*
	_maxHp += 50;
	_maxMp += 30;
	_currentHp = _maxHp;
	_currentMp = _maxMp;

	_atk += 10;
	_def += 5;

	_str += 5;
	_dex += 3;
	_luck += 3;

	_agi += 10;

	_atkSpeed -= 3;
	_moveSpeed -= 5;
	*/
}
int Unit::getTotalAtk()
{
	int result = 0;
	result = static_cast<int>(getAtk() * (1 + (static_cast<float>(getStr()) / 100)));

	return result;
}
int Unit::getTotalDef()
{
	int result = 0;
	result = (getDef() * (1 + (getStr() / 200))) + (getDex() / 5);

	return result;
}
int Unit::getTotalAgi()
{
	int result = 0;
	result = getAgi() + (getDex() / 2) + getLuck();

	return result;
}
int Unit::getTotalAtkSpeed()
{
	int result = 0;
	result = getAtkSpeed() - (getDex() / 3);

	return result;
}
int Unit::getTotalMoveSpeed()
{
	int result = 0;
	result = getMoveSpeed() - (getDex() / 2);

	return result;
}

void Unit::addSkill(Skill* skill)
{
	_skillVector.push_back(*skill);
}

Player::Player(char* name, int lv, int hp, int mp, int atk, int def, int str, int dex, int luck, int agi, int atkS, int moveS)
	: Unit(name, lv, hp, mp, atk, def, str, dex, luck, agi, atkS, moveS)
{
	_helmet = new Item(ITEM_TYPE_ARMOR_HELMET, "�⺻ ����", 0, 0);
	_top = new Item(ITEM_TYPE_ARMOR_TOP, "�⺻ ����", 0, 0);
	_pants = new Item(ITEM_TYPE_ARMOR_PANTS, "�⺻ ����", 0, 0);
	_shoes = new Item(ITEM_TYPE_ARMOR_SHOES, "�⺻ �Ź�", 0, 0);
	_glove = new Item(ITEM_TYPE_ARMOR_GLOVE, "�⺻ �尩", 0, 0);
	_weapon = new Item(ITEM_TYPE_WEAPON, "����", 10, 1, "��𼱰� �ֿ� �����̴�.");

	//_inventoryVector;
	//_inventoryIterator;
}
Player::~Player()
{
	delete _helmet, _top, _pants, _shoes, _glove, _weapon;
	//delete _inventoryIterator, _inventoryVector;
}

void Player::removePotion(Item* item)
{
	if (!(item->getType() == ITEM_TYPE_POTION_HP || item->getType() == ITEM_TYPE_POTION_MP)) return;

	for (int i = 0; i < _inventoryVector.size(); i++)
	{
		if (strcmp(_inventoryVector[i].getName(), item->getName()) == 0)
		{
			_inventoryVector.erase(_inventoryVector.begin() + i);
			break;
		}
	}
}

Enemy::Enemy(char* name, int lv, int hp, int mp, int atk, int def, int str, int dex, int luck, int agi, int atkS, int moveS)
	: Unit(name, lv, hp, mp, atk, def, str, dex, luck, agi, atkS, moveS)
{
	_uniqueItem = new Item(ITEM_TYPE_WEAPON, "����ũ ������", 0, 0, "����ũ �ϱ� �ѵ�.. ����?");
}

Enemy::~Enemy()
{

}

void Enemy::setUniqueItem(Item* item)
{
	_uniqueItem = item;
}

Item* Enemy::getUniqueItem()
{
	return _uniqueItem;
}

Item Enemy::dropItem()
{
	srand((unsigned int)time(NULL));

	int randNum = rand() % 1000;

	if (randNum < 100)
	{
		return *_uniqueItem;
	}
	else
	{
		return Container::getInstance()->getItemManager()->randomItem();
	}
}

UnitManager::UnitManager()
{
	_player = new Player("K17");
	_player->addSkill(new Skill("���ػ��", 1, 1.5, 0.2, 0, 0, 5000, "���� �����Ͽ� �� ���� �ϰ��� ���Ѵ�."));
	_player->addSkill(new Skill("���߻��", 1, 2.0, 0.1, 10, 0, 6000, "������ �� �� �������� ���."));
	_player->addSkill(new Skill("3���߻��", 1, 3.0, 0.15, 0, 0, 10000, "������ �� �� �������� ���."));
	_player->addSkill(new Skill("��弦!", 1, 5.0, 0.2, 100, 10, 15000, "���� �Ӹ��� �����Ͽ� �ſ� ������ �ϰ��� ���Ѵ�."));

	_enemys[ENEMY_1] = new Enemy("�ڿ���");
	_enemys[ENEMY_2] = new Enemy("����ȫ");
	_enemys[ENEMY_3] = new Enemy("�赿��");
	_enemys[ENEMY_4] = new Enemy("���ѿ�");
	_enemys[ENEMY_5] = new Enemy("������");
	_enemys[ENEMY_6] = new Enemy("������");
	_enemys[ENEMY_7] = new Enemy("������");
	_enemys[ENEMY_8] = new Enemy("�̰���");
	_enemys[ENEMY_9] = new Enemy("�迵��");
	_enemys[ENEMY_10] = new Enemy("�赵��");
	_enemys[ENEMY_11] = new Enemy("���ѱ�");
	_enemys[ENEMY_12] = new Enemy("������");
	_enemys[ENEMY_13] = new Enemy("�赿��");
	_enemys[ENEMY_14] = new Enemy("�輱��");
	_enemys[ENEMY_LINA] = new Enemy("����");
	_enemys[ENEMY_JINA] = new Enemy("����");
	_enemys[ENEMY_K14] = new Enemy("K14 - DeadFace");

}

UnitManager::~UnitManager()
{
	delete _player;
	delete[] _enemys;
}

Player* UnitManager::getPlayer()
{
	return _player;
}
Enemy* UnitManager::getEnemy(int num)
{
	return _enemys[num];
}