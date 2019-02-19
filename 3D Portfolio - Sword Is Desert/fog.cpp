#include "stdafx.h"
#include "fog.h"


fog::fog()
{
}


fog::~fog()
{
}

void fog::Setup()
{

	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	VEC_SCALE scales;
	scales.push_back(4);
	scales.push_back(4);
		
	//cParticleQuad::GetParticleVertex(
	//	)

	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource(
		"../Resources/FX/Test/RedRing.png");

	cQuadParticleEmitter::Init(1,0.5,2,
		2,				//라이브 타임
		D3DXVECTOR3(0.0, 0.0, 0),	//파티클 입자 속도 최소값 ( 로컬기준 )
		D3DXVECTOR3(0.0, 0.0, 0.0),	//파티클 입자 속도 최대값 ( 로컬기준 )
		D3DXVECTOR3(0.0, 0.0, 0),	//파티클 입자 가속 최소값 ( 로컬기준 )
		D3DXVECTOR3(0.0, 0.0, 0),	//초당 회전 값
		D3DXVECTOR3(4.7, 0, 0),
		D3DXVECTOR3(4.7, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		colors,
		scales,
		//초당 회전 증가값
		1,1,
		pTex,
		true
		);

	cQuadParticleEmitter::SetUVLength(70, 1,true);



}