#include "stdafx.h"
#include "MapBase.h"


MapBase::MapBase()
{
}


MapBase::~MapBase()
{
}

void MapBase::Setup(tagMapBaseInfo info)
{
	_info = info;
	_nameTag.SetTexture(_info.IconPath);
	_nameTag.SetPosition(_pos = _info.Pos);

	D3DXVECTOR3 start, end;
	start = end = info.Pos;
	end.y = 0;
	_line.Setup();
	_line.SetAlpha(0);
	_line.SetStart(start);
	_line.SetEnd(end);
}

void MapBase::Release()
{
	_nameTag.Release();
}

void MapBase::Update()
{
	_nameTag.Look(CAMERA->GetWorldPosition(), D3DXVECTOR3(0, 1, 0));
}

void MapBase::Render()
{
	_nameTag.RenderToSprite(SPRITE, CAMERA);
	//D3DXVECTOR3 pos = _nameTag.GetScreenPos(CAMERA);

	//DXFONT_MGR->PrintText(_info.Name, pos.x, pos.y, 0xffffffff);

	_line.Render();
}
