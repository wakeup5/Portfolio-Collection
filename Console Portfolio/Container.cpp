#include "Container.h"


Container::Container()
{
	_dungeon = new Dungeon();
	_story = new Story();
	_unitManager = new UnitManager();
	_itemManager = new ItemManager();
}

Container::~Container()
{
	delete _dungeon;
	delete _story;
	delete _unitManager;
	delete _itemManager;
}

Container* Container::_instance = NULL;

Container* Container::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new Container;
	}

	return _instance;
}

Dungeon* Container::getDungeon()
{
	return _dungeon;
}

Story* Container::getStory()
{
	return _story;
}

UnitManager* Container::getUnitManager()
{
	return _unitManager;
}

ItemManager* Container::getItemManager()
{
	return _itemManager;
}
