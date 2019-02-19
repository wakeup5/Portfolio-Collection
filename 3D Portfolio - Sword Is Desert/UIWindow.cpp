#include "stdafx.h"
#include "UIWindow.h"

UIWindow* UIWindow::_root = new UIWindow;

UIWindow::UIWindow()
	: _parent(NULL)
	, _position(0, 0, 0)
	, _size(0, 0)
	, _deligate(NULL)
	, _texture(NULL)
	, _isHidden(false)
	, _isMouseOver(false)
	, _isMouseClick(false)
	, _tagNum(0)
	, _top(this)
{
	D3DXMatrixIdentity(&_matSize);
#ifdef _DEBUG
	_isDrawBounding = true;
#else
	_isDrawBounding = false;
#endif
}

UIWindow::~UIWindow()
{

}

void UIWindow::Setup()
{
	_root->AddChild(this);
	_deligate = new UIDeligateDefault;
}
void UIWindow::Release()
{
	if (_texture) _texture->Release();

	//if (_deligate) delete _deligate;

	auto iter = _root->_child.find(this);
	if(iter != _root->_child.end())	_root->_child.erase(iter);

	//UIWindow* c;
	//for (std::set<UIWindow*>::iterator i = _child.begin(); i != _child.end(); i++)
	//{
		//c = (*i);
		//c->Release();
		//delete c;
		//c = NULL;
	//}

	_child.clear();
}
void UIWindow::Update()
{
	for (auto i = _child.begin(); i != _child.end(); ++i)
	{
		(*i)->Update();
		if ((*i)->IsThisMouseOver())
		{
			//LOG_MGR->AddLog("자식 들어왔니?");
			_isMouseClick = false;
			_isMouseOver = false;

			return;
		}
	}
	/*
	for (int i = 0; i < _child.size(); ++i)
	{
		_child[i]->Update();
		if (_child[i]->IsThisMouseOver())
		{
			//LOG_MGR->AddLog("자식 들어왔니?");
			_isMouseClick = false;
			_isMouseOver = false;

			return;
		}
	}
	*/

	if (_isHidden)
		return;

	POINT pos = GetFinalPosition();

	RECT rc;
	SetRect(&rc, pos.x, pos.y, pos.x + _size.width, pos.y + _size.height);

	POINT mousePt = GetMousePos();
	
	if (PtInRect(&rc, mousePt))
	{
		if (!_isMouseOver)
		{
			if (_deligate) _deligate->OnMouseOver(this, this->_root);
			_isMouseOver = true;
		}

		if (KEY_MGR->IsStayDown(VK_RBUTTON)){
			if (_deligate) _deligate->OnRightClickDown(this, this->_root);
		}

		if (KEY_MGR->IsStayDown(VK_LBUTTON))
		{
			//GS.isUIClick = true;
			if (!_isMouseClick)
			{
				if (_deligate) _deligate->OnClickDown(this, this->_root);
				_isMouseClick = true;
			}

			if (_deligate) _deligate->OnClick(this, this->_root);
		}
		else
		{
			if (_isMouseClick)
			{
				//GS.isUIClick = false;
				if (_deligate) _deligate->OnClickUp(this, this->_root);
				_isMouseClick = false;
			}
		}
	}
	else
	{
		if (_isMouseOver)
		{
			if (_deligate) _deligate->OnMouseOut(this, this->_root);
			_isMouseOver = false;
		}
	}

	if (_isMouseClick)
	{
		if (_deligate) _deligate->OnDrag(this, this->_root);
	}

}

void UIWindow::Render(LPD3DXSPRITE sprite)
{
	if (_isHidden)
		return;

	D3DXMATRIXA16 matPos, matTotal;

	//이동
	POINT pos = GetFinalPosition();

	D3DXMatrixTranslation(&matPos, pos.x, pos.y, 0.0f);

	//합
	D3DXMatrixIdentity(&matTotal);
	matTotal = _matSize * matPos;

	if (_texture)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		sprite->SetTransform(&matTotal);

		RECT rc;
		SetRect(&rc, 0, 0, _textureInfo.Width, _textureInfo.Height);
		sprite->Draw(_texture,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			D3DCOLOR_ARGB(_alphaBlend, 255, 255, 255));

		//Sprite->Draw(buttonTexture, NULL, NULL, &D3DXVECTOR3(-width2, -height2, 0.0), whitecol);

		sprite->End();
	}

	if (_isDrawBounding)
	{
		std::vector<RHWC_VERTEX> vecVertex;
		vecVertex.reserve(5);

		RHWC_VERTEX v;

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(matTotal._41, matTotal._42, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(matTotal._41 + _size.width, matTotal._42, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(matTotal._41 + _size.width, matTotal._42 + _size.height, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(matTotal._41, matTotal._42 + _size.height, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(matTotal._41, matTotal._42, 0, 1);
		vecVertex.push_back(v);

		DEVICE->SetFVF(RHWC_VERTEX::FVF);
		DEVICE->DrawPrimitiveUP(
			D3DPT_LINESTRIP, 4, &vecVertex[0], sizeof(RHWC_VERTEX));
	}
	/*
	for (int i = 0; i < _child.size(); ++i)
	{
		_child[i]->Render(sprite);
	}
	*/
	for (auto i = _child.begin(); i != _child.end(); ++i)
	{
		(*i)->Render(sprite);
	}
}

UIWindow* UIWindow::AttachTo(UIWindow* parents)
{
	parents->AddChild(this);

	return this;
}
UIWindow* UIWindow::AddChild(UIWindow* child)
{
	this->_child.insert(child);
	child->_parent = this;
	child->_root = this->_root;

	return this;
}

UIWindow* UIWindow::AttackToWindow(UIWindow * top)
{
	_top = top;

	return this;
}

UIWindow* UIWindow::SetHiddenState(bool is)
{
	for (auto i = _child.begin(); i != _child.end(); ++i)
	{
		(*i)->SetHiddenState(is);
	}

	_isHidden = is;

	if (_isHidden)
	{
		_isMouseOver = false;
		_isMouseClick = false;
	}

	return this;
}

UIWindow* UIWindow::SetPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;

	return this;
}
POINT UIWindow::GetPosition()
{
	POINT result;
	result.x = _position.x;
	result.y = _position.y;
	return result;
}
UIWindow* UIWindow::SetSize(int width, int height)
{
	_size.width = width;
	_size.height = height;

	UpdateTextureSizeMatrix();

	return this;
}

WindowSize UIWindow::GetSize()
{
	return _size;
}

POINT UIWindow::GetFinalPosition()
{
	POINT pos;
	pos.x = _position.x;
	pos.y = _position.y;

	if (_parent != NULL)
	{
		pos.x += _parent->GetFinalPosition().x;
		pos.y += _parent->GetFinalPosition().y;
	}

	return pos;
}

UIWindow* UIWindow::SetDrawBoundingState(bool is)
{
	for (auto i = _child.begin(); i != _child.end(); ++i)
	{
		(*i)->SetDrawBoundingState(is);
	}


	_isDrawBounding = is;

	return this;
}

UIWindow* UIWindow::SetBackground(LPDIRECT3DTEXTURE9 tex, D3DXIMAGE_INFO info)
{
	if (_texture) _texture->Release();

	_texture = tex;

	if (!tex) return this;

	_texture->AddRef();
	_textureInfo = info;

	UpdateTextureSizeMatrix();

	return this;
}

UIWindow* UIWindow::SetBackground(char* path)
{
	if (_texture) _texture->Release();

	D3DXIMAGE_INFO info;
	_texture = TEXTURE_MGR->GetTexture(path, &info);

	if (!_texture) return this;

	_texture->AddRef();
	_textureInfo = info;

	UpdateTextureSizeMatrix();

	return this;
}


UIWindow* UIWindow::SetBackground(std::string path)
{
	if (_texture) _texture->Release();

	D3DXIMAGE_INFO info;
	_texture = TEXTURE_MGR->GetTexture(path, &info);

	if (!_texture) return this;

	_texture->AddRef();
	_textureInfo = info;

	UpdateTextureSizeMatrix();

	return this;
}


UIWindow* UIWindow::GetWindowByTag(UINT num)
{
	if (GetTagNum() == num) return this;

	UIWindow* result = NULL;

	for (auto i = _child.begin(); i != _child.end(); ++i)
	{
		if ((*i)->GetTagNum() == num) return (*i);

		result = (*i)->GetWindowByTag(num);

		if (result) break;
	}

	return result;
}

bool UIWindow::IsMouseOver()
{
	for (auto i = _child.begin(); i != _child.end(); ++i)
	{
		if ((*i)->IsThisMouseOver())
			return true;
		else if ((*i)->IsMouseOver())
			return true;
	}

	if (_isHidden)
		return false;

	return _isMouseOver;
}

void UIWindow::Toggle()
{
	SetHiddenState(!IsHidden());//IsHidden()
}

void UIWindow::UpdateTextureSizeMatrix()
{
	if (!_texture) return;

	D3DXMatrixIdentity(&_matSize);
	D3DXMatrixScaling(&_matSize, (float)_size.width / (float)_textureInfo.Width, (float)_size.height / (float)_textureInfo.Height, 0);
}

void UIWindow::FillTextureRender(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 _t, D3DXIMAGE_INFO _i)
{
	D3DXMATRIXA16 matPos, matSize, matTotal;

	//이동
	POINT pos = GetFinalPosition();

	D3DXMatrixTranslation(&matPos, pos.x, pos.y, 0.0f);

	//사이즈
	D3DXMatrixIdentity(&matSize);
	D3DXMatrixScaling(&matSize, ((float)_size.width / (float)_i.Width), (float)_size.height / (float)_i.Height, 0);

	//합
	D3DXMatrixIdentity(&matTotal);
	matTotal = matSize * matPos;

	if (_t)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		sprite->SetTransform(&matTotal);

		RECT rc;
		SetRect(&rc, 0, 0, _i.Width, _i.Height);
		sprite->Draw(_t,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			D3DCOLOR_ARGB(255, 255, 255, 255));

		//Sprite->Draw(buttonTexture, NULL, NULL, &D3DXVECTOR3(-width2, -height2, 0.0), whitecol);

		sprite->End();
	}
}
