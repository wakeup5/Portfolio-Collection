#include "stdafx.h"
#include "BowEffect02.h"


BowEffect02::BowEffect02()
{
}


BowEffect02::~BowEffect02()
{

}
void BowEffect02::Setup()
{
	//배열을 2 개이상 
	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.01f));

	VEC_SCALE scales;
	scales.push_back(0.5f);
	scales.push_back(0.1f);


	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource(
		"../Resources/FX/Test/Sword2.png");

	cPartcleEmitter::Init(
		5000,				//최대 파티클 수
		1000,				//초당 파티클 발생 량
		0.3,					//하나의 파티클 입자 라이프 최소값
		0.6,					//하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(0,0,0),	//파티클 입자 속도 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 0),	//파티클 입자 속도 최대값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 0),	//파티클 입자 가속 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 0), //파티클 입자 가속 최대값 ( 로컬기준 )
		colors,				//컬러 배열
		scales,				//스케일 배열
		0.1,				//입자크기 최소값
		0.2,				//최대값
		pTex,				//텍스쳐
		false);


	EmissionType = PATICLE_EMISSION_TYPE::SPHERE;
	SphereEmissionRange = 0.0f;
	StartEmission();
}
