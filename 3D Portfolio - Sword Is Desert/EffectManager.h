#pragma once

#include "Singleton.h"
#include "cQuadParticleEmitter.h"
#include "cPartcleEmitter.h"
#include "cParticleQuad.h"
#include "cParticle.h"
#include "fog.h"
#include "Around_Dust.h"
#include "Dust.h"
#include "Fire.h"
#include "RideFX.h"
#include "effect_02.h"
#include "PC_UFX.h"
#include "PCU_Around_Dust.h"
#include "HitRing.h"
#include "Sword_effect.h"
#include "MoveAttackFX.h"
//#include "BossAtkFX.h"
#include "RollingDust.h"
#include "ShiledFX.h"
#include "ShiledPtFX.h"
#include "BowEffect03.h"
#include "Uskill_DustFX.h"
//#include "SpriteEffect.h"
class SpriteEffect;

typedef struct tagSpriteEffectParam
{
	D3DXVECTOR3 MinSpeed = D3DXVECTOR3(0, 0, 0), MaxSpeed = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 MinRot = D3DXVECTOR3(0, 0, 0), MaxRot = D3DXVECTOR3(0, 0, 0);
	std::vector<D3DXVECTOR3> Scales;
	std::vector<DWORD> Colors;

	void Clear()
	{
		MinSpeed = MaxSpeed = MinRot = MaxRot = D3DXVECTOR3(0, 0, 0);
		Scales.clear();
		Colors.clear();
	}
} SEParam;

//편리하게 로그
#define LOG_MGR_VECTOR(v) LOG_MGR->AddLog("%.2f, %.2f, %.2f", v.x, v.y, v.z)
//편의 타입 정의
class EffectVector
{
private:
	std::vector<cQuadParticleEmitter*> _qData;
	std::vector<cPartcleEmitter*> _pData;
	std::vector<SpriteEffect*> _sData;
	float _playTime;

	Transform* _trans = NULL;
	bool _isLoop = false;

	bool _isStart = false;

	D3DXVECTOR3 _pos;
	D3DXVECTOR3 _dir;
	float _speed = 0;
public:
	void push_back(cQuadParticleEmitter* data) { _qData.push_back(data);}
	void push_back(cPartcleEmitter* data) { _pData.push_back(data); }
	void push_back(SpriteEffect* data) { _sData.push_back(data); }
	void clear() { _qData.clear(); _pData.clear(); _sData.clear(); }

	virtual void Update(float timeDelta);
	virtual void Render();

	void Start(float playTime = 0.0f, bool isLoop = false);
	void Stop();

	void SetPosition(D3DXVECTOR3 pos)
	{ 
		for (int i = 0; i < _qData.size(); i++) _qData[i]->pTransform->SetWorldPosition(pos);
		for (int i = 0; i < _pData.size(); i++) _pData[i]->pTransform->SetWorldPosition(pos);
	}

	void SetRotation(D3DXMATRIXA16& rot)
	{
		for (int i = 0; i < _qData.size(); i++) _qData[i]->pTransform->SetRotateLocal(rot);
		for (int i = 0; i < _pData.size(); i++) _pData[i]->pTransform->SetRotateLocal(rot);
	}

	void SetTransform(Transform* trans)
	{
		_trans = trans;
	}

	void SetOrigin(Transform* trans);

	void Move(D3DXVECTOR3 pos, D3DXVECTOR3 dir, float speed)
	{
		_pos = pos;
		_dir = dir;
		_speed = speed;
	}
};

class EffectManager : public Singleton < EffectManager >
{
private:

	std::map<std::string, cQuadParticleEmitter*> _em; //쿼드 파티클
	std::map<std::string, cPartcleEmitter*> _pm; //파티클

	//스프라이트 이펙트
	std::map<std::string, SpriteEffect*> _sm;
public:
	EffectManager();
	~EffectManager();

	void Setup();
	void Release();
	void Update(float timedelta);
	void Render();
	//void CreateEffect(std::string name);
	
private:
	template < typename T, typename O >
	T* Create(std::string name);

public:
	template < typename T > 
	cQuadParticleEmitter* CreateQuadParticleEmitter(std::string name);
	cQuadParticleEmitter* GetQuadParticleEmitter(std::string name);

	template < typename T >
	cPartcleEmitter* CreateParticleEmitter(std::string name);
	cPartcleEmitter* GetParticleEmitter(std::string name);

	SpriteEffect* CreateEffect(std::string name, LPDIRECT3DTEXTURE9 tex, UINT u, UINT v, Transform* origin, bool isCross = false, const SEParam* param = NULL);
	SpriteEffect* CreateEffect(std::string name, LPDIRECT3DTEXTURE9 tex, UINT u, UINT v, Transform* origin, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool isCross = false, const SEParam* param = NULL);
	SpriteEffect* CreateEffect(std::string name, LPDIRECT3DTEXTURE9 tex, UINT u, UINT v, Transform* origin, const D3DXMATRIXA16* mat, bool isCross = false, const SEParam* param = NULL);
	SpriteEffect* GetEffect(std::string name);
	
	void Start(std::string name, float lifeTime, bool isLoop = false);
	void Stop(std::string name);
};

#define EFFECT_MGR EffectManager::GetInstance()

//템플릿 함수 정의
template < typename T, typename O >
T* EffectManager::Create(std::string name)
{
	T* result = new T;

	O* temp = static_cast<O*>(result);

	if (!temp)
	{
		delete result;
		return NULL;
	}

	result->Setup();
	result->SetActive(true);
	return result;
}

template < typename T >
cQuadParticleEmitter* EffectManager::CreateQuadParticleEmitter(std::string name)
{
	//이미 있는지 확인
	cQuadParticleEmitter* temp = GetQuadParticleEmitter(name);
	if (temp != NULL) return temp;

	//생성 확인
	T* result = Create<T, cQuadParticleEmitter>(name);
	if (result != NULL) 
		_em.insert(std::make_pair(name, result));

	return result;
}

template < typename T >
cPartcleEmitter* EffectManager::CreateParticleEmitter(std::string name)
{
	//있는지 확인
	cPartcleEmitter* temp = GetParticleEmitter(name);
	if (temp != NULL) return temp;

	//생성 확인
	T* result = Create<T, cPartcleEmitter>(name);
	if (result != NULL)
		_pm.insert(std::make_pair(name, result));

	return result;
}