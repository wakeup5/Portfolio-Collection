#include "stdafx.h"
#include "MercenaryWeapon.h"


MercenaryWeapon::MercenaryWeapon()
{
}


MercenaryWeapon::~MercenaryWeapon()
{
}

void MercenaryWeapon::Setup()
{
	D3DXMATRIXA16 MatWorld, scale;
	D3DXMatrixScaling(&scale, 0.04f, 0.04f, 0.04f);
	D3DXMatrixTranslation(&MatWorld, 0.0, 0.0, 0.0);
	MatWorld = scale*MatWorld;
	this->pMesh = RESOURCE_STATICXMESH->GetResource("../Resources/Weapon/Bow.x", &MatWorld);
	
}
void MercenaryWeapon::BaseObjectUpdate(float timeDelta)
{

}

void MercenaryWeapon::BaseObjectRender()
{
	this->pMesh->Render(this->pTransform);
}