#include "stdafx.h"
#include "Device.h"


Device::Device()
	: _device(NULL)
{
}


Device::~Device()
{
}

LPDIRECT3DDEVICE9 Device::GetDevice()
{
	return _device;
}

HRESULT Device::Setup()
{
	if (_device)
	{
		_device->Release();
		delete _device;
		_device = NULL;
	}

	LPDIRECT3D9 d3d9;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (d3d9 == NULL) {
		MessageBox(hWnd, "Direct3DCreate9() - FAILED", "FAILED", MB_OK);
		return E_FAIL;
	}

	D3DCAPS9	caps;
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,		//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,			//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		&caps					//디바이스 정보를 받아올 D3DCAPS9 포인터
		);

	DWORD vertexProcessing = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
	d3dpp.BackBufferCount = 1;						//백버퍼는 한개
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//백버퍼 색상 포맷 알파8비트,레드8비트,그린8비트,블루8비트
	d3dpp.BackBufferHeight = WINSIZE_Y;				//백버퍼 픽셀 높이 크기
	d3dpp.BackBufferWidth = WINSIZE_X;				//백버퍼 픽셀 가로 크기
	d3dpp.EnableAutoDepthStencil = true;						//자동 깊이버퍼 사용 여부 ( 그냥 true )
	d3dpp.Flags = 0;						//일단 0
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//화면 주사율 ( 그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일시 )
	d3dpp.hDeviceWindow = hWnd;					//Device 가 출력될 윈도우 핸들
	d3dpp.MultiSampleQuality = 0;						//멀티 샘플링 질 
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//멀티 샘플링 타입 ( D3DMULTISAMPLE_NONE 안티알리아싱 사용안함 ) 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//화면 스왑 체인 방식 ( D3DSWAPEFFECT_DISCARD )
	d3dpp.Windowed = true;

	HRESULT result = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,			//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		hWnd,						//디바이스를 사용할 윈도우 핸들
		vertexProcessing,			//정점 처리 방식에 대한 플레그
		&d3dpp,						//앞에서 정의한 D3DPRESENT_PARAMETERS 구조체 포인터
		&_device);				//얻어올 디바이스 더블포인터

	if (FAILED(result))
	{
		d3d9->Release();
		d3d9 = NULL;

		MessageBox(hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK);
		return E_FAIL;
	}

	d3d9->Release();
	d3d9 = NULL;
	return S_OK;
}

void Device::Release()
{
	SAFE_RELEASE(_device);
}
