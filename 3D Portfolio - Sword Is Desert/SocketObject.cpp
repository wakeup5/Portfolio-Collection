#include "stdafx.h"
#include "SocketObject.h"


SocketObject::~SocketObject() 
{
	SAFE_RELEASE(_icon);
}


void SocketObject::SetIcon(LPDIRECT3DTEXTURE9 icon, D3DXIMAGE_INFO iconInfo)
{
	_icon = icon;
	SAFE_ADDREF(_icon);
	_iconInfo = iconInfo;
}

void SocketObject::SetIcon(char * resourcePath)
{
	_icon = TEXTURE_MGR->GetTexture(resourcePath, &_iconInfo);
	_icon->AddRef();
}

void SocketObject::SetIcon(std::string resourcePath)
{
	_icon = TEXTURE_MGR->GetTexture(resourcePath, &_iconInfo);
	_icon->AddRef();
}

void SocketObject::RemoveIcon()
{
	SAFE_RELEASE(_icon);
	/*
	_icon = NULL;
	*/
	//	_iconInfo = NULL;
}