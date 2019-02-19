#include "stdafx.h"
#include "TextureManager.h"

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

HRESULT TextureManager::Setup(LPDIRECT3DDEVICE9 d)
{
	m_pDevice = d;

	return S_OK;
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(char* path, D3DXIMAGE_INFO *i)
{
	if (_mapTexture.find(path) == _mapTexture.end())
	{
		D3DXIMAGE_INFO info;
		//D3DXCreateTextureFromFile(DEVICE, path, &_mapTexture[path]);
		//D3DXGetImageInfoFromFile(path, &info);

		D3DXCreateTextureFromFileEx(
			m_pDevice,
			path,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&_mapTextureInfo[path],
			NULL,
			&_mapTexture[path]);
	}

	if (i) *i = _mapTextureInfo[path];

	return _mapTexture[path];
}
LPDIRECT3DTEXTURE9 TextureManager::GetTexture(std::string& path, D3DXIMAGE_INFO *i)
{
	return GetTexture((char*)path.c_str(), i);
}

void TextureManager::RemoveTexture(char* path){
	for (auto i = _mapTexture.begin(); i != _mapTexture.end(); ++i){
	
		if (i != _mapTexture.end()){
			_mapTexture.erase(path);
			break;
		}
	}
	for (auto i = _mapTextureInfo.begin(); i != _mapTextureInfo.end(); ++i){

		if (i != _mapTextureInfo.end()){
			_mapTextureInfo.erase(path);
			break;
		}
	}
	
	
}


void TextureManager::RemoveTexture(std::string path){
	for (auto i = _mapTexture.begin(); i != _mapTexture.end(); ++i){

		if (i != _mapTexture.end()){
			_mapTexture.erase(path);
			break;
		}
	}
	for (auto i = _mapTextureInfo.begin(); i != _mapTextureInfo.end(); ++i){

		if (i != _mapTextureInfo.end()){
			_mapTextureInfo.erase(path);
			break;
		}
	}


}



void TextureManager::Release()
{

}