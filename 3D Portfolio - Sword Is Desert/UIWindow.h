#pragma once
#include "UICommon.h"

#define DELIGATEFUNCTION(func) virtual UINT func(UIWindow*, UIWindow*){ return RETURN_DEFAULT; } 

enum RETURNVALUE
{
	RETURN_DEFAULT,
	RETURN_OK,
	RETURN_NO,
	RETURN_CANSLE,
	RETURN_EXIT,
	RETURNVALUE_LENGTH
};

class UIWindow;
interface UIDeligate
{
protected:
	friend class UIWindow;

	DELIGATEFUNCTION(OnClick);
	DELIGATEFUNCTION(OnClickDown);
	DELIGATEFUNCTION(OnClickUp);
	DELIGATEFUNCTION(OnRightClickDown);
	DELIGATEFUNCTION(OnDrag);
	DELIGATEFUNCTION(OnMouseOver);
	DELIGATEFUNCTION(OnMouseOut);
	
};

class UIWindow
{
public:
	friend class UIDeligateDefault;

	UIWindow();
	virtual ~UIWindow();

	virtual void Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render(LPD3DXSPRITE sprite);

public:
	//메소드 체인
	UIWindow* SetDeligate(UIDeligate* deligate) { if (_deligate) delete _deligate; _deligate = deligate; return this; }

	UIWindow* AttachTo(UIWindow* parents);
	UIWindow* AddChild(UIWindow* child);

	UIWindow* AttackToWindow(UIWindow* top);
	UIWindow* GetTop() { return _top; }

	UIWindow* SetHiddenState(bool is);
	bool IsHidden() { return _isHidden; }
	UIWindow* SetDrawBoundingState(bool is);
	bool IsDrawBounding() { return _isDrawBounding; }

	UIWindow* SetBackground(LPDIRECT3DTEXTURE9 tex, D3DXIMAGE_INFO info);
	UIWindow* SetBackground(char* path);
	UIWindow* SetBackground(std::string path);
	UIWindow* SetPosition(float x, float y);
	POINT GetPosition();
	UIWindow* SetSize(int width, int height);
	WindowSize GetSize();

	POINT GetFinalPosition();

	UIWindow* SetTagNum(UINT num) { _tagNum = num; return this; }
	UINT GetTagNum() { return _tagNum; }

	UIWindow* GetWindowByTag(UINT num);

	bool IsMouseOver();

	void Toggle();

	static UIWindow* GetRoot() { return _root; }
	void SetAlphaBlend(int alpha){
		_alphaBlend = alpha;
	}
protected:
	void UpdateTextureSizeMatrix();

	bool IsThisMouseOver() { return _isMouseOver; }

	void FillTextureRender(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 _t, D3DXIMAGE_INFO _i);

	//private:
		//위치와 크기
	D3DXVECTOR3 _position;
	WindowSize _size;

	UIDeligate* _deligate = NULL;

	static UIWindow* _root;
	UIWindow* _top;
	UIWindow* _parent;
	std::set<UIWindow*> _child;

	//텍스쳐
	LPDIRECT3DTEXTURE9 _texture;
	D3DXIMAGE_INFO _textureInfo;
	//LPD3DXSPRITE _sprite;

	//숨김
	bool _isHidden;

	//텍스쳐 사이즈 셋팅하려는 매트릭스
	D3DXMATRIXA16 _matSize;

	//이벤트 관련
	bool _isMouseOver;
	bool _isMouseClick;

	//테두리 그림?
	bool _isDrawBounding;

	//윈도우 구분하기 위한 태그번호
	UINT _tagNum;

	// sprite 투명도
	int _alphaBlend = 255;

};

class UIDeligateDefault : public UIDeligate
{
	//private:
protected:
	friend class UIWindow;

	POINT _clickDownPt;
	POINT _clickDownWindowPt;
	POINT _clickUpPt;
	POINT _clickUpWindowPt;
	POINT _prevClickPt;

	inline virtual UINT OnClick(UIWindow* caller, UIWindow* root) override
	{
		return RETURN_DEFAULT;
	}
	inline virtual UINT OnClickDown(UIWindow* caller, UIWindow* root) override
	{
		//LOG_MGR->AddLog("down");
		_clickDownPt = MyUtil::GetMousePos();
		_clickDownWindowPt = caller->GetTop()->GetPosition();

		return RETURN_DEFAULT;
	}
	inline virtual UINT OnClickUp(UIWindow* caller, UIWindow* root) override
	{
		//LOG_MGR->AddLog("up");
		return RETURN_DEFAULT;
	}
	inline virtual UINT OnMouseOver(UIWindow* caller, UIWindow* root) override
	{
		//LOG_MGR->AddLog("over");
		return RETURN_DEFAULT;
	}
	inline virtual UINT OnMouseOut(UIWindow* caller, UIWindow* root) override
	{
		//LOG_MGR->AddLog("out");
		return RETURN_DEFAULT;
	}
	inline virtual UINT OnDrag(UIWindow* caller, UIWindow* root) override
	{
		//LOG_MGR->AddLog("drag");
		POINT currentMousePt = MyUtil::GetMousePos();
		POINT currentPos = {};

		currentPos.x = _clickDownWindowPt.x + currentMousePt.x - _clickDownPt.x;
		currentPos.y = _clickDownWindowPt.y + currentMousePt.y - _clickDownPt.y;

		caller->GetTop()->SetPosition(currentPos.x, currentPos.y);

		return RETURN_DEFAULT;
	}
};

class UICloseDeligate : public UIDeligate
{
	inline virtual UINT OnClickUp(UIWindow* caller, UIWindow* root) override
	{
		//root->GetWindowByTag(UITag::UI_SCREEN)->SetHiddenState(true);
		caller->GetTop()->SetHiddenState(true);
		return RETURN_DEFAULT;
	}
};