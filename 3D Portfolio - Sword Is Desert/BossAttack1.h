#pragma once
#include "cQuadParticleEmitter.h"
#include "stdafx.h"

class BossAttack1 : public cQuadParticleEmitter
{
public:
	BossAttack1() {}
	virtual ~BossAttack1() {}

	void Setup()
	{
		//배열을 2 개이상 
		VEC_COLOR colors;
		colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f));
		colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1));
		colors.push_back(D3DXCOLOR(1.0, 1.0f, 1.0f, 0.2f));

		VEC_SCALE scales;
		scales.push_back(1.0f);
		scales.push_back(1.0f);
		scales.push_back(1.0f);
		scales.push_back(1.5f);
		scales.push_back(2.0f);

		LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource(
			"../Resources/FX/Test/PoisonPurple.png");

		cQuadParticleEmitter::Init(
			200, 20, 2, 2,
			D3DXVECTOR3(0, 10, 0), D3DXVECTOR3(0, 10, 0), //속도
			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), //가속도
			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), //시작 각도
			D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), //각속도
			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), //각가속도
			colors, scales, 2, 2, pTex, false);
	}

	virtual void BaseObjectUpdate(float timeDelta) override
	{
		//float angle = RandomFloatRange(0, RAD(3600));
		//pTransform->SetRotateLocal(0, angle / RandomFloatRange(1, 10), 0);
		cQuadParticleEmitter::BaseObjectUpdate(timeDelta);
	}
};

