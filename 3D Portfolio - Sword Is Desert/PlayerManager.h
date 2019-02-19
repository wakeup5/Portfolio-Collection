#pragma once
#include "Singleton.h"

class Player;
class Inventory;
class Equipment;
class Mercenary;
class Pet;
class Around_Dust;
class Dust;

class PlayerManager : public Singleton < PlayerManager >
{
private:
	Player* _player;
	Inventory* _inven;
	Equipment* _equip;

	cXMesh_Static* _bowMesh;

	Mercenary* _mercenary;
	Pet* _pet;

	//플레이어 이펙트
	Around_Dust* _adEffect;
	Dust* _dEffect;
public:
	PlayerManager();
	~PlayerManager();

	HRESULT Setup();
	void Release();
	void Update(float timeDelta);
	void Render();

	Player* GetPlayer() { return _player; }
	Inventory* GetInventory() { return _inven; }
	Equipment* GetEquipment() { return _equip; }
	Mercenary* GetMercenary() { return _mercenary; }
	Pet* GetPet() { return _pet; }

	bool MonsterCollision(Monster* monster);

	CharacterInfo GetPlayerTotalStatus();
};

#define PLAYER_MGR PlayerManager::GetInstance()