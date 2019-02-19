#pragma once

#ifndef interface
#define interface struct
#endif
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace LerpAnimation
{
	float ClampZeroOne(float t) { return min(1, max(0, t)); }

	interface IEasing { virtual float Lerp(float t) = 0; };

	struct Linear : public IEasing
	{
		float Lerp(float t) { return ClampZeroOne(t); }
	};

	struct EaseInQurd : public IEasing
	{
		float Lerp(float t) 
		{ 
			float time = ClampZeroOne(t);  

			return pow(time, 2);
		}
	};
	struct EaseOutQurd : public IEasing
	{
		float Lerp(float t)
		{
			float time = ClampZeroOne(t);

			return sqrt(time);
		}
	};

	struct EaseInQuart : public IEasing
	{
		float Lerp(float t)
		{
			float time = ClampZeroOne(t);

			return pow(time, 4);
		}
	};
	struct EaseOutQuart : public IEasing
	{
		float Lerp(float t)
		{
			float time = ClampZeroOne(t);

			return sqrt(sqrt(time));
		}
	};

	template < typename T >
	float Lerp(float t)
	{
		T e;
		return e.Lerp(t);
	}

	template < typename T >
	D3DXVECTOR3 Lerp(D3DXVECTOR3 from, D3DXVECTOR3 to, float t)
	{
		float time = Lerp<T>(t);
		return from * (1 - time) + to * time;
	}

	template < typename T >
	float Lerp(float from, float to, float t)
	{
		float time = Lerp<T>(t);
		return from * (1 - time) + to * time;
	}

}
