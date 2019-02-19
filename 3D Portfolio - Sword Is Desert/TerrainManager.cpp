#include "stdafx.h"
#include "TerrainManager.h"
#include "cTerrain.h"
#include "Pet.h"


TerrainManager::TerrainManager()
{
}


TerrainManager::~TerrainManager()
{
}

HRESULT TerrainManager::Setup()
{
	_mTerrain.clear();

	return S_OK;
}

void TerrainManager::Release()
{
	for (auto t : _mTerrain)
	{
		t.second->Release();
		SAFE_DELETE(t.second);
	}

	_mTerrain.clear();
}

void TerrainManager::Render(std::string name, Camera* cam)
{
	cTerrain* t = GetTerrain(name);

	if (t) t->Render(cam);
}

float TerrainManager::GetHeight(std::string name, float x, float z)
{
	cTerrain* t = GetTerrain(name);

	if (t == NULL) return 0.0f;

	return t->GetHeight(x, z);
}

float TerrainManager::GetHeight(std::string name, OUT Transform * trans)
{
	D3DXVECTOR3 temp = trans->GetWorldPosition();
	float result = GetHeight(name, temp.x, temp.z);

	temp.y = result;

	trans->SetWorldPosition(temp);

	return result;
}

float TerrainManager::GetHeight(std::string name, OUT D3DXVECTOR3 * pos)
{
	float result = GetHeight(name, pos->x, pos->z);

	pos->y = result;

	return result;
}

void TerrainManager::SetHeight(std::string name, Character * character)
{
	D3DXVECTOR3 pPos = character->pTransform->GetWorldPosition();
	character->SetHeight(GetTerrain(name)->GetHeight(pPos.x, pPos.z));
}

void TerrainManager::SetHeight(std::string name, Pet * character)
{
	D3DXVECTOR3 pPos = character->pTransform->GetWorldPosition();
	character->SetHeight(GetTerrain(name)->GetHeight(pPos.x, pPos.z));
}

void TerrainManager::SetHeight(std::string name, const D3DXVECTOR3 * pos, OUT float * height)
{
	*height = GetTerrain(name)->GetHeight(pos->x, pos->z);
}


cTerrain * TerrainManager::GetTerrain(std::string name)
{
	auto iter = _mTerrain.find(name);

	if (iter == _mTerrain.end())
	{
		return NULL;
	}

	return iter->second;
}

void TerrainManager::Create(std::string name, const char * szHeightMapName, const char * szTile_0, const char * szTile_1, const char * szTile_2, const char * szTile_3, const char * szTileSplat, float cellSize, float heightScale, int smoothLevel, int tileNum)
{
	if (GetTerrain(name)) return;

	cTerrain* newTerrain = new cTerrain;

	newTerrain->Init(szHeightMapName, szTile_0, szTile_1, szTile_2, szTile_3, szTileSplat, cellSize, heightScale, smoothLevel, tileNum);

	_mTerrain.insert(std::make_pair(name, newTerrain));
}
