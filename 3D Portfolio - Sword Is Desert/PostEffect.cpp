#include "stdafx.h"
#include "PostEffect.h"


PostEffect::PostEffect()
{
	_effect = RESOURCE_FX->GetResource(RESOURCE("Shaders/PostEffect.fx"));

	_effect->SetFloat("pixelSizeU", 1.0f / WINSIZE_X);
	_effect->SetFloat("pixelSizeV", 1.0f / WINSIZE_Y);
	_effect->SetFloat("pixelHalfSizeU", 0.5f / WINSIZE_X);
	_effect->SetFloat("pixelHalfSizeV", 0.5f / WINSIZE_Y);

	//기본
	DEVICE->CreateTexture(WINSIZE_X, WINSIZE_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_base, NULL);
	DEVICE->CreateTexture(WINSIZE_X, WINSIZE_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_depth, NULL);

	//블러
	DEVICE->CreateTexture(500, 500, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_blur, NULL);

	//모션블러인데 안씀..
	//DEVICE->CreateTexture(WINSIZE_X, WINSIZE_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_motionBlur, NULL);

	//선라이트
	DEVICE->CreateTexture(WINSIZE_X, WINSIZE_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_sunlight, NULL);

	for (int i = 0; i < 6; i++)
	{
		DEVICE->CreateTexture(WINSIZE_X / 2, WINSIZE_Y / 2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_sunlightStar[i], NULL);
	}
	DEVICE->CreateTexture(WINSIZE_X / 2, WINSIZE_Y / 2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_sunlightStarBase, NULL);
	DEVICE->CreateTexture(WINSIZE_X / 2, WINSIZE_Y / 2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_sunlightBase, NULL);

	DEVICE->CreateTexture(WINSIZE_X / 2, WINSIZE_Y / 2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_rainBase, NULL);
	DEVICE->CreateTexture(WINSIZE_X / 2, WINSIZE_Y / 2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_rainNormal, NULL);
	
	_renderVertex[0] = PT_VERTEX(-1, 1, 0, 0, 0);
	_renderVertex[1] = PT_VERTEX(1, 1, 0, 1, 0);
	_renderVertex[2] = PT_VERTEX(1, -1, 0, 1, 1);
	_renderVertex[3] = _renderVertex[0];
	_renderVertex[4] = _renderVertex[2];
	_renderVertex[5] = PT_VERTEX(-1, -1, 0, 0, 1);

	_showVertex[0] = PT_VERTEX(-1, -0.5, 0, 0, 0);
	_showVertex[1] = PT_VERTEX(-0.5, -0.5, 0, 1, 0);
	_showVertex[2] = PT_VERTEX(-0.5, -1, 0, 1, 1);
	_showVertex[3] = _showVertex[0];
	_showVertex[4] = _showVertex[2];
	_showVertex[5] = PT_VERTEX(-1, -1, 0, 0, 1);

	_lightCamera = new Camera;
	_lightCamera->OrthoToggle();
}


PostEffect::~PostEffect()
{
	SAFE_RELEASE(_base);
	SAFE_RELEASE(_depth);
	SAFE_RELEASE(_blur);

	SAFE_DELETE(_lightCamera);
}

void PostEffect::BeginBase()
{
	BeginTexture(_base);
}

void PostEffect::EndBase()
{
	EndTexture();
}

void PostEffect::RenderBase()
{
	_effect->SetTechnique("base");

	_effect->SetTexture("screenTex", _base);

	UINT pass;
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();
}

void PostEffect::BeginSunlight()
{
	//DEVICE->GetDepthStencilSurface(&_ds);
	BeginTexture(_sunlight, D3DCLEAR_TARGET);
	//DEVICE->SetDepthStencilSurface(_ds);
}

void PostEffect::EndSunlight()
{
	EndTexture();
}

void PostEffect::RenderSunlightBlur()
{
	RenderSixStar();

	BeginTexture(_sunlightBase, D3DCLEAR_TARGET);

	//블러 X
	_effect->SetTechnique("BlurX");

	_effect->SetTexture("screenTex", _sunlightStarBase);
	_effect->SetTexture("normalTex", _depth);
	_effect->SetFloat("blurScale", 2.5);

	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);

	UINT pass;
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();

	//블러 Y
	_effect->SetTechnique("BlurY");
	_effect->SetTexture("screenTex", _sunlightBase);

	_effect->Begin(&pass, 0);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}
	_effect->End();

	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

	EndTexture();
	
	_effect->SetTechnique("Base");

	_effect->SetTexture("screenTex", _sunlightBase);
	//_effect->SetTexture("normalTex", _depth);
	Figure::Begin();
	//UINT pass;
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();
	Figure::End();
}

void PostEffect::BeginRain()
{
	DEVICE->GetDepthStencilSurface(&_ds);
	BeginTexture(_rainNormal);
	DEVICE->SetDepthStencilSurface(_ds);
}

void PostEffect::EndRain()
{
	EndTexture();
}

void PostEffect::RenderRain()
{
	BeginTexture(_rainBase);

	_effect->SetTechnique("Rain");

	_effect->SetTexture("m_TxDif", _base);
	_effect->SetTexture("m_TxDif2", _rainNormal);

	UINT pass;
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();

	EndTexture();

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	_effect->SetTechnique("base");

	_effect->SetTexture("screenTex", _rainBase);
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void PostEffect::ShowTexture(LPDIRECT3DTEXTURE9 t)
{
	_effect->SetTechnique("Base");

	_effect->SetTexture("screenTex", t);

	UINT pass;
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _showVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();
}

void PostEffect::BeginTexture(LPDIRECT3DTEXTURE9 t, DWORD flag)
{
	//DEVICE->GetDepthStencilSurface(&_ds);
	DEVICE->GetRenderTarget(0, &prevTarget);
	DEVICE->GetRenderTarget(1, &prevDepth);
	//DEVICE->SetDepthStencilSurface(_ds);

	LPDIRECT3DSURFACE9 texSurface = NULL, texDepthSurface = NULL;
	t->GetSurfaceLevel(0, &texSurface); DEVICE->SetRenderTarget(0, texSurface); SAFE_RELEASE(texSurface);
	_depth->GetSurfaceLevel(0, &texDepthSurface); DEVICE->SetRenderTarget(1, texDepthSurface); SAFE_RELEASE(texDepthSurface);

	DEVICE->Clear(0, NULL, flag, 0x00000000, 1, 0);
}

void PostEffect::EndTexture()
{
	DEVICE->SetRenderTarget(0, prevTarget);
	DEVICE->SetRenderTarget(1, prevDepth);

	SAFE_RELEASE(prevTarget);
	SAFE_RELEASE(prevDepth);
}

void PostEffect::RenderSixStar()
{
	float m_fTxW = WINSIZE_X / 2;
	float m_fTxH = WINSIZE_Y / 2;

	DEVICE->SetFVF(PT_VERTEX::FVF);


	for (int j = 0; j < 6; ++j)
	{
		LPDIRECT3DTEXTURE9	pTrnd = _sunlightStar[j];

		float	fTheta = (2.f * j * D3DX_PI) / 6;
		fTheta = RandomFloatRange(fTheta - RAD(0.3), fTheta + RAD(0.3));
		float	r = 8.f;

		float	x = r * cosf(fTheta) / m_fTxW;
		float	y = r * sinf(fTheta) / m_fTxH;

		static const int MAX_SAMP = 14;

		double	fDelta = 0.01;
		D3DXVECTOR4	StarVal[MAX_SAMP];

		float	fStarPow = 1.1f;

		for (int k = 0; k < MAX_SAMP; ++k)
		{
			StarVal[k].x = x*k;
			StarVal[k].y = y*k;
			StarVal[k].z = expf(-k*k / 30.f);
		}

		BeginTexture(pTrnd);

		_effect->SetTechnique("SunlightStar");
		_effect->SetTexture("m_TxDif", _sunlight);

		_effect->SetVectorArray("m_StarVal", StarVal, MAX_SAMP);
		_effect->SetFloat("m_StarPow", fStarPow);

		_effect->Begin(NULL, 0);
		_effect->BeginPass(0);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
		_effect->End();

		EndTexture();
	}

	BeginTexture(_sunlightStarBase);

	//DEVICE->SetVertexDeclaration(m_pFVF);
	_effect->SetTechnique("SunlightStar");

	_effect->Begin(NULL, 0);
	_effect->BeginPass(1);

	for (int j = 0; j < 6; ++j)
	{
		LPDIRECT3DTEXTURE9 pTrnd = _sunlightStar[j];
		DEVICE->SetTexture(j, pTrnd);
	}

	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

	_effect->EndPass();
	_effect->End();

	EndTexture();
}

void PostEffect::BeginBlur()
{
	DEVICE->GetRenderTarget(0, &prevTarget);

	LPDIRECT3DSURFACE9 texSurface = NULL, texDepthSurface = NULL;
	_blur->GetSurfaceLevel(0, &texSurface); DEVICE->SetRenderTarget(0, texSurface); SAFE_RELEASE(texSurface);

	DEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0xffffffff, 1, 0);
}

void PostEffect::EndBlur()
{
	DEVICE->SetRenderTarget(0, prevTarget);

	SAFE_RELEASE(prevTarget);
}

void PostEffect::RenderDof()
{
	BeginBlur();
	RenderBlur();
	EndBlur();

	//Dof 그림
	_effect->SetTechnique("DepthOfField");

	_effect->SetTexture("screenTex", _base);
	_effect->SetTexture("normalTex", _depth);
	_effect->SetTexture("blurTex", _blur);

	UINT pass;
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();
}

void PostEffect::RenderBlur()
{
	_effect->SetTechnique("BlurX");

	_effect->SetTexture("screenTex", _base);
	_effect->SetTexture("normalTex", _depth);
	_effect->SetFloat("blurScale", 1);

	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);

	//블러 X
	UINT pass;
	_effect->Begin(&pass, 0);

	DEVICE->SetFVF(PT_VERTEX::FVF);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}

	_effect->End();

	_effect->SetTechnique("BlurY");
	_effect->SetTexture("screenTex", _blur);

	//블러 Y
	_effect->Begin(&pass, 0);

	for (int i = 0; i < pass; i++)
	{
		_effect->BeginPass(i);

		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, _renderVertex, sizeof(PT_VERTEX));

		_effect->EndPass();
	}
	_effect->End();

	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
}