#pragma once
#include "cBaseObject.h"
class vehicle :
	public cBaseObject
{

private:
	cSkinnedAnimation* _animation;

	const static Transform* PlayerTrans;
	bool isMove;
	bool isJump;

public:
	vehicle();
	~vehicle();

	virtual void Setup();
	virtual void BaseObjectUpdate(float timeDelta, Transform* playertrans);
	virtual void BaseObjectRender();
	bool GetisJump(){ return isJump; }
	void SetisMove(bool move){ isMove = move; }
	void SetisJump(bool jump){ isJump = jump; }
	static void SetPlayerTrans(const Transform* ptrans){ PlayerTrans = ptrans; }
};

