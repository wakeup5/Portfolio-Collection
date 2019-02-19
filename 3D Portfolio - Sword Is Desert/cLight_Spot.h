#pragma once
#include "cLight.h"
class cLight_Spot : public cLight
{
public:
	float				inAngle;
	float				outAngle;
	float				minRange;			//빛의 최소거리 
	float				maxRange;			//빛의 최대거리
	float				distancePow;		//거리 감쇠율

public:
	cLight_Spot();
	virtual ~cLight_Spot();

	virtual D3DXMATRIXA16 GetLightMatrix();

	virtual void RenderGizmo();
};

