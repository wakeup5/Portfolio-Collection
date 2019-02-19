#pragma once
#include <iostream>
#include <conio.h>
#include "GameDisplay.h"
#include "GameSound.h"
#include "Unit.h"
//#include "Container.h"

enum MAP
{
	MAP_1 = 1,
	MAP_2,
	MAP_3,
	MAP_4,
	MAP_5,
	MAP_6,
	MAP_7,
	MAP_8,
	MAP_9,
	MAP_10,
	MAP_11,
	MAP_12,
	MAP_13,
	MAP_14,
	MAP_PARK,
	MAP_SCHOOL,
	MAP_LAST,
};

struct tagMapInfo
{
	char* name;
	char* enemyName;
	char* specialItemInfo;
	char* mapInfo;
};

class Map
{
private:
	char* getPositionMap(int);
	void printPositionMap(int);

	UnitManager* _unitManager;
public:
	Map(UnitManager*);
	~Map();

	int openMap();
};

