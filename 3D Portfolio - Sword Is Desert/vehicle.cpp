#include "stdafx.h"
#include "vehicle.h"

const Transform* vehicle::PlayerTrans = NULL;
vehicle::vehicle()
{
}

vehicle::~vehicle()
{
}

void vehicle::Setup()
{
	this->pMesh = RESOURCE_SKINNEDXMESH->LoadResource("../Resources/meshes/vehicle/vehicle_Lion.x");
	this->pTransform->SetWorldPosition(10, 0, 10);

	_animation = new cSkinnedAnimation;
	_animation->Init((cXMesh_Skinned*)this->pMesh);
	this->pTransform->SetScale(0.05, 0.05, 0.05);
	isMove = false;
	isJump = false;
	//_animation->Play("JUMP");
}
void vehicle::BaseObjectUpdate(float timeDelta, Transform* playertrans)
{

	_animation->Update(timeDelta);
	this->pTransform = playertrans;
	
	if (isMove && !isJump)
		_animation->Play("RUN");
	else if (!isMove && !isJump )
		_animation->Play("STAND");
	else
	{
		if (!_animation->IsPlay())
		{
			isJump = false;
		}
		_animation->PlayOneShotAfterHold("JUMP");
		
	}
		
}
void vehicle::BaseObjectRender()
{
	_animation->Render(this->pTransform);
}