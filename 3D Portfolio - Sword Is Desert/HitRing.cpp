#include "stdafx.h"
#include "HitRing.h"


HitRing::HitRing()
{
}


HitRing::~HitRing()
{
}

void HitRing::Setup()
{
	

		VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
//	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	VEC_SCALE scales;
	scales.push_back(0.1f);
	scales.push_back(8.0f);

	cQuadParticleEmitter::Init(
		100,
		2,
		0.6f,
		0.7f,
		D3DXVECTOR3(0, -1, 0),
		D3DXVECTOR3(0, -1, 0),
		D3DXVECTOR3(0, 5, 0),
		D3DXVECTOR3(0, 6, 0),
		D3DXVECTOR3(4.7, 0, 0),		//초기시작시 회전Min
		D3DXVECTOR3(4.7, 0, 0),     //초기시작시 회전Max
		D3DXVECTOR3(0, 0, 15),				//초당 회전할 회전 량 Min
		D3DXVECTOR3(0, 0, 15),				//초당 회전할 화전 량 Max
		D3DXVECTOR3(0, 0, 5),				//초당 회전 가속 Min
		D3DXVECTOR3(0, 0, 5),				//초당 회전 가속 Max
		colors, scales,
		1, 1, 
		RESOURCE_TEXTURE->GetResource("../Resources/FX/Test/SWeffect02.png"),
		true);

	//StartEmission();
	
	//카메라 소팅
	SetCameraSort(CAMERA, true);
}