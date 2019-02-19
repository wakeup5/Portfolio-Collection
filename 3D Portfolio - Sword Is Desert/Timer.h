#pragma once
class Timer
{
private:
	float _endTime, _runTime;
	bool _start;

	std::set<int> _checkTime;
public:
	Timer();
	virtual ~Timer();

	void Update(float timeDelta);

	void Start(float endTime);
	void Stop();
	void Reset();

	bool End();
	bool Check(UINT id, float checkTime);

	float GetTime() { return _runTime; }
private:
	bool IsExistId(int id);
};

