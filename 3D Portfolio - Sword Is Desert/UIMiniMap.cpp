#include "stdafx.h"
#include "UIMiniMap.h"


UIMiniMap::UIMiniMap()
{
}


UIMiniMap::~UIMiniMap()
{
}

void UIMiniMap::Setup()
{
	UIWindow::Setup();
}

void UIMiniMap::Release()
{
	SAFE_RELEASE(_map);
	SAFE_RELEASE(_sight);
	SAFE_RELEASE(_monster);
	SAFE_RELEASE(_npc);
}

void UIMiniMap::Update()
{
	UIWindow::Update();

	if (KEY_MGR->IsOnceDown(VK_NUMPAD9))
	{
		_zoom += 0.1;
		if (_zoom > 3.0f) _zoom = 3.0f;
	}
	if (KEY_MGR->IsOnceDown(VK_NUMPAD6))
	{
		_zoom -= 0.1;
		if (_zoom < 0.5f) _zoom = 0.5f;
	}

	if (KEY_MGR->IsOnceDown(VK_NUMPAD8))
	{
		_alphaBlend += 10;
		if (_alphaBlend > 255) _alphaBlend = 255;
	}
	if (KEY_MGR->IsOnceDown(VK_NUMPAD5))
	{
		_alphaBlend -= 10;
		if (_alphaBlend < 50) _alphaBlend = 50;
	}
}

void UIMiniMap::Render(LPD3DXSPRITE sprite)
{
	if (_isHidden)
		return;

	D3DXMATRIXA16 matRot, matPos, matSize, matTotal;

	//플레이어 위치
	//맵 사이즈는 -768 ~ 768 총 길이 1536

	//이동
	POINT pos = GetFinalPosition();
	D3DXMatrixTranslation(&matPos, pos.x, pos.y, 0.0f);

	//줌
	D3DXMatrixScaling(&matSize, _zoom, _zoom, 1);

	//합
	D3DXMatrixIdentity(&matTotal);
	matTotal = matSize * matPos;

	float size = 300 / _zoom;

	//플레이어 위치
	D3DXVECTOR3 pPos = PLAYER_MGR->GetPlayer()->pTransform->GetWorldPosition();
	//0~1사이의 값으로 셋팅한다.
	pPos.x += 768;
	pPos.z -= 768;
	pPos /= 1536;

	//맵상의 플레이어 위치
	pPos.x *= _mapI.Width;
	pPos.z *= _mapI.Height;

	//z 반전
	pPos.z = -pPos.z;

	RECT rc;
	SetRect(&rc, pPos.x - size / 2, pPos.z - size / 2, pPos.x + size / 2, pPos.z + size / 2);


	if (_map)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		sprite->SetTransform(&matTotal);

		sprite->Draw(_map,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			D3DCOLOR_ARGB(_alphaBlend, 255, 255, 255));

		//Sprite->Draw(buttonTexture, NULL, NULL, &D3DXVECTOR3(-width2, -height2, 0.0), whitecol);

		sprite->End();
	}

	//사이트
	//줌
	D3DXMatrixScaling(&matSize, 1.3333, 1.3333, 1);
	//회전
	//회전하기전에 이동좀
	D3DXMatrixTranslation(&matPos, -100, -100, 0);
	
	D3DXVECTOR3 forward = PLAYER_CAMERA->GetForward();
	forward.y = 0;
	D3DXVec3Normalize(&forward, &forward);

	float angle = acos(D3DXVec3Dot(&D3DXVECTOR3(0, 0, 1), &forward));
	if (forward.x < 0) angle = D3DX_PI * 2 - angle;

	D3DXMatrixRotationZ(&matRot, angle);
	matRot = matPos * matRot;
	//다시 이동
	D3DXMatrixTranslation(&matPos, pos.x + 150, pos.y + 150, 0);
	//합
	D3DXMatrixIdentity(&matTotal);
	matTotal = matSize * matRot * matPos;
	SetRect(&rc, 0, 0, _sightI.Width, _sightI.Height);

	if (_sight)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		sprite->SetTransform(&matTotal);

		sprite->Draw(_sight,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			D3DCOLOR_ARGB(_alphaBlend, 255, 255, 255));

		//Sprite->Draw(buttonTexture, NULL, NULL, &D3DXVECTOR3(-width2, -height2, 0.0), whitecol);

		sprite->End();
	}

	Monster* mon = NULL;
	D3DXVECTOR3 mPos, fPos;
	for (int i = 0; i < MONSTER_MGR->_vecTotalMonster.size(); i++)
	{
		mon = MONSTER_MGR->_vecTotalMonster[i];

		//몹 위치
		mPos = mon->pTransform->GetWorldPosition();


		//0~1사이의 값으로 셋팅한다.
		mPos.x += 768;
		mPos.z -= 768;
		mPos /= 1536;

		//맵상의 몹 위치
		mPos.x *= (_mapI.Width);
		mPos.z *= (_mapI.Height);
		mPos.z = -mPos.z;

		fPos = mPos - pPos;
		fPos *= _zoom;
		//LOG_V(fPos);

		if (fPos.x < -150 || fPos.x > 150 || fPos.z < -150 || fPos.z > 150) continue;

		D3DXMatrixTranslation(&matPos, pos.x + fPos.x + 150, pos.y + fPos.z + 150, 0.0f);

		SetRect(&rc, 0, 0, _monsterI.Width, _monsterI.Height);

		if (_monster)
		{
			sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			sprite->SetTransform(&matPos);

			sprite->Draw(_monster,
				&rc,
				&D3DXVECTOR3(0, 0, 0),
				&D3DXVECTOR3(0, 0, 0),
				D3DCOLOR_ARGB(_alphaBlend, 255, 255, 255));

			//Sprite->Draw(buttonTexture, NULL, NULL, &D3DXVECTOR3(-width2, -height2, 0.0), whitecol);

			sprite->End();
		}
	}

	NPC* npc = NULL;
	D3DXVECTOR3 nPos;
	for (int i = 0; i < NPC_MGR->_data.size(); i++)
	{
		npc = NPC_MGR->_data[i];

		//몹 위치
		nPos = npc->pTransform->GetWorldPosition();

		//0~1사이의 값으로 셋팅한다.
		nPos.x += 768;
		nPos.z -= 768;
		nPos /= 1536;

		//맵상의 몹 위치
		nPos.x *= (_mapI.Width);
		nPos.z *= (_mapI.Height);
		nPos.z = -nPos.z;

		fPos = nPos - pPos;
		fPos *= _zoom;

		if (fPos.x < -150 || fPos.x > 150 || fPos.z < -150 || fPos.z > 150) continue;

		D3DXMatrixTranslation(&matPos, pos.x + fPos.x + 150, pos.y + fPos.z + 150, 0.0f);

		SetRect(&rc, 0, 0, _npcI.Width, _npcI.Height);

		if (_npc)
		{
			sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			sprite->SetTransform(&matPos);

			sprite->Draw(_npc,
				&rc,
				&D3DXVECTOR3(0, 0, 0),
				&D3DXVECTOR3(0, 0, 0),
				D3DCOLOR_ARGB(_alphaBlend, 255, 255, 255));

			//Sprite->Draw(buttonTexture, NULL, NULL, &D3DXVECTOR3(-width2, -height2, 0.0), whitecol);

			sprite->End();
		}
	}

	UIWindow::Render(sprite);
}

void UIMiniMap::SetMapTexture(std::string map, std::string s, std::string m, std::string n)
{
	SAFE_RELEASE(_map);
	SAFE_RELEASE(_sight);
	SAFE_RELEASE(_monster);
	SAFE_RELEASE(_npc);

	_map = TEXTURE_MGR->GetTexture(map, &_mapI);
	_sight = TEXTURE_MGR->GetTexture(s, &_sightI);
	_monster = TEXTURE_MGR->GetTexture(m, &_monsterI);
	_npc = TEXTURE_MGR->GetTexture(n, &_npcI);

	SAFE_ADDREF(_map);
	SAFE_ADDREF(_sight);
	SAFE_ADDREF(_monster);
	SAFE_ADDREF(_npc);
}
