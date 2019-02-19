#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
	_checkTime.clear();
	_endTime = INT_MAX;
	_runTime = 0;
}


Timer::~Timer()
{
}

void Timer::Update(float timeDelta)
{
	if (!_start) return;

	_runTime += timeDelta;
}

void Timer::Start(float endTime)
{
	_start = true;
	_endTime = endTime;
	_runTime = 0;

	_checkTime.clear();
}

void Timer::Stop()
{
	_start = false;

	_checkTime.clear();
}

void Timer::Reset()
{
	Stop();
	Start(_endTime);
}

bool Timer::End()
{
	return Check(-1, _endTime);
}

bool Timer::Check(UINT id, float checkTime)
{
	if (IsExistId(id)) return false;

	if (_runTime >= checkTime)
	{
		_checkTime.insert(id);
		return true;
	}

	return false;
}

bool Timer::IsExistId(int id)
{
	return _checkTime.find(id) != _checkTime.end();
}
