#pragma once
#include "Singleton.h"

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual HRESULT Setup() = 0;
	virtual void Release() = 0;
	virtual void Update(float timeDelta) = 0;
	virtual void Render() = 0;

	virtual inline LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
};

class SceneManager : public Singleton < SceneManager >
{
private:
	std::map<std::string, Scene*> _sceneMap;

	Scene* _change = NULL;
	Scene* _current = NULL;

	std::thread _changeThread;
public:
	void Release()
	{
		for (std::pair<std::string, Scene*> i : _sceneMap)
		{
			delete i.second;
		}
	}
	void Update(float timeDelta)
	{
		if (_current) _current->Update(timeDelta);
	}
	void Render()
	{
		if (_current) _current->Render();
	}

	HRESULT Change(std::string name)
	{
		auto iter = _sceneMap.find(name);
		if (iter == _sceneMap.end()) return E_FAIL;
		if (_current == iter->second) return E_FAIL;
	
		LOG_MGR->AddLog("SceneManager : 전환 - %s", name.c_str());
		if (SUCCEEDED(iter->second->Setup()))
		{
			LOG_MGR->AddLog("SceneManager : 셋업 OK - %s", name.c_str());

			if (_current != NULL)
			{
				(_current)->Release();
				LOG_MGR->AddLog("SceneManager : 기존 씬 릴리즈");
			}

			_current = iter->second;

			return S_OK;
		}

		LOG_MGR->AddLog("SceneManager : 셋업 fail - %s", name.c_str());

		return E_FAIL;
	}

	void Add(std::string name, Scene* scene)
	{
		_sceneMap.insert(std::make_pair(name, scene));
	}

	Scene* GetCurrent() { return _current; }
};

#define SCENE_MGR SceneManager::GetInstance()