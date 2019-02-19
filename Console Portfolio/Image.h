#pragma once

enum IMAGE_CG
{
	CG_OPENING,
	CG_YEUN_NORMAL,
	CG_WONJANG_NORMAL,
	CG_LINA_NORMAL
};

enum IMAGE_ANIMATE
{
	ANIMATE_OPENING
};

class Image
{
public:
	Image();
	~Image();

	static char* get(IMAGE_CG);
	//static char* getAnimate(IMAGE_ANIMATE);
};

