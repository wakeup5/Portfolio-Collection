#include "stdafx.h"
#include "EffectManager.h"
#include "SpriteEffect.h"

EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
}

void EffectManager::Setup()
{

}

void EffectManager::Release()
{
	for (auto iter = _em.begin(); iter != _em.end(); iter++)
	{
		iter->second->Release();
		SAFE_DELETE(iter->second);
	}

	for (auto iter = _pm.begin(); iter != _pm.end(); iter++)
	{
		iter->second->Release();
		SAFE_DELETE(iter->second);
	}

	for (auto iter = _sm.begin(); iter != _sm.end(); iter++)
	{
		iter->second->Release();
		SAFE_DELETE(iter->second);
	}
}
void EffectManager::Update(float timedelta)
{
	for (auto iter = _em.begin(); iter != _em.end(); iter++)
	{
		iter->second->Update(timedelta);
	}

	for (auto iter = _pm.begin(); iter != _pm.end(); iter++)
	{
		iter->second->Update(timedelta);
	}

	for (auto iter = _sm.begin(); iter != _sm.end(); iter++)
	{
		iter->second->Update(timedelta);
	}
}
void EffectManager::Render()
{
	for (auto iter = _em.begin(); iter != _em.end(); iter++)
	{
		iter->second->Render();
	}

	for (auto iter = _pm.begin(); iter != _pm.end(); iter++)
	{
		iter->second->Render();
	}

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);		//라이팅을 끈다.
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);	//z 버퍼의 쓰기를 막는다.

	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//알파 블렌딩 셋팅
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//Texture 의 값과 Diffuse 여기서는 정점컬러의 알파값 을 섞어 최종 출력을 한다.
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	for (auto iter = _sm.begin(); iter != _sm.end(); iter++)
	{
		iter->second->Render();
	}

	//파티클 그리고 난후 후처리
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	DEVICE->SetTexture(0, NULL);
}

cQuadParticleEmitter* EffectManager::GetQuadParticleEmitter(std::string name)
{
	auto iter = _em.find(name);

	if (iter == _em.end())
	{
		return NULL;
	}
	
	return iter->second;
}

cPartcleEmitter* EffectManager::GetParticleEmitter(std::string name)
{
	auto iter = _pm.find(name);

	if (iter == _pm.end())
	{
		return NULL;
	}

	return iter->second;
}

SpriteEffect * EffectManager::CreateEffect(std::string name, LPDIRECT3DTEXTURE9 tex, UINT u, UINT v, Transform* origin, bool isCross, const SEParam* param)
{
	static UINT num = 0;
	SpriteEffect* result;

	if (name == "")
	{
		name = std::string("sprite effect - ") + std::to_string(num++);
		result = NULL;
	}
	else
	{
		result = GetEffect(name);
	}

	if (result == NULL)
	{
		result = new SpriteEffect;
		_sm.insert(std::make_pair(name, result));
	}

	result->Setup(param);
	result->SetTexture(tex);
	result->SetUV(u, v);
	result->SetCrossState(isCross);
	result->Origin = origin;
	//if (origin) origin->AddChild(result->Local);
	result->Stop();

	return result;
}

SpriteEffect * EffectManager::CreateEffect(std::string name, LPDIRECT3DTEXTURE9 tex, UINT u, UINT v, Transform * origin, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool isCross, const SEParam* param)
{
	SpriteEffect* result = CreateEffect(name, tex, u, v, origin, isCross, param);

	result->Local->SetLocalPosition(pos);
	result->Local->SetScale(scale);
	result->Scale = scale;
	result->Local->SetRotateLocal(rot.x, rot.y, rot.x);

	return result;
}

SpriteEffect * EffectManager::CreateEffect(std::string name, LPDIRECT3DTEXTURE9 tex, UINT u, UINT v, Transform * origin, const D3DXMATRIXA16 * mat, bool isCross, const SEParam* param)
{
	SpriteEffect* result = CreateEffect(name, tex, u, v, origin, isCross, param);

	result->Local->SetWorldMatrix(*mat);

	return result;
}

SpriteEffect * EffectManager::GetEffect(std::string name)
{
	auto iter = _sm.find(name);

	if (iter == _sm.end()) return NULL;

	return iter->second;
}

void EffectManager::Start(std::string name, float lifeTime, bool isLoop)
{
	SpriteEffect* result = GetEffect(name);

	if (result != NULL) result->Start(lifeTime, isLoop);
}

void EffectManager::Stop(std::string name)
{
	SpriteEffect* result = GetEffect(name);

	if (result != NULL) result->Stop();
}

void EffectVector::Update(float timeDelta)
{
	for (int i = 0; i < _qData.size(); i++) _qData[i]->Update(timeDelta);
	for (int i = 0; i < _pData.size(); i++) _pData[i]->Update(timeDelta);
	for (int i = 0; i < _sData.size(); i++) _sData[i]->Update(timeDelta);

	if (!_isLoop &&
		(_playTime -= timeDelta) < 0 &&
		_isStart) 
		Stop();

	if (_trans != NULL)
	{
		for (int i = 0; i < _qData.size(); i++) _qData[i]->pTransform->SetRotateWorld(_trans->GetWorldRotateMatrix());
		for (int i = 0; i < _pData.size(); i++) _pData[i]->pTransform->SetRotateWorld(_trans->GetWorldRotateMatrix());

		for (int i = 0; i < _qData.size(); i++) _qData[i]->pTransform->SetWorldMatrix(_trans->GetFinalMatrix());
		for (int i = 0; i < _pData.size(); i++) _pData[i]->pTransform->SetWorldMatrix(_trans->GetFinalMatrix());
	}
	else if (_isStart && _speed > 0.01)
	{
		_pos += _dir * _speed * timeDelta;
		SetPosition(_pos);
		LOG_MGR_VECTOR(_pos);
	}
	//*/
}

void EffectVector::Render()
{
	//return;
	for (int i = 0; i < _qData.size(); i++) _qData[i]->Render();
	for (int i = 0; i < _pData.size(); i++) _pData[i]->Render();

	Figure::Begin();
	for (int i = 0; i < _sData.size(); i++) _sData[i]->Render();
	Figure::End();
}
void EffectVector::Start(float playTime, bool isLoop)
{
	for (int i = 0; i < _qData.size(); i++) _qData[i]->StartEmission();
	for (int i = 0; i < _pData.size(); i++) _pData[i]->StartEmission();

	_playTime = playTime;
	_isLoop = isLoop;

	_isStart = true;

	for (int i = 0; i < _sData.size(); i++) _sData[i]->Start(playTime, isLoop);
}

void EffectVector::Stop()
{
	for (int i = 0; i < _qData.size(); i++) _qData[i]->StopEmission();
	for (int i = 0; i < _pData.size(); i++) _pData[i]->StopEmission();
	for (int i = 0; i < _sData.size(); i++) _sData[i]->Stop();
	_isStart = false;
}

void EffectVector::SetOrigin(Transform * trans)
{
	for (int i = 0; i < _sData.size(); i++) _sData[i]->Origin = trans;
}