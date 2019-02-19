#pragma once
#include "MapBase.h"
#include "Day.h"
#include "SpriteEffect.h"

class MapView
{
private:
	std::vector<MapBase*> _bases;

	bool _isOpen;

	Transform _mapViewPoint;
	float _dist;

	POINT prev;

	CircleLine _smallCircle;
	CircleLine _bigCircle;
	CircleLine _dayCircle;

	Day* _day = NULL;

	NameTag _dayLight;
	NameTag _nightLight;

	//ÀÌÆåÆ® °ü¸®
	//EffectVector _quadEffectVector;

	Transform _pointTrans;
	SpriteEffect* _effect;
	SpriteEffect* _effectP;
	EffectVector _pointEffect;

	NameTag _npcPos;
public:
	MapView();
	virtual ~MapView();

	void Setup();
	void Release();
	void Update();
	void Render();
	void EffectRender();

	void AddBase(MapBaseInfo &info);

	bool IsOpen() { return _isOpen; }
	void SetOpenState(bool is) { _isOpen = is; if (_isOpen) Open(); else Close(); }

	void Open();
	void Close();

	void SetDay(Day* day) { _day = day; }

	std::string GetNearBaseName(D3DXVECTOR3 pos);
};

