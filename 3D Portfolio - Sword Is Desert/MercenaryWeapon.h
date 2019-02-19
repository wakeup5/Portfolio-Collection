#pragma once
#include "cBaseObject.h"
class MercenaryWeapon :
	public cBaseObject
{
public:
	MercenaryWeapon();
	~MercenaryWeapon();

	virtual void Setup();
	virtual void BaseObjectUpdate(float timeDelta);
	virtual void BaseObjectRender();
};

