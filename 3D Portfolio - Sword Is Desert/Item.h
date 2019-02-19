#pragma once
#include "cBaseObject.h"
#include "Character.h"
#include "SocketObject.h"

#define EQUIPTYPE_LENGTH 12

enum ITEM_TYPE
{
	IT_SWORD,
	IT_SHEILD,
	IT_BOW,
	IT_BODY,
	IT_HEAD,
	IT_GLOVE,
	IT_SHOES,
	IT_RING,
	IT_NECKLESS,
	IT_EARING,
	IT_WAIST,
	IT_NONE,
	//equip type length = 7

	IT_POTION
};

// 아이템 등급
enum ITEM_RATING
{	
	ITR_COMMON,
	ITR_UNCOMMON,
	ITR_RARE,	
	ITR_EPIC
};

typedef struct tagItemInfo
{
	//능력치
	float MaxHp = 0;
	float MaxMp = 0;
	UINT Atk = 0;
	UINT Def = 0;	
	//아이템 정보
	std::string Name;
	char* Disc;//설명
	

	ITEM_TYPE Type;
	ITEM_RATING RATING;
	UINT Price;

	std::string IconPath;
	std::string ResourcePath;
	bool isRender = true;

	D3DXMATRIXA16 Matrix = D3DXMATRIXA16(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	
	tagItemInfo() {}
	tagItemInfo(
		std::string name, char* disc, std::string iconPath, std::string resourcePath, ITEM_TYPE type = ITEM_TYPE(0), ITEM_RATING rating = ITEM_RATING(0),
		UINT price = 0, float maxHp = 0, float maxMp = 0, UINT atk = 0, UINT def = 0, bool rendering = false
		)
	{
		Name = name;
		Disc = disc;
		IconPath = iconPath;
		ResourcePath = resourcePath;
		Type = type;
		RATING = rating;
		Price = price;
		MaxHp = maxHp;
		MaxMp = maxMp;
		Atk = atk;
		Def = def;
		isRender = rendering;
		
		D3DXMatrixIdentity(&Matrix);
	}
public:
	void InitMatrix()
	{
		D3DXMatrixIdentity(&Matrix);
	}
	void SetMatrix(float sx = 1.0f, float sy = 1.0f, float sz = 1.0f, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f, float px = 0.0f, float py = 0.0f, float pz = 0.0f)
	{
		InitMatrix();

		D3DXMATRIXA16 scale, rot, pos;

		D3DXMatrixScaling(&scale, sx, sy, sz);
		D3DXMatrixRotationYawPitchRoll(&rot, rx, ry, rz);
		D3DXMatrixTranslation(&pos, px, py, pz);

		Matrix = scale * rot * pos;
	}
	
	
} ItemInfo;

class Item : public cBaseObject, public SocketObject
{
private:
	//ITEM_TYPE _type;
	ItemInfo _info;

public:
	Item();
	virtual ~Item();

	void Setup();
	void Setup(ItemInfo &info);
	void Render();

	cXMesh_Skinned* GetMesh() { return (cXMesh_Skinned*)pMesh; }
	ItemInfo GetInfo() { return _info; }
	ITEM_TYPE GetType() { return _info.Type; }
	void SetInfo(tagItemInfo &info){
		_info = info;
	}

	void RemoveIcon();
};

