#pragma once
#include "Action.h"
#include "Player.h"
#include "Timer.h"

class ActionPlayerDeligate : public ActionDeligate
{
private:
	Player* _player = NULL;

public:
	void SetPlayer(Player* player) { _player = player; }

	void OnActionFinish();
	void OnActionFinish(float cTime);
};

class ActionPlayer : public Action
{
protected:
	static Player* _player;
	static std::map<std::string, ActionPlayer*> _actionName;

	D3DXVECTOR3 _destPos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 _tempPos = D3DXVECTOR3(0, 0, 0);

	float _start;
	float _current;
	float _end;

	//Å¸ÀÌ¸Ó
	Timer* _timer;

public:
	float Damage = 1;

public:
	ActionPlayer() { _timer = TIME_MGR->CreateTimer(); }
	~ActionPlayer();

	virtual void Start(void* lpvoid = NULL);
	virtual void Update();

	static void SetPlayer(Player* player) { _player = player; ReadyToActionPlayer(player); }

	static ActionPlayer* GetAction(std::string name);
	static void ReadyToActionPlayer(Player* player);
protected:
	void SetDestPosition(D3DXVECTOR3 pos);
	void SetAction(std::string name);

	D3DXVECTOR3 GetTempPos(float s);

	bool isTimeInRange(float start, float end);
	void UpdatePlayerAttackTime(float start, float end);
};

class ActionPlayerRollingBack : public ActionPlayer
{
public:
	ActionPlayerRollingBack();
	virtual void Start(void* lpvoid = NULL);
	virtual void Update();
};

class ActionPlayerRollingLeft : public ActionPlayer
{
public:
	ActionPlayerRollingLeft();
	virtual void Start(void* = NULL);
	virtual void Update();
};

class ActionPlayerRollingRight : public ActionPlayer
{
public:
	ActionPlayerRollingRight();
	virtual void Start(void* = NULL);
	virtual void Update();
};

class ActionPlayerAttack : public ActionPlayer
{
private:
	std::vector<KeyType> _command;
public:
	ActionPlayerAttack();
	virtual void Start(void* lpvoid = NULL);
	virtual void Update();
};

class ActionPlayerWalkAttack : public ActionPlayer
{
private:
	std::vector<KeyType> _command;

public:
	ActionPlayerWalkAttack();
	virtual void Start(void* lpvoid = NULL);
	virtual void Update();
};

class ActionPlayerSideAttackLeft : public ActionPlayer
{
private:
	std::vector<KeyType> _command;

public:
	ActionPlayerSideAttackLeft();
	virtual void Start(void* lpvoid = NULL);
	virtual void Update();
};

class ActionPlayerSideAttackRight : public ActionPlayer
{
private:
	std::vector<KeyType> _command;

public:
	ActionPlayerSideAttackRight();
	virtual void Start(void* lpvoid = NULL);
	virtual void Update();
};

class ActionPlayerSheildAttack : public ActionPlayer
{
public:
	ActionPlayerSheildAttack();
	virtual void Start(void* lpvoid = NULL);
	virtual void Update();
};

class ActionPlayerUltimate : public ActionPlayer
{
private:
	float _ySpeed;

	bool _up;
public:
	ActionPlayerUltimate();
	virtual void Start(void* lpvoid = NULL);
	virtual void Update();
};