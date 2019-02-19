#pragma once
#include "Singleton.h"

class cTerrain;
class Pet;

class TerrainManager : public Singleton < TerrainManager >
{
private:
	std::map<std::string, cTerrain*> _mTerrain;
public:
	TerrainManager();
	virtual ~TerrainManager();

	HRESULT Setup();
	void Release();

	void Render(std::string name, Camera* cam);
	
	float GetHeight(std::string name, float x, float z);
	float GetHeight(std::string name, OUT Transform* trans);
	float GetHeight(std::string name, OUT D3DXVECTOR3* pos);
	void SetHeight(std::string name, Character* character);
	void SetHeight(std::string name, Pet* character);
	void SetHeight(std::string name, const D3DXVECTOR3* pos, OUT float* height);

	cTerrain* GetTerrain(std::string name);
	void Create(
		std::string name,
		const char* szHeightMapName,		//높이맵 이름
		const char* szTile_0,				// 타일 이미지 0
		const char* szTile_1,				// 타일 이미지 1
		const char* szTile_2,				// 타일 이미지 2
		const char* szTile_3,				// 타일 이미지 3
		const char* szTileSplat,			// 타일 이미지 컨트롤 ( RGB Black )
		float cellSize,						// 하나의 셀크기
		float heightScale,					// 높이 스케일
		int smoothLevel,					// 스무싱 처리 횟수 
		int tileNum							// 타일링 갯수
		);
};

#define TERRAIN_MGR TerrainManager::GetInstance()