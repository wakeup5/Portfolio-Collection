#pragma once
#include "cBaseObject.h"
class Bomb :
	public cBaseObject
{
private:
	cSkinnedAnimation* _animation;
public:
	Bomb();
	~Bomb();

	void Setup();
	virtual void BaseObjectUpdate(float timeDelta);
	virtual void BaseObjectRender();
};

