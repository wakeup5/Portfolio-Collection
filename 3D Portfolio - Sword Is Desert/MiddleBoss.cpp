#include "stdafx.h"
#include "MiddleBoss.h"
#include "MonsterManager.h"

MiddleBoss::MiddleBoss()
{
	_attackNum = 0;
	SetRandomSeed();
}


MiddleBoss::~MiddleBoss()
{
}

void MiddleBoss::Setup(const char* path, RECT rect, CharacterInfo &info)
{
	Monster::Setup(path, rect, info);

	animation->Play("NSTAND");
	
	_attackBound1.SetBound(&D3DXVECTOR3(0, 0, -8), &D3DXVECTOR3(2, 6, 15));
	_attackTrans1 = pTransform;

	_hpProgress.SetPosition(0, 30)->SetSize(100, 20);
	_hpProgress.SetBarTexture(RESOURCE("Textures/Bar/hp_bar.png"), RESOURCE("Textures/Bar/hp_back.png"));
	_hpProgress.SetMax(_charInfo.MaxHp);

	_waitTime = 1.0f;

	isOrcSpawn = false;

	_atkTimer = TIME_MGR->CreateTimer("MBossAtk01_Timer");

	_Dust = new Orc_Dust;
	_Dust->Setup();
	_Dust->SetActive(true);

	_Poison = new OrcPoisonCloud;
	_Poison->Setup();
	_Poison->SetActive(true);
	animation->AddBoneTransform("Bip01_Head", _Poison->pTransform);
	

	LPDIRECT3DTEXTURE9 tex3 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/RedRing.png"));

	SEParam Param3;
	Param3.MinRot = D3DXVECTOR3(0, 0, 0);
	Param3.MaxRot = D3DXVECTOR3(0, 0, 0);
	Param3.MinSpeed = D3DXVECTOR3(0, 0, 0);
	Param3.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param3.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param3.Scales.push_back(D3DXVECTOR3(4, 4, 4));

	_Atk01FX.push_back(EFFECT_MGR->CreateEffect("Orc_Atk01", tex3, 7, 1, _Dust->pTransform,
		D3DXVECTOR3(0, 0, 0), //이동
		D3DXVECTOR3(RAD(90), 0, 0),  //회전
		D3DXVECTOR3(1.5, 1.5, 1.5), false, &Param3)->SetRender(true)); //크기));



	LPDIRECT3DTEXTURE9 tex4 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/outside.jpg"));

	SEParam Param4;
	Param4.MinRot = D3DXVECTOR3(0, 0, 0);
	Param4.MaxRot = D3DXVECTOR3(0, 0, 0);
	Param4.MinSpeed = D3DXVECTOR3(0, 0, 0);
	Param4.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param3.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param4.Scales.push_back(D3DXVECTOR3(4, 4, 4));
	Param4.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));

	_Atk02FX.push_back(EFFECT_MGR->CreateEffect("Orc_Atk02", tex4, 1, 1, pTransform,
		D3DXVECTOR3(0, 0, 0), //이동
		D3DXVECTOR3(RAD(90), 0, 0),  //회전
		D3DXVECTOR3(1.5, 1.5, 1.5), false, &Param4)->SetRender(true)); //크기));


	LPDIRECT3DTEXTURE9 tex5 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/EarthQuake.png"));

	SEParam Param5;
	Param5.MinRot = D3DXVECTOR3(0, 0, 0);
	Param5.MaxRot = D3DXVECTOR3(0, 0, 0);
	Param5.MinSpeed = D3DXVECTOR3(0, 0, 0);
	Param5.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param5.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param5.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param5.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	Param5.Scales.push_back(D3DXVECTOR3(4, 4, 4));
	Param5.Scales.push_back(D3DXVECTOR3(4, 4, 4));

	_Atk03FX.push_back(EFFECT_MGR->CreateEffect("Orc_Atk03", tex5, 1, 1, pTransform,
		D3DXVECTOR3(0, 0, 0), //이동
		D3DXVECTOR3(RAD(90), 0, 0),  //회전
		D3DXVECTOR3(1.5, 1.5, 1.5), false, &Param5)); //크기));


	LPDIRECT3DTEXTURE9 tex6 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/FireRing.png"));

	SEParam Param6;
	Param6.MinRot = D3DXVECTOR3(0, 0, 0);
	Param6.MaxRot = D3DXVECTOR3(0, 0, 0);
	Param6.MinSpeed = D3DXVECTOR3(0, 0, 0);
	Param6.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param6.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param6.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param6.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	Param6.Scales.push_back(D3DXVECTOR3(0, 0, 0));
	Param6.Scales.push_back(D3DXVECTOR3(5, 5, 5));

	_Atk04FX.push_back(EFFECT_MGR->CreateEffect("Orc_Atk04", tex6, 1, 1, pTransform,
		D3DXVECTOR3(0, 0.7, 0), //이동
		D3DXVECTOR3(RAD(90), 0, 0),  //회전
		D3DXVECTOR3(1.5, 1.5, 1.5), false, &Param6)); //크기));







	LPDIRECT3DTEXTURE9 tex11 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/BowEffect02.png"));

	SEParam Param17;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage.push_back(EFFECT_MGR->CreateEffect("", tex11, 6, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(3, 3, 3), false, &Param17)->SetLookCam(true)); //크기));


	//fire
	LPDIRECT3DTEXTURE9 tex13 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/explosion.png"));

	SEParam Param12;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex13, 7, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param12)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex14 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/hit02.png"));

	SEParam Param13;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param13.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param13.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex14, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param13)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex15 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/FireRing.png"));

	SEParam Param14;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param14.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param14.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param14.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param14.Scales.push_back(D3DXVECTOR3(3, 3, 3));

	_MonsterDamage2.push_back(EFFECT_MGR->CreateEffect("", tex15, 1, 1, pTransform,
		D3DXVECTOR3(0, 1, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param14)->SetLookCam(true)); //크기));




	//ice
	LPDIRECT3DTEXTURE9 tex16 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Ice.png"));

	SEParam Param15;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex16, 5, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param15)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex17 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/HitEffect.png"));

	SEParam Param16;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param16.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param16.Colors.push_back(D3DXCOLOR(0, 0, 0, 0));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex17, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param16)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex18 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/outside.png"));

	SEParam Param18;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param18.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param18.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param18.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param18.Scales.push_back(D3DXVECTOR3(3, 3, 3));

	_MonsterDamage3.push_back(EFFECT_MGR->CreateEffect("", tex18, 1, 1, pTransform,
		D3DXVECTOR3(0, 1, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param18)->SetLookCam(true)); //크기));



	//Thunder
	LPDIRECT3DTEXTURE9 tex19 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Thunder.png"));

	SEParam Param20;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param7.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param7.Scales.push_back(D3DXVECTOR3(1, 1, 1));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex19, 6, 1, pTransform,
		D3DXVECTOR3(0, 1.5, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1.8, 1.8, 1.8), false, &Param20)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex20 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Lightning.png"));

	SEParam Param21;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param21.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param21.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	//Param3.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	//Param3.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex20, 25, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 2, 1), false, &Param21)->SetLookCam(true)); //크기));


	LPDIRECT3DTEXTURE9 tex21 = RESOURCE_TEXTURE->GetResource(RESOURCE("FX/Test/Sword2.png"));

	SEParam Param22;
	//Param7.MinRot = D3DXVECTOR3(0, 0, 0);
	//Param7.MaxRot = D3DXVECTOR3(0, 0, 0);
	//Param2.MinSpeed = D3DXVECTOR3(0, 0, 0);
	//Param2.MaxSpeed = D3DXVECTOR3(0, 0, 0);
	Param22.Colors.push_back(D3DXCOLOR(1, 1, 1, 1));
	Param22.Colors.push_back(D3DXCOLOR(0.5, 0.5, 0.5, 0.5));
	Param22.Scales.push_back(D3DXVECTOR3(0.1, 0.1, 0.1));
	Param22.Scales.push_back(D3DXVECTOR3(2, 2, 2));

	_MonsterDamage4.push_back(EFFECT_MGR->CreateEffect("", tex21, 1, 1, pTransform,
		D3DXVECTOR3(0, 2, 0), //이동
		D3DXVECTOR3(0, 0, 0),  //회전
		D3DXVECTOR3(1, 1, 1), false, &Param22)->SetLookCam(true)); //크기));
	
}

void MiddleBoss::BaseObjectUpdate(float timeDelta)
{
	D3DXVECTOR3 selfPos = pTransform->GetWorldPosition();

	this->pTransform->SetScale(2, 2, 2);
	_Dust->Update(timeDelta);
	_Atk01FX.Update(timeDelta);
	_Poison->Update(timeDelta);
	_Atk02FX.Update(timeDelta);
	_Atk03FX.Update(timeDelta);
	_Atk04FX.Update(timeDelta);


	_MonsterDamage.Update(timeDelta);
	_MonsterDamage2.Update(timeDelta);
	_MonsterDamage3.Update(timeDelta);
	_MonsterDamage4.Update(timeDelta);

	/*
	if (isOrcSpawn)
	{
		for (int i = 0; i < 5; i++)
		{
			Mons[i]->Update(timeDelta);
		}
	}
	*/

	if (!IsState(Character::CS_DOWN))
	{
		if (GetCharacterInfo().Hp <= 0) Down();

		//네임태그 셋
		D3DXVECTOR3 TagPos = selfPos;
		TagPos.y += 3;
		_tag.SetPosition(TagPos);
		_tag.Look(CAMERA->GetWorldPosition(), D3DXVECTOR3(0, 1, 0));

		_hpProgress.SetCurrent(_charInfo.Hp);
	}
		
	Monster::BaseObjectUpdate(timeDelta);
}

void MiddleBoss::BaseObjectRender()
{
	this->animation->Render(this->pTransform);
	_Dust->Render();
	_Atk01FX.Render();
	_Poison->Render();
	_Atk02FX.Render();
	_Atk03FX.Render();
	_Atk04FX.Render();


	_MonsterDamage.Render();
	_MonsterDamage2.Render();
	_MonsterDamage3.Render();
	_MonsterDamage4.Render();
	//_attackBound1.RenderGizmo(this->pTransform);
	//animation->RenderBoneName(CAMERA, this->pTransform);
	//네임태그용
	_tagTexture.Begin(); //네임태그 텍스쳐로 렌더 타겟 변경 후 그림

	_hpProgress.Render(SPRITE);
	RECT rc = { 0, 0, 100, 30 };
	LPD3DXFONT font = DXFONT_MGR->GetStyle("");
	font->DrawText(NULL, _name.c_str(), _name.size(), &rc, DT_CENTER | DT_TOP | DT_WORDBREAK, 0xffffffff);

	_tagTexture.End(); //렌더 타겟 원상태로 복구

	cBoundBox atkBox;
	atkBox.SetBound(&D3DXVECTOR3(0, 1.5f, 0), &D3DXVECTOR3(1, 1, _charInfo.AtkScale * 2));
	atkBox.RenderGizmo(pTransform);
	
	/*
	if (isOrcSpawn)
	{
		for (int i = 0; i < 5; i++)
		{
			Mons[i]->Render();
		}
	}

	*/
	//플레이어 거리를 구하고 거리에 따라 알파값 적용
	float distance = D3DXVec3Length(&(PlayerTrans->GetWorldPosition() - pTransform->GetWorldPosition()));
	float alpha = 1 - MyUtil::Clamp01((distance - 20.0f) / 20.0f);
	if (alpha > 0.01)
		_tag.RenderToSprite(SPRITE, CAMERA, D3DXCOLOR(1, 1, 1, alpha));


	//LOG_MGR->AddLog("%f", _atkRunTime);
}

void MiddleBoss::AIBattleUpdate(float timeDelta)
{

	if (IsState(Character::CS_ATTACK))
	{
		AttackUpdate();//
	}


	if (!IsState(Character::CS_DOWN))
	{
		if (IsState(Character::CS_ATTACK))
		{
				
			if (!animation->IsPlay())
			{
				SetState(Character::CS_STAND);
				_AttackTime = 2.5f;
			}
			
	
		}
		else
		{
			_DestPos = PlayerTrans->GetWorldPosition();
			LookPosition(_DestPos);

			if (IsDestPos(4.0f))
			{
				_waitTime = 1.0f;
				SetState(Character::CS_NONE);
				ActionState("BSTAND", Character::CS_STAND, 0, false);

				if (_AttackTime <= 0)
				{
					if (RandomFloatRange(0, 100) < 50)
					{
				
					
							int num;
							num = RandomIntRange(0, 5);
							switch (num)
							{
							case 0:
								ActionState("ATK03", Character::CS_ATTACK, 0, true);
								_attackNum = 3;
								break;
							case 1:
								ActionState("ATK04", Character::CS_ATTACK, 0, true);
								_attackNum = 4;
								break;
							case 2:
								ActionState("ATK05", Character::CS_ATTACK, 0, true);
								_attackNum = 5;
								break;
							case 3:
								ActionState("ATK07", Character::CS_ATTACK, 0, true);
								_attackNum = 7;
								break;
								break;
							case 4:
								ActionState("ATK02", Character::CS_ATTACK, 0, true);
								_attackNum = 2;
								break;
							}
												
					}

					else
					{
						ActionState("ATK01", Character::CS_ATTACK, 0.2, true);
					
						_attackNum = 1;
					}
				
					_atkTimer->Start(INT_MAX);
					
				}
			}
			else if (_waitTime <= 0)
			{
				ActionState("RUN", Character::CS_RUN, 0, false);
				MoveDestPosition(_charInfo.WalkSpeed * timeDelta);
			}
			else
			{
				_waitTime -= timeDelta;
				SetState(Character::CS_NONE);
				ActionState("BSTAND", Character::CS_STAND, 0, false);
			
			}

			if (!IsPosInDistance(_PrevPos, 20.0f))
			{
				_DestPos = _PrevPos;
				_battleState = Character::CBS_BACK;
			}

			if (_AttackTime > 0)
			{
				_AttackTime = _AttackTime - timeDelta;
			}
		}
		
	}


}

bool MiddleBoss::GetHitTransform(OUT Transform ** transArr, OUT int * arrLength)
{
	ZeroMemory(transArr, sizeof(Transform*) * 4);
	for (int i = 0; i < 4; i++)
	{
		transArr[i] = new Transform;
	}

	animation->AddBoneTransform("Bip01_Spine", transArr[0]);
	//animation->AddBoneTransform("Bip01_L_Foot", transArr[1]);

	if (arrLength) *arrLength = 4;

	return true;
}

void MiddleBoss::AINormalUpdate(float timeDelta)
{
	MoveUpdate(timeDelta);

	if (IsPosInDistance(PlayerTrans->GetWorldPosition(), 6))
	{
		_battleState = Character::CBS_BATTLE;
		_PrevPos = pTransform->GetWorldPosition();
	}
}

void MiddleBoss::MoveUpdate(float timeDelta)
{

}

void MiddleBoss::Alive()
{

}

void MiddleBoss::CreateOrc()
{
	
	RECT temp = _Area;
		
	for (int i = 0; i < 5; i++)
	{
		Mons[i] = new Monster;
		MonsterInfo info = MonsterInfo("../Resources/Meshes/Monster/orc/orc.X", "Orc", 1, temp, 500, 0, 10, 5, 0.5, 5, 6, 10);
		Mons[i]->Setup(info.ResourcePath, info.Area, info);
		Mons[i]->SetActive(true);
		Mons[i]->SetName(info.Name);
		
		MONSTER_MGR->_vecTotalMonster.push_back(Mons[i]);
	}
	isOrcSpawn = true;
}

void MiddleBoss::SetHeight(float height)
{
	D3DXVECTOR3 temp = pTransform->GetWorldPosition();
	pTransform->SetWorldPosition(temp.x, height, temp.z);
	
	if (!isOrcSpawn) return;
	for (int i = 0; i < 5; i++)
	{
		TERRAIN_MGR->SetHeight("worldMap",Mons[i]);
	}
}

void MiddleBoss::AttackUpdate()
{

	Player* player = PLAYER_MGR->GetPlayer();
	cBoundBox* pBox = &player->BoundBox;

	if (!animation->IsPlay())
	{
		_attackNum = 0;
	}

	switch (_attackNum)
	{
		case 1:
		{
			LOG_MGR->AddLog("1번 케이스");
			if (_atkTimer->Check(1, 0.65))
			{
				SOUND_MGR->play("orc_voice", 0.2f);
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 1.0f);
				}
				_Atk01FX.Start(0.5,false);
				_Dust->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
				_Dust->pTransform->MovePositionLocal(-pTransform->GetForward()*5);
				_Dust->StartEmission();
			
			}
			if (_atkTimer->Check(2, 0.90))
			{
				_Dust->StopEmission();
			}
			break;
		}
		case 2:
		{
			LOG_MGR->AddLog("2번 케이스");
			SOUND_MGR->isPlay("orc_voice");
			if (_atkTimer->Check(2, 0.65))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 1.0f);
				}
			}
			break;
		}
		case 3:
		{
			LOG_MGR->AddLog("3번 케이스");
			SOUND_MGR->isPlay("orc_voice");
			if (isOrcSpawn) return;

			CreateOrc();

			break;
		}
		case 4:
		{
			LOG_MGR->AddLog("4번 케이스");
			SOUND_MGR->isPlay("orc_voice");
			if (_atkTimer->Check(12, 0.5))
			{
				_Atk02FX.Start(0.4, true);
			}
			if (_atkTimer->Check(13, 3.0))
			{
				_Atk02FX.Stop();
			}
			if (_atkTimer->Check(3, 3.3))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 0.0f);
				
				}
				_Dust->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
				_Dust->Burst(500, 3, 5, 0.6, 0.7);
				_Atk03FX.Start(0.5, false);

			}
			if (_atkTimer->Check(4, 4.0))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 1.0f);
				
				}
				_Dust->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
				_Dust->Burst(500, 6, 7, 1.0, 1.5);
				_Atk03FX.Start(1.5, false);
				_Atk04FX.Start(0.4, false);
				
			}
			break;
		}
		case 5:
		{
			LOG_MGR->AddLog("5번 케이스");
			SOUND_MGR->isPlay("orc_voice");
			_Poison->StartEmission();
			if (_atkTimer->Check(3, 3.7))
			{
				SOUND_MGR->play("poison", 0.2f);
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 0.0f);
				}
				
			}
			if (_atkTimer->Check(4, 4.1))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 0.0f);
				}
			}
			if (_atkTimer->Check(5, 4.5))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 0.0f);
				}
				
			}
			if (_atkTimer->Check(6, 4.8))
			{
				_Poison->StopEmission();
			}
			
			break;
		}
		case 7:
		{
			LOG_MGR->AddLog("7번 케이스");
			SOUND_MGR->isPlay("orc_voice");
			if (_atkTimer->Check(6, 1.5))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(100, 0.0f);
				}
				_Atk01FX.Start(0.5, false);
				_Dust->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
				_Dust->pTransform->MovePositionLocal(-pTransform->GetForward() * 5);
				_Dust->StartEmission();
			}
			if (_atkTimer->Check(7, 1.8))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(200, 0.0f);
				}
				_Atk01FX.Start(0.5, false);
				_Dust->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
				_Dust->pTransform->MovePositionLocal(-pTransform->GetForward() * 5);
				_Dust->StartEmission();
			}
			if (_atkTimer->Check(10, 2.05))
			{
				_Dust->StopEmission();
			}
			if (_atkTimer->Check(8, 2.4))
			{
				if (PHYSICS_MGR->IsOverlap(player->pTransform, pBox, _attackTrans1, &_attackBound1))
				{
					player->Hit(500, 1.0f);
				}
				_Atk01FX.Start(0.5, false);
				_Dust->pTransform->SetWorldPosition(this->pTransform->GetWorldPosition());
				_Dust->pTransform->MovePositionLocal(-pTransform->GetForward() * 5);
				_Dust->StartEmission();
			}
			if (_atkTimer->Check(9, 2.65))
			{
				_Dust->StopEmission();
			}
			break;
		}

	
	}

}


void MiddleBoss::Hit(float damage)
{
	float totalDamage = max(0, damage - _charInfo.Def);

	_charInfo.Hp -= totalDamage;
	if (PLAYER_MGR->GetPlayer()->GetIsBuffState() == PB_FIRE)
		_MonsterDamage2.Start(0.3, false);
	else if (PLAYER_MGR->GetPlayer()->GetIsBuffState() == PB_ICE)
		_MonsterDamage3.Start(0.3, false);
	else if (PLAYER_MGR->GetPlayer()->GetIsBuffState() == PB_THUNDER)
		_MonsterDamage4.Start(0.3, false);

	//_MonsterDamage3.Start(0.3, false);
	if (!IsState(CS_ATTACK) && RandomFloatRange(0, 100) < 30)
	{
		ActionState("HIT", CS_KNOCK, 0, true);
	}

	_hitStopTime = 0.2f;
}
