#include "stdafx.h"
#include "Pet.h"


Pet::Pet()
{
}


Pet::~Pet()
{
}

void Pet::Setup()
{
	this->pMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/meshes/Pet/Pet.x");
		
	_animation = new cSkinnedAnimation;
	_animation->Init((cXMesh_Skinned*)this->pMesh);
	this->pTransform->SetScale(0.03, 0.03, 0.03);
	isMove = false;
	Speed = 0;

	_effects.push_back(EFFECT_MGR->CreateParticleEmitter<Dust>("PET_RUN"));
	_effects.SetTransform(pTransform);
}
void Pet::BaseObjectUpdate(float timeDelta)
{

	SetPos(_PCtrans->GetWorldPosition());
	_animation->Update(timeDelta);
	_effects.Update(timeDelta);
			
	if (isMove)
	{
		if (KEY_MGR->IsStayDown(VK_LSHIFT))
		{
			_animation->Play("Anims_2");
			Speed = 5.1;
		}
		else
		{
			_animation->Play("Anims_2");
			Speed = 3.2;
		}
	
	}
	else
	{
		_animation->Play("NSTAND");
	}
	
	if (IsPosInDistance(_Pos, 2.5))
	{
		_waitTime += timeDelta;
		if (_waitTime > 0.4)
		MoveDestPosition((Speed) * timeDelta);
		LookPosition(_PCtrans->GetForward());
	}
	else
	{
		isMove = false;
		_waitTime = 0;
		
	}

	
}
void Pet::BaseObjectRender()
{
	_animation->Render(this->pTransform);
	_effects.Render();
	//static_cast<cXMesh_Skinned*>(pMesh)->ShowAnimationName(0, 100);
}

void Pet::LookPosition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	D3DXVECTOR3 tempDest = _Pos;
	tempDest.y = temp.y;
	
	D3DXVECTOR3 direction = temp - tempDest;
	direction.z -= 1;
	D3DXVec3Normalize(&direction, &direction);
	pTransform->LookDirection(direction);
}
bool Pet::IsPosInDistance(D3DXVECTOR3 destPos, float distance)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	//destPos.y = 0;
	temp.y = destPos.y;
	return D3DXVec3Length(&(temp - destPos)) > distance;
}
void Pet::MoveDestPosition(float speed)
{
	isMove = true;
	_effects.Start(0.3, false);
	pTransform->MovePositionWorld(GetDeltaDestPos(speed));
}
D3DXVECTOR3 Pet::GetDeltaDestPos(float speed)
{
	D3DXVECTOR3 direction, temp = pTransform->GetWorldPosition();// = _DestPos - pTransform->GetWorldPosition();
	temp.y = _Pos.y;
	_Pos.z += 1.0;
	D3DXVec3Normalize(&direction, &(_Pos - temp));

	return direction * speed;
}
void Pet::SetHeight(float height)
{
	_height = height;
		
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	temp.y = _height;
	
	pTransform->SetWorldPosition(temp);
}
