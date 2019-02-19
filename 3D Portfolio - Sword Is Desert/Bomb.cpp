#include "stdafx.h"
#include "Bomb.h"


Bomb::Bomb()
{
}


Bomb::~Bomb()
{
}

void Bomb::Setup()
{
	this->pMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/FX/Test/bomb.x");

	_animation = new cSkinnedAnimation;
	_animation->Init((cXMesh_Skinned*)this->pMesh);
	this->pTransform->SetScale(0.1, 0.1, 0.1);
}
void Bomb::BaseObjectUpdate(float timeDelta)
{
	_animation->Play("bomb");
	_animation->Update(timeDelta);
}
void Bomb::BaseObjectRender()
{

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	_animation->Render(this->pTransform);

	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	DEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	DEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

	DEVICE->SetTexture(0, NULL);
}
