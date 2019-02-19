#include "stdafx.h"
#include "Day.h"

Day::Day()
{
	//환경 Effect 물어 놓는다.
	this->evironmentEffect = RESOURCE_FX->GetResource("../Resources/Shaders/DaliyCUBE.fx");
}


Day::~Day()
{
}

void Day::Setup()
{
	D3DXCreateCubeTextureFromFile(DEVICE, "../Resources/TextureCUBE/desert.dds", &evironmentTexture);
	D3DXCreateCubeTextureFromFile(DEVICE, "../Resources/TextureCUBE/rainCube.dds", &evironmentRainTexture);
	D3DXCreateCubeTextureFromFile(DEVICE, "../Resources/TextureCUBE/CubeMap_Night.dds", &evironmentTexture2);

	D3DXCreateSphere(DEVICE, 200.0f, 20, 20, &evironmemtSphereMesh, NULL);
	
	_currentTime = 0;

	_flag = 1;

	//라이트 셋팅
	_dayLight = new cLight_Direction();
	_dayLight->Color = D3DXCOLOR(1, 1, 1, 1);
	_dayLight->Intensity = 1;
	_dayLight->pTransform->SetRotateLocal(0, RAD(60), 0);

	_nightLight = new cLight_Direction();
	_nightLight->Color = D3DXCOLOR(0.5, 0.5, 0.8, 1);
	_nightLight->Intensity = 1;
	_nightLight->pTransform->SetRotateLocal(0, RAD(60), 0);

	_lights.push_back(_dayLight);
	_lights.push_back(_nightLight);

	//비
	VEC_COLOR c;
	c.push_back(0xffffffff); c.push_back(0xffffffff);
	VEC_SCALE s;
	s.push_back(1); s.push_back(1);

	_rain = new cPartcleEmitter;
	_rain->SetActive(true);
	_rain->Init(1000, 500, 2, 2,
		D3DXVECTOR3(-1, -100, 0), D3DXVECTOR3(-1, -100, 0),
		D3DXVECTOR3(-1, -50, 0), D3DXVECTOR3(-1, -50, 0),
		c, s, 0.5, 1, RESOURCE_TEXTURE->GetResource(RESOURCE("Textures/rain.png")));
	_rain->EmissionType = BOX;
	_rain->MinEmissionRangeX = -25;		//X 생성범위
	_rain->MaxEmissionRangeX = 25;
	_rain->MinEmissionRangeZ = -25;		//Z 생성범위
	_rain->MaxEmissionRangeZ = 25;

	_rainChange = TIME_MGR->CreateTimer();
	_thunderTimer = TIME_MGR->CreateTimer();
	_isRain = false;

	//선라이트
	SEParam param;
	param.MaxRot.y = RAD(1);
	param.MinRot.y = RAD(10);
	param.Colors.push_back(D3DXCOLOR(1, 1, 0.8f, 1));
	LPDIRECT3DTEXTURE9 tex = RESOURCE_TEXTURE->GetResource(RESOURCE("Textures/sunlight3.png"));

	for (int i = 0; i < 3; i++)
	{
		_sunlight[i] = new SpriteEffect;
		_sunlight[i]->Setup(&param);
		_sunlight[i]->SetTexture(tex);
		_sunlight[i]->SetUV(1, 1);
		_sunlight[i]->Local->SetLocalPosition({ 0, 500, 0 });
		_sunlight[i]->Local->SetScale({ 6, 18, 18 });
		_sunlight[i]->Local->SetRotateLocal(RAD(90), 0, 0);
			/*
			EFFECT_MGR->CreateEffect(
			"", tex, 1, 1, NULL, { 0, 500, 0 }, { RAD(90), 0, 0 }, { 150, 150, 150 }, false, &param);
			*/
		_sunlight[i]->Start(INT_MAX, true);
		_sunlight[i]->SetLookCameraState(true);
	}
}


void Day::Release()
{
	SAFE_RELEASE(evironmentTexture);
	SAFE_RELEASE(evironmentTexture2);
	SAFE_RELEASE(evironmentEffect);
	SAFE_RELEASE(evironmemtSphereMesh);

	SAFE_RELEASE(_rain);
}

void Day::Update()
{
	float timeDelta = 0.016;

	if (GAMECONFIG->GetIsTimeFast())
		timeDelta = TIME_MGR->GetFrameDeltaSec() * 20;
	else
		timeDelta = TIME_MGR->GetFrameDeltaSec();

	//밤이면 시간 두배
	if (_flag < 0) timeDelta *= 2;

	//시간 빼줌
	_currentTime += timeDelta;

	//낮과 밤 전환
	if (_currentTime > _daliyTime / 2)
	{
		_currentTime = 0;
		_flag *= -1;
	}
	
	float angle = RAD(((_currentTime) / _daliyTime) * 360);

	if (_flag > 0)
	{
		_alpha = Clamp01(_currentTime / _changeTime);
	}
	else
	{
		_alpha = 1 - Clamp01(_currentTime / _changeTime);

		angle += RAD(180);
	}

	_dayLight->Intensity = _alpha;
	_nightLight->Intensity = 1 - _alpha;
	
	_dayLight->pTransform->SetRotateWorld(angle, RAD(60), 0);
	_nightLight->pTransform->SetRotateWorld(angle - RAD(180), RAD(60), 0);

	//비 위치
	D3DXVECTOR3 camPos = CAMERA->GetWorldPosition();
	_rain->Update(timeDelta);
	_rain->pTransform->SetWorldPosition(camPos.x, camPos.y + 25, camPos.z);

	//비 안올때
	if (!_isRain)
	{
		//임시로 키로 누름
		if (KEY_MGR->IsOnceDown(VK_F11)) //RandomFloatRange(0, 100) < 0.1 //천분에 1 확률
		{
			_isRain = true;
			_rainChange->Start(10);
			_rain->StartEmission();
			_dayLight->Color = D3DXCOLOR(0.2, 0.2, 0.4, 1);

			SOUND_MGR->play("weather rain", 0.5);
		}
	}
	else //비올때
	{
		if (KEY_MGR->IsOnceDown(VK_F11)) //RandomFloatRange(0, 100) < 0.1 //천분에 1 확률
		{
			_isRain = false;
			_rainChange->Start(10);
			_rain->StopEmission();
			_dayLight->Color = D3DXCOLOR(1, 1, 1, 1);

			SOUND_MGR->stop("weather rain");
			SOUND_MGR->stop("weather thunder");
		}

		if (KEY_MGR->IsOnceDown(VK_F12))
		{
			_thunderTimer->Start(30);
		}

		if (_thunderTimer->Check(0, 1)) _dayLight->Color = D3DXCOLOR(1, 1, 1, 1);
		else if (_thunderTimer->Check(1, 1.05)) _dayLight->Color = D3DXCOLOR(0.2, 0.2, 0.4, 1);
		else if (_thunderTimer->Check(2, 1.2)) _dayLight->Color = D3DXCOLOR(1, 1, 1, 1);
		else if (_thunderTimer->Check(3, 1.25)) _dayLight->Color = D3DXCOLOR(0.2, 0.2, 0.4, 1);
		else if (_thunderTimer->Check(4, 3)) SOUND_MGR->play("weather thunder", 0.7);
		else if (_thunderTimer->End()) SOUND_MGR->stop("weather thunder");
	}

	//선라이트 셋팅
	D3DXVECTOR3 pos = PLAYER_MGR->GetPlayer()->pTransform->GetWorldPosition();
	D3DXVECTOR3 dayPos = D3DXVECTOR3(0, 0, -500);

	D3DXMATRIXA16 rx, ry;
	D3DXMatrixRotationX(&rx, GetDayLightAngle());
	D3DXMatrixRotationY(&ry, RAD(60));
	D3DXVec3TransformCoord(&dayPos, &dayPos, &(rx * ry));

	for (int i = 0; i < 3; i++)
	{
		_sunlight[i]->Update(timeDelta);
		_sunlight[i]->Local->SetLocalPosition(pos + dayPos);
	}
}

void Day::Render()
{
	cXMesh_Skinned::SetBaseLight((cLight_Direction*)_dayLight);

	//데일라이트
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->_lights.size(); i++)
		matLights[i] = this->_lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	//if (cXMesh_Skinned::sSkinnedMeshEffect)
	//{
	if (cXMesh_Skinned::sSkinnedMeshEffect)
	{
		cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
		cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->_lights.size());
	}
	//}
	if (cXMesh_Static::sStaticMeshEffect)
	{
		cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 10);
		cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->_lights.size());
	}

	if (this->evironmentTexture == NULL || this->evironmentTexture2 == NULL || this->evironmentEffect == NULL)
		return;

	//알파 구하기
	//낮일 경우 알파값은 1, 밤일경우 0
	//하루 시간의 반에서 changetime 만큼 만을 경우 1 에서 0, 0에서 1로 보간 된다.
	this->evironmentEffect->SetFloat("Day_Alpha", _alpha);

	if (!_isRain)
	{
		this->evironmentEffect->SetTexture("Day_Texture", this->evironmentTexture);
	}
	else
	{
		this->evironmentEffect->SetTexture("Day_Texture", this->evironmentRainTexture);
	}
	this->evironmentEffect->SetTexture("Night_Texture", this->evironmentTexture2);

	D3DXMATRIXA16 matWorld; //= CAMERA->GetFinalMatrix();
	D3DXMatrixTranslation(&matWorld, CAMERA->GetWorldPosition().x, CAMERA->GetWorldPosition().y, CAMERA->GetWorldPosition().z);
	D3DXMATRIXA16 matViewProj = CAMERA->GetViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	this->evironmentEffect->SetMatrix("matWVP", &matWVP);
	this->evironmentEffect->SetMatrix("matWorld", &matWorld);

	UINT numPass;
	this->evironmentEffect->Begin(&numPass, 0);		//셰이더로 그린다는 것을 알리고 pass 수를 얻는다.

	for (UINT i = 0; i < numPass; i++)	//공정 수대로 돈다.
	{
		this->evironmentEffect->BeginPass(i);			//i번째 공정시작
		this->evironmemtSphereMesh->DrawSubset(0);
		this->evironmentEffect->EndPass();				//i번째 공정끝
	}
	this->evironmentEffect->End();						//셰이더로 그리기 끝
}

void Day::SunlightRender()
{
	Figure::Begin();
	for (int i = 0; i < 3; i++)
	{
		_sunlight[i]->Render();
	}
	Figure::End();
}

