#include "stdafx.h"
#include "MapView.h"

MapView::MapView()
{
}


MapView::~MapView()
{
}

void MapView::Setup()
{
	_bases.clear();

	_isOpen = false;

	//MAPVIEW_CAMERA->IsBoundPlayer = true;
	//MAPVIEW_CAMERA->IsHiddenCursor = false;
	MAPVIEW_CAMERA->SetFar(3000);

	MapBaseInfo info;

	info.IconPath = RESOURCE("Textures/Icon/base/Town.png");
	info.Pos = D3DXVECTOR3(-450, 150, 470);
	info.Name = "칼페온";

	AddBase(info);

	info.IconPath = RESOURCE("Textures/Icon/base/Town.png");
	info.Pos = D3DXVECTOR3(-22, 100, -5);
	info.Name = "하이델";

	AddBase(info);

	info.IconPath = RESOURCE("Textures/Icon/base/Load.png");
	info.Pos = D3DXVECTOR3(120, 100, 470);
	info.Name = "초보자의 사냥터";

	AddBase(info);

	info.IconPath = RESOURCE("Textures/Icon/base/Load.png");
	info.Pos = D3DXVECTOR3(-300, 100, -300);
	info.Name = "중간 사냥터";

	AddBase(info);

	info.IconPath = RESOURCE("Textures/Icon/base/Load.png");
	info.Pos = D3DXVECTOR3(540, 100, -120);
	info.Name = "마지막 사냥터";

	AddBase(info);

	info.IconPath = RESOURCE("Textures/Icon/base/cal.png");
	info.Pos = D3DXVECTOR3(-430, 350, 460);
	info.Name = "칼페온";

	AddBase(info);

	_smallCircle.Setup(100, 150);
	_smallCircle.SetAlpha(1);
	_bigCircle.Setup(100, 300);
	_bigCircle.SetAlpha(1);
	_dayCircle.Setup(100, 300);
	_dayCircle.SetAlpha(1);
	_dayCircle.pTransform.SetRotateLocal(RAD(30), RAD(60), RAD(90));

	_dayLight.SetTexture(RESOURCE("Textures/Icon/base/sun.png"));
	_nightLight.SetTexture(RESOURCE("Textures/Icon/base/moon.png"));

	SEParam param;
	param.MinRot = D3DXVECTOR3(0, RAD(45), 0);
	param.MaxRot = D3DXVECTOR3(0, RAD(45), 0);
	_effect = EFFECT_MGR->CreateEffect("mapView Point", RESOURCE_TEXTURE->GetResource(RESOURCE("Textures/figureTex.png")), 1, 1, &_pointTrans, true, &param);

	_effect->Local->SetScale(10, 500, 10);
	//_effect->Local->SetRotateLocal(RAD(90), 0, 0);
	_effect->Local->SetWorldPosition(0, 200, 0);

	_effect->Start(4, true);

	_effectP = EFFECT_MGR->CreateEffect("mapView Point Player", RESOURCE_TEXTURE->GetResource(RESOURCE("Textures/figureTex.png")), 1, 1, PLAYER_MGR->GetPlayer()->pTransform, true, &param);

	_effectP->Local->SetScale(100, 2500, 100);
	//_effectP->Local->SetRotateLocal(RAD(90), 0, 0);
	_effectP->Local->SetWorldPosition(0, 100, 0);

	_effectP->Start(4, true);
	_effectP->SetRenderingState(false);

	_pointEffect.push_back(_effect);
	_pointEffect.push_back(_effectP);
	//_effect->Local->SetScale(1000, 10000, 1000);
}

void MapView::Release()
{
	for (int i = 0; i < _bases.size(); i++)
	{
		_bases[i]->Release();
		SAFE_DELETE(_bases[i]);
	}

	_bases.clear();

	SAFE_RELEASE(_effect);
}

void MapView::Update()
{
	//토글 키
	if (KEY_MGR->IsOnceDown('M'))
	{
		SetOpenState(!IsOpen());
	}

	//맵 닫혀있으면 리턴
	if (!IsOpen()) return;

	//거점 업데이트
	for (int i = 0; i < _bases.size(); i++) _bases[i]->Update();

	//맵 이동
	D3DXVECTOR3 pos = _mapViewPoint.GetWorldPosition();

	if (KEY_MGR->IsOnceDown(VK_LBUTTON))
	{
		prev = GetMousePos();
	}

	if (KEY_MGR->IsStayDown(VK_LBUTTON))
	{
		//MAPVIEW_CAMERA->IsHiddenCursor = false;
		POINT mPt = GetMousePos();

		float x = (prev.x - mPt.x);
		float y = (prev.y - mPt.y);

		_mapViewPoint.MovePositionWorld((MAPVIEW_CAMERA->GetRight() * x) + (-MAPVIEW_CAMERA->GetDirectForward() * y));

		D3DXVECTOR3 pos1 = _mapViewPoint.GetWorldPosition();

		if (pos1.x > 500) pos1.x = 500;
		if (pos1.x < -500) pos1.x = -500;
		if (pos1.z > 500) pos1.z = 500;
		if (pos1.z < -500) pos1.z = -500;

		_mapViewPoint.SetWorldPosition(pos1);

		prev = mPt;
	}

	MAPVIEW_CAMERA->SetLook(pos);
	if (MAPVIEW_CAMERA->IsPosLerpEnd())
	{
		MAPVIEW_CAMERA->SetPos(pos + D3DXVECTOR3(-550, 750, 0));
	}

	//원들 그림
	D3DXVECTOR3 pPos = PLAYER_MGR->GetPlayer()->pTransform->GetWorldPosition();
	pPos.y = 200;

	_smallCircle.pTransform.SetWorldPosition(pPos.x, 200, pPos.z);
	_bigCircle.pTransform.SetWorldPosition(pPos.x, 200, pPos.z);
	_dayCircle.pTransform.SetWorldPosition(pPos.x, 200, pPos.z);

	//햇빛 달빛 설정
	D3DXVECTOR3 dayPos = D3DXVECTOR3(0, 0, -300), nightPos = D3DXVECTOR3(0, 0, -300);

	D3DXMATRIXA16 total, rx, ry, rz;
	D3DXMatrixRotationX(&rx, _day->GetDayLightAngle());
	D3DXMatrixRotationY(&ry, RAD(60));
	D3DXMatrixRotationZ(&rz, 0);
	total = rx * ry * rz;
	D3DXVec3TransformCoord(&dayPos, &dayPos, &total);
	_dayLight.SetPosition(pPos + dayPos);

	D3DXMatrixRotationX(&rx, _day->GetNightLightAngle());
	total = rx * ry * rz;
	D3DXVec3TransformCoord(&nightPos, &nightPos, &total);
	_nightLight.SetPosition(pPos + nightPos);

	//우클릭 포인트 이동
	if (KEY_MGR->IsOnceDown(VK_RBUTTON))
	{
		Ray ray;
		POINT pos = GetMousePos();
		MAPVIEW_CAMERA->ComputeRay(&ray, &D3DXVECTOR2(pos.x, pos.y));

		D3DXVECTOR3 pos2;
		if (TERRAIN_MGR->GetTerrain("worldMap")->IsIntersectRay(&pos2, &ray))
		{
			pos2.y = TERRAIN_MGR->GetTerrain("worldMap")->GetHeight(pos2.x, pos2.z);
			_pointTrans.SetWorldPosition(pos2.x, pos2.y, pos2.z);
			MAPVIEW_CAMERA->ShakePos(5, 10);
		}
	}

	_pointEffect.Update(0.016);
}

void MapView::Render()
{
	for (int i = 0; i < _bases.size(); i++)
	{
		_bases[i]->Render();
	}

	Figure::Begin();
	_smallCircle.Render();
	_bigCircle.Render();
	_dayCircle.Render();

	//_effect->Render();
	//_effectP->Render();

	Figure::End();

	_dayLight.RenderToSprite(SPRITE, CAMERA);
	_nightLight.RenderToSprite(SPRITE, CAMERA);

	NPC_MGR->MapRender();
	EffectRender();
}

void MapView::EffectRender()
{
	_pointEffect.Render();
}

void MapView::AddBase(MapBaseInfo & info)
{
	MapBase* base = new MapBase;

	base->Setup(info);

	_bases.push_back(base);
}

void MapView::Open()
{
	CAMERA_MGR->SetMapView();

	_mapViewPoint.SetWorldPosition(PLAYER_MGR->GetPlayer()->pTransform->GetWorldPosition());

	SPRITE_MGR->FadeInBlack(0);
	MAPVIEW_CAMERA->SetPos(_mapViewPoint.GetWorldPosition(), _mapViewPoint.GetWorldPosition() + D3DXVECTOR3(-550, 750, 0), 1);

	//_quadEffectVector.Start(0, true);
	_effectP->SetRenderingState(true);
}

void MapView::Close()
{
	CAMERA_MGR->SetPlayer();

	SPRITE_MGR->FadeInBlack(3);

	//_quadEffectVector.Stop();
	_effectP->SetRenderingState(false);
}

std::string MapView::GetNearBaseName(D3DXVECTOR3 pos)
{
	MapBase* base;
	float length = 1000;

	for (int i = 0; i < _bases.size(); i++)
	{
		float temp = D3DXVec3Length(&(pos - _bases[i]->GetPos()));
		if (length > temp)
		{
			base = _bases[i];
			length = temp;
		}
	}

	return base->GetName();
}
