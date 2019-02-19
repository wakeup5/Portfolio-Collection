#pragma once
#include "NameTag.h"
#include "Line.h"

typedef struct tagMapBaseInfo
{
	char* Name;
	char* IconPath;
	
	D3DXVECTOR3 Pos;
} MapBaseInfo;

class MapBase
{
private:
	MapBaseInfo _info;

	D3DXVECTOR3 _pos;

	NameTag _nameTag;

	Line _line;
public:
	MapBase();
	virtual ~MapBase();

	void Setup(MapBaseInfo info);
	void Release();
	void Update();
	void Render();

	D3DXVECTOR3 GetPos() { return _info.Pos; }
	std::string GetName() { return _info.Name; }
};

