#pragma once
#include "Figure.h"

class Line :
	public Figure
{
private:
	D3DXVECTOR3 _start, _end;
public:
	Line();
	virtual ~Line();

	void Setup();

	void SetStart(D3DXVECTOR3 s) { _start = s; _ver[0].p = s; }
	void SetEnd(D3DXVECTOR3 e) { _end = e; _ver[1].p = e; }

	void Render();
};

