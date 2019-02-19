#include "stdafx.h"
#include "Line.h"


Line::Line()
	: _start(0, 0, 0)
	, _end(0, 0, 0)
{

}


Line::~Line()
{
}

void Line::Setup()
{
	MyPC_Vertex temp;

	temp.p = _start;
	temp.c = 0xffffffff;

	_ver.push_back(temp);

	temp.p = _end;
	temp.c = 0xffffffff;

	_ver.push_back(temp);

	_primitiveType = D3DPT_LINELIST;
	_count = 1;
}

void Line::Render()
{
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 matWorld;
	D3DXVECTOR3 pos = pTransform.GetWorldPosition();

	D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);

	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	DEVICE->SetTransform(D3DTS_VIEW, &CAMERA->GetViewMatrix());
	DEVICE->SetTransform(D3DTS_PROJECTION, &CAMERA->GetProjectionMatrix());
	
	DEVICE->SetFVF(MyPC_Vertex::FVF);
	DEVICE->DrawPrimitiveUP(_primitiveType, _count, &_ver[0], sizeof(MyPC_Vertex));
}
