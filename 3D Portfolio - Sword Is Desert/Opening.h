#pragma once
#include "Scene.h"
#include "UIScreen.h"
#include "UIWindow.h"
#include "UIButton.h"

class Opening :
	public Scene
{
private:
	LPD3DXSPRITE _sprite;
	UIWindow* _ui;
	float x;
	float time;
public:
	Opening();
	~Opening();
	
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();

};


class OpeningDeligate : public UIDeligate
{
public:
	inline virtual UINT OnClickDown(UIWindow* caller, UIWindow* root) override
	{
		UINT t = caller->GetTagNum();
		LOG_MGR->AddLog("안에 클릭");
		caller->SetBackground("../Resources/Textures/opening/btn/op_start_btn_click.png");


		return RETURN_DEFAULT;
	}
	
};

