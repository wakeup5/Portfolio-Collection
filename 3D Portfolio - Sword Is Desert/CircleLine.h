#pragma once
#include "Figure.h"

class CircleLine : public Figure
{
public:
	CircleLine();
	virtual ~CircleLine();

	void Setup(float vertexNum, float radius);
};

