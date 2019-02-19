#include "stdafx.h"
#include "Figure.h"

LPD3DXEFFECT Figure::Effect = NULL;

MyPCT_Vertex Figure::_pointBlendVertex[6];
bool Figure::_initPointVertex = false;
bool Figure::_isRenderBegin = false;

Figure::Figure()
{
	this->Effect = RESOURCE_FX->LoadResource("../Resources/Shaders/Default.fx");
	_alpha = 0;

	pTransform.SetWorldPosition(0, 0, 0);
	pTransform.SetRotateLocal(0, 0, 0);

	_pointTrans = new Transform;

	if (!_initPointVertex)
	{
		_pointBlendVertex[0] = MyPCT_Vertex(1, 1, 0, 0, 0);
		_pointBlendVertex[1] = MyPCT_Vertex(-1, 1, 0, 1, 0);
		_pointBlendVertex[2] = MyPCT_Vertex(-1, -1, 0, 1, 1);
		_pointBlendVertex[3] = _pointBlendVertex[0];
		_pointBlendVertex[4] = _pointBlendVertex[2];
		_pointBlendVertex[5] = MyPCT_Vertex(1, -1, 0, 0, 1);

		_initPointVertex = true;
	}

	_pointTexture = RESOURCE_TEXTURE->GetResource(RESOURCE("Textures/figureTex.png"));
}


Figure::~Figure()
{
	_initPointVertex = false;
}

void Figure::Render()
{
	/*
	if (!this->Effect) return;
	
	this->Effect->SetTechnique("Vertex");

	UINT pass;

	D3DXMATRIXA16 matWorld, matRot, matPos;
	//matWorld = pTransform.GetWorldRotateMatrix();
	matWorld = pTransform.GetFinalMatrix();
	
	//D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);

	this->Effect->SetMatrix("matWorld", &matWorld);
	this->Effect->SetMatrix("matViewProjection", &CAMERA->GetViewProjectionMatrix());
	this->Effect->SetFloat("alpha", _alpha);
	
	this->Effect->Begin(&pass, 0);

	DEVICE->SetFVF(MyPC_Vertex::FVF);
	for (int i = 0; i < pass; i++)
	{
		this->Effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(_primitiveType, _count, &_ver[0], sizeof(MyPC_Vertex));

		this->Effect->EndPass();
	}

	this->Effect->End();
	*/
	if (!_isRenderBegin) return;
	
	D3DXMATRIXA16 matWorld, texWorld;
	matWorld = pTransform.GetFinalMatrix();
	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	DEVICE->SetFVF(MyPC_Vertex::FVF);
	DEVICE->DrawPrimitiveUP(_primitiveType, _count, &_ver[0], sizeof(MyPC_Vertex));

	_pointTrans->SetWorldMatrix(matWorld);
	_pointTrans->SetScale(_size, _size, _size);
	_pointTrans->LookPosition(CAMERA->GetWorldPosition());
	DEVICE->SetFVF(MyPCT_Vertex::FVF);
	DEVICE->SetTexture(0, _pointTexture);
	for (int i = 0; i < _ver.size(); i++)
	{
		//_pointTrans->SetLocalPosition(_ver[i].p);
		//D3DXMatrixTranslation(&texWorld, _ver[i].p.x, _ver[i].p.y, _ver[i].p.z);
		texWorld = _pointTrans->GetFinalMatrix();
		texWorld._41 += _ver[i].p.x;
		texWorld._42 += _ver[i].p.y;
		texWorld._43 += _ver[i].p.z;

		DEVICE->SetTransform(D3DTS_WORLD, &(texWorld));

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &_pointBlendVertex[0], sizeof(MyPCT_Vertex));
	}
	DEVICE->SetTexture(0, NULL);
}

void Figure::SetAlpha(float a)
{
	for (int i = 0; i < _ver.size(); i++)
	{
		_ver[i].c = D3DXCOLOR(1, 1, 1, a);
	}

	for (int i = 0; i < 6; i++)
	{
		_pointBlendVertex[i].c = D3DXCOLOR(1, 1, 1, a / 2);
	}
}

void Figure::Begin()
{
	_isRenderBegin = true;

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);

	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Texture 의 값과 Diffuse 여기서는 정점컬러의 알파값 을 섞어 최종 출력을 한다.
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
}

void Figure::End()
{
	_isRenderBegin = false;

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
