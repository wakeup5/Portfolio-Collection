#include "stdafx.h"
#include "cLight_Spot.h"
#include "Transform.h"


cLight_Spot::cLight_Spot()
{
}


cLight_Spot::~cLight_Spot()
{
}

D3DXMATRIXA16 cLight_Spot::GetLightMatrix()
{
	// 라이트 행렬
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	D3DXMATRIXA16 matLight;
	D3DXMatrixIdentity(&matLight);
	matLight._14 = 2;		//Directional Light Type
	D3DXVECTOR3 pos = this->pTransform->GetWorldPosition();
	memcpy(&matLight._11, &pos, sizeof(D3DXVECTOR3));
	D3DXVECTOR3 forward = this->pTransform->GetForward();
	memcpy(&matLight._21, &forward, sizeof(D3DXVECTOR3));
	memcpy(&matLight._31, &Color, sizeof(FLOAT) * 3);
	matLight._34 = this->Intensity;
	matLight._24 = distancePow;
	matLight._41 = this->maxRange;
	matLight._42 = this->minRange;
	matLight._43 = this->inAngle;
	matLight._44 = this->outAngle;

	return matLight;
}

void cLight_Spot::RenderGizmo()
{
	this->pTransform->RenderGimozo();

}