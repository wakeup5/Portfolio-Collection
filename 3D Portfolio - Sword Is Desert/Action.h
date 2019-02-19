#pragma once
//#include "Transform.h"
#include "stdafx.h"

class ActionObject;
interface ActionDeligate;

class Action
{
public:
	virtual void Start(void* = NULL) = 0;
	virtual void Update() = 0;

	void SetDeligate(ActionDeligate* d) { _deligate = d; }
	void SetObject(ActionObject* o) { _object = o; }

	void SetTime(float time) { _endTime = time; }
protected:
	ActionDeligate*		_deligate = NULL;
	ActionObject*		_object = NULL;

	float _startTime;
	float _currentTime;
	float _endTime;
};


class ActionObject : public cBaseObject
{
public:
	ActionObject() {}
	virtual ~ActionObject() {}

	void SetAction(Action* a) { _action = a; if(_action) _action->SetObject(this); }
protected:
	Action* _action = NULL;
};


interface ActionDeligate
{
public:
	virtual void OnActionFinish() {}
};
