#pragma once

template < class T > // : public Singleton >
class Singleton
{
protected:
	static T* _instance;

protected:
	Singleton() {}
	~Singleton() {}

public:
	static T* GetInstance()
	{
		if (_instance == NULL)
		{
			_instance = new T;
			//_instance->Setup();
		}

		return static_cast <T*>(_instance);
	}

	static void ReleaseInstance()
	{
		if (_instance != NULL)
		{
			_instance->Release();
			delete _instance;
			_instance = NULL;
		}
	}

	//virtual HRESULT Setup()
	virtual void Release() {}
};

template < class T >
T* Singleton<T>::_instance = NULL;