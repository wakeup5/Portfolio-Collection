#pragma once
#include "cLight_Direction.h"

#define DAY_CHANGETIME 30
#define DAY_TIME 600 //낮 시간이며 밤은 낮 시간의 1 / 2
class Day
{
private:
	float _changeTime = DAY_CHANGETIME;
	float _daliyTime = DAY_TIME * 2;
	float _currentTime;
	int _flag;
	float _alpha;

	LPDIRECT3DCUBETEXTURE9			evironmentTexture;			//환경 Texture
	LPDIRECT3DCUBETEXTURE9			evironmentRainTexture;
	LPDIRECT3DCUBETEXTURE9			evironmentTexture2;			//환경 Texture
	LPD3DXMESH						evironmemtSphereMesh;		//환경 구
	LPD3DXEFFECT					evironmentEffect;			//환경 Effect

	std::vector<cLight*>	_lights;

	cLight_Direction* _dayLight;
	cLight_Direction* _nightLight;

	bool _isRain;
	Timer* _rainChange;
	Timer* _thunderTimer;
public:
	cPartcleEmitter* _rain;

private:
	SpriteEffect* _sunlight[5];
public:
	Day();
	virtual ~Day();

	void Setup();
	void Release();
	void Update();
	void Render();

	void SunlightRender();

	void SetEnvironment(std::string cubeFilePath)
	{
		
	}

	cLight_Direction* GetDayLight()
	{
		return _dayLight;
	}

	cLight_Direction* GetNightLight()
	{
		return _nightLight;
	}

	float GetDayLightAngle() 
	{
		if (_flag > 0)
		{
			return RAD(((_currentTime) / _daliyTime) * 360); 
		}
		else
		{
			return RAD(180 + (((_currentTime) / _daliyTime) * 360));
		}
	}

	float GetNightLightAngle()
	{
		if (_flag < 0)
		{
			return RAD(((_currentTime) / _daliyTime) * 360);
		}
		else
		{
			return RAD(180 + (((_currentTime) / _daliyTime) * 360));
		}
	}

	float GetTime() 
	{
		if (_flag > 0)
		{
			return (_currentTime / (_daliyTime / 2));
		}
		else
		{
			return 1 + (_currentTime / (_daliyTime / 2));
		}
	}
};

