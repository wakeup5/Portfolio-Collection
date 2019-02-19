#pragma once

class Device : public Singleton < Device >
{
private:
	LPDIRECT3DDEVICE9 _device;

public:
	Device();
	virtual ~Device();

public:
	LPDIRECT3DDEVICE9 GetDevice();

	HRESULT Setup();
	void Release() override;
};

#define DEVICEMANAGER	Device::GetInstance()
#define DEVICE			DEVICEMANAGER->GetDevice()
