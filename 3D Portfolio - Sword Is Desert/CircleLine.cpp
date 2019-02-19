#include "stdafx.h"
#include "CircleLine.h"


CircleLine::CircleLine()
{
}


CircleLine::~CircleLine()
{
}

void CircleLine::Setup(float vertexNum, float radius)
{
	float x, z;

	MyPC_Vertex temp, start;

	start.p = D3DXVECTOR3(0, 0, radius);
	start.c = D3DXCOLOR(1, 1, 1, 1);

	//_ver.push_back(start);
	
	for (int i = 0; i < vertexNum; i++)
	{
		float rad = RAD(((int)i / (float)vertexNum) * 360);

		x = cos(rad) * radius;
		z = sin(rad) * radius;

		temp.p = D3DXVECTOR3(x, 0, z);
		temp.c = D3DXCOLOR(1, 1, 1, 1);

		_ver.push_back(temp);
		//_ver.push_back(temp);
	}

	_ver.push_back(_ver[0]);

	_primitiveType = D3DPT_LINESTRIP;
	_count = vertexNum;

	SetSize(0.25f);
}
