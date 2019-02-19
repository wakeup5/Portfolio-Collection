#pragma once

#include "Singleton.h"

class DXFontManager : public Singleton < DXFontManager >
{
private:
	typedef std::map< std::string, LPD3DXFONT>		MAP_FONT;
	typedef MAP_FONT::iterator						MAP_FONT_ITER;

private:
	LPDIRECT3DDEVICE9	m_pDevice;

	LPD3DXFONT			m_pDefultFont;		//디폴트 폰트
	MAP_FONT			m_FontStyleMap;		//폰트 StyleMap

public:
	DXFontManager(void);
	~DXFontManager(void);

	HRESULT Setup(LPDIRECT3DDEVICE9	device);
	void Release() override;


	LPD3DXFONT GetStyle(std::string style);
	void AddStyle(std::string style, std::string fontName, float fontSize);

	void PrintText(std::string str, int x, int y, DWORD color);
	void PrintTextShadow(std::string str, int x, int y, DWORD color, DWORD shadow = 0xff000000);
	void PrintTextOutline(std::string str, int x, int y, DWORD color, DWORD outLine = 0xff000000);

	void PrintStyleText(std::string style, std::string str, int x, int y, DWORD color);
	void PrintStyleTextShadow(std::string style, std::string str, int x, int y, DWORD color, DWORD shadow = 0xff000000);
	void PrintStyleTextOutline(std::string style, std::string str, int x, int y, DWORD color, DWORD outLine = 0xff000000);

};

#define DXFONT_MGR DXFontManager::GetInstance()

