#pragma once

enum SOCKET_TYPE
{
	ST_ITEM, ST_SKILL
};

class SocketObject
{
protected:
	LPDIRECT3DTEXTURE9 _icon;
	D3DXIMAGE_INFO _iconInfo;

	SOCKET_TYPE _type;
public:
	~SocketObject();

	LPDIRECT3DTEXTURE9 GetIcon() { return _icon; }
	D3DXIMAGE_INFO GetIconInfo() { return _iconInfo; }

	void SetIcon(LPDIRECT3DTEXTURE9 icon, D3DXIMAGE_INFO iconInfo);

	void SetIcon(char* resourcePath);
	void SetIcon(std::string resourcePath);

	void RemoveIcon();

	SOCKET_TYPE GetType() { return _type; }
};
