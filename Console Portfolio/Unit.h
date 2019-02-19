#pragma once
#include <vector>
#include <time.h>
#include <iostream>
#include "Skill.h"
#include "Item.h"

using namespace std;

class Container;

class Unit
{
protected:
	char* _name;

	int _level;		/**< 레벨 */
	int _exp;		/**< 경험치 */

	int _maxHp;		/**< 최대 피 */
	int _maxMp;		/**< 최대 마나 */
	int _currentHp;	/**< 현재 피 */
	int _currentMp;	/**< 현재 마나 */
	int _atk;		/**< 공격력 */
	int _def;		/**< 방어력 */

	int _str;		/**< 힘 - 공격력과 방어력에 영향 */
	int _dex;		/**< 민첩 - 방어력과 공격, 이동 속도, 회피에 영향 */
	int _luck;		/**< 운 - 회피와 아이템 드랍율에 영향 */

	int _agi;		/**< 회피 - X/10000값으로 적용됨. */

	int _atkSpeed;	/**< 일반 공격 쿨타임 */
	int _moveSpeed;	/**< 이동 쿨타임 */

	vector<Skill> _skillVector;
	//vector<Item>* _itemVector;

public:
	//Unit();
	Unit(char* name, int lv = 1, int hp = 1000, int mp = 300, int atk = 50, int def = 30, int str = 30, int dex = 20, int luck = 10, int agi = 500, int atkS = 1200, int moveS = 1200);
	~Unit();

	void levelUp();
	int getTotalAtk();
	int getTotalDef();
	int getTotalAgi();
	int getTotalAtkSpeed();
	int getTotalMoveSpeed();

	char* getName(){ return _name; }
	void setName(char* name){ _name = name; }
	int getLevel(){ return _level; }
	void setLevel(int level){ _level = level; }
	int getExp(){ return _exp; }
	void setExp(int exp){ _exp = exp; }
	virtual int getMaxHp(){ return _maxHp + _level * 50; }
	int getMaxMp(){ return _maxMp + _level * 30; }
	int getCurrentHp(){ return _currentHp; }
	void setCurrentHp(int hp){ _currentHp = hp; if (_currentHp > getMaxHp()) _currentHp = getMaxHp(); }
	int getCurrentMp(){ return _currentMp; }
	void setCurrentMp(int mp){ _currentMp = mp; if (_currentMp > getMaxMp()) _currentMp = getMaxMp(); }
	virtual int getAtk(){ return _atk + _level * 10; }
	void setAtk(int atk){ _atk = atk; }
	virtual int getDef(){ return _def + _level * 5; }
	void setDef(int def){ _def = def; }
	virtual int getStr(){ return _str + _level * 5; }
	void setStr(int str){ _str = str; }
	virtual int getDex(){ return _dex + _level * 3; }
	void setDex(int dex){ _dex = dex; }
	int getLuck(){ return _luck + _level * 3; }
	void setLuck(int luck){ _luck = luck; }
	int getAgi(){ return _agi + _level * 10; }
	void setAgi(int agi){ _agi = agi; }
	int getAtkSpeed(){ return _atkSpeed - _level * 3; }
	virtual void setAtkSpeed(int speed){ _atkSpeed = speed; }
	int getMoveSpeed(){ return _moveSpeed - _level * 5; }
	virtual void setMoveSpeed(int speed){ _moveSpeed = speed; }
	vector<Skill> getSkills(){ return _skillVector; }
	void addSkill(Skill*);

};

class Player : public Unit
{
private:
	vector<Item> _inventoryVector;
	vector<Item>::iterator _inventoryIterator;
	Item* _helmet;
	Item* _top;
	Item* _pants;
	Item* _shoes;
	Item* _glove;
	Item* _weapon;
public:
	Player(char* name, int lv = 1, int hp = 1000, int mp = 300, int atk = 50, int def = 30, int str = 30, int dex = 20, int luck = 10, int agi = 500, int atkS = 1200, int moveS = 1200);
	~Player();

	vector<Item> getInventory(){ return _inventoryVector; }

	int getMaxHp(){ return _maxHp + _level * 50 + _helmet->getMain() + _top->getSub(); }
	int getAtk(){ return _atk + _level * 10 + _weapon->getMain(); }
	int getDef(){ return _def + _level * 5 + _top->getMain() + _pants->getMain() + _helmet->getSub(); }
	int getStr(){ return _str + _level * 5 + _glove->getMain() + _weapon->getSub(); }
	int getDex(){ return _dex + _level * 3 + _shoes->getMain() + _pants->getSub(); }
	int getAtkSpeed(){ return _atkSpeed - _level * 3 - _glove->getSub(); }
	int getMoveSpeed(){ return _moveSpeed - _level * 5 - _shoes->getSub(); }

	void removePotion(Item*);
};

enum ENEMY
{
	ENEMY_1 = 0,
	ENEMY_2,
	ENEMY_3,
	ENEMY_4,
	ENEMY_5,
	ENEMY_6,
	ENEMY_7,
	ENEMY_8,
	ENEMY_9,
	ENEMY_10,
	ENEMY_11,
	ENEMY_12,
	ENEMY_13,
	ENEMY_14,
	ENEMY_LINA,
	ENEMY_JINA,
	ENEMY_K14,
};

class Enemy : public Unit
{
private:
	Item* _uniqueItem;

public:
	Enemy(char* name, int lv = 1, int hp = 1000, int mp = 300, int atk = 50, int def = 30, int str = 30, int dex = 20, int luck = 10, int agi = 500, int atkS = 1200, int moveS = 1200);
	~Enemy();

	void setUniqueItem(Item*);
	Item* getUniqueItem();

	Item dropItem();
};

class UnitManager
{
private:
	Player* _player;
	Enemy* _enemys[17];

public:
	UnitManager();
	~UnitManager();

	Player* getPlayer();
	Enemy* getEnemy(int num);

};
