#include "StdAfx.h"
#include "cLight.h"
#include "Transform.h"

cLight::cLight(void)
	: Intensity( 1.0 )
{
	this->pTransform = new Transform();
}


cLight::~cLight(void)
{
	SAFE_DELETE( this->pTransform );
}
