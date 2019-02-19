#pragma once
#include <string>
#include <fstream>
//#include "Image.h"

using namespace std;

enum IMAGE_CG
{
	CG_OPENING,
	CG_YEUN_NORMAL,
	CG_WONJANG_NORMAL,
	CG_LINA_NORMAL,
	CG_K17,
	CG_TOWN,
	CG_MAEJUM,
	CG_MAX
};

class GameImage
{
private:

	static string _images[CG_MAX];

	GameImage();
	~GameImage();

	static string load(IMAGE_CG, string);

public:
	static string get(IMAGE_CG);

};

