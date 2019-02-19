#pragma once
#include "Singleton.h"

class TextureManager : public Singleton < TextureManager >
{
private:
	std::map<std::string, LPDIRECT3DTEXTURE9> _mapTexture;
	std::map<std::string, D3DXIMAGE_INFO> _mapTextureInfo;

	LPDIRECT3DDEVICE9 m_pDevice;
public:
	TextureManager();
	~TextureManager();

	HRESULT Setup(LPDIRECT3DDEVICE9 d);

	LPDIRECT3DTEXTURE9 GetTexture(char* path, D3DXIMAGE_INFO *i = NULL);
	LPDIRECT3DTEXTURE9 GetTexture(std::string& path, D3DXIMAGE_INFO *i = NULL);

	void RemoveTexture(char* path);
	void RemoveTexture(std::string path);

	void Release();
};

#define TEXTURE_MGR TextureManager::GetInstance()