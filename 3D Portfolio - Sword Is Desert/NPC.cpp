#include "stdafx.h"
#include "NPC.h"

Transform* NPC::PlayerTrans = NULL;
NPC::NPC()
{
}


NPC::~NPC()
{
}

void NPC::Setup(NpcInfo &info)
{
	this->pMesh = RESOURCE_SKINNEDXMESH->LoadResource(info.ResourcePath);// "../Resources/meshes/NPC01/NPC01.x");
	this->pTransform->SetWorldPosition(info.Pos);

	_animation = new cSkinnedAnimation;
	_animation->Init((cXMesh_Skinned*)this->pMesh);
	this->pTransform->SetScale(0.05, 0.05, 0.05);

	_animation->Play("TALK");

	_info = info;
}
void NPC::Release()
{
	_animation->Release();
	//SAFE_DELETE(_animation);
}

void NPC::BaseObjectUpdate(float timeDelta)
{
	_animation->Update(timeDelta);
}
void NPC::BaseObjectRender()
{
	_animation->Render(this->pTransform);

	//animation->RenderBoneName(CAMERA, this->pTransform);

	//static_cast<cXMesh_Skinned*>(pMesh)->ShowAnimationName(0, 100);

	//DXFONT_MGR->PrintText(std::to_string(_keyBuffer.size()), 10, 80, 0xffffffff);
}