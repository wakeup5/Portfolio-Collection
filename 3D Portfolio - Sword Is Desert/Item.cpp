#include "stdafx.h"
#include "Item.h"


Item::Item()
{
	_type = ST_ITEM;
}


Item::~Item()
{
	_type = ST_ITEM;
}

void Item::Setup()
{

}

void Item::Setup(ItemInfo & info)
{
	_info = info;
	SetIcon(_info.IconPath);

	pMesh = RESOURCE_STATICXMESH->GetResource(_info.ResourcePath, &_info.Matrix);

	if(_info.isRender) SetActive(true);
}

void Item::Render()
{
	if (_info.isRender && pMesh)
	{
		cBaseObject::Render();
	}
	this->BoundBox.RenderGizmo(this->pTransform);
}

void Item::RemoveIcon(){
	//SocketObject::RemoveIcon();
	TEXTURE_MGR->RemoveTexture(_info.IconPath);

	memset(&_info, 0, sizeof(_info));
}