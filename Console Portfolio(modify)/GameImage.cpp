#include "GameImage.h"


GameImage::GameImage()
{
}


GameImage::~GameImage()
{
}

string GameImage::_images[CG_MAX];

string GameImage::get(IMAGE_CG cg)
{
	string fileName = "";
	switch (cg)
	{
	case CG_K17:
		fileName = "/cg/k17.txt";
		break;
	case CG_OPENING:
		fileName = "/cg/opening.txt";
		break;
	case CG_YEUN_NORMAL:
		fileName = "/cg/yeun_normal.txt";
		break;
	case CG_WONJANG_NORMAL:
		fileName = "/cg/wonjang_normal.txt";
		break;
	case CG_LINA_NORMAL:
		fileName = "/cg/lina_normal.txt";
		break;
	case CG_TOWN:
		fileName = "/cg/town.txt";
		break;
	case CG_MAEJUM:
		fileName = "/cg/maejum.txt";
		break;
	case CG_MAX:
		fileName = "/cg/opening.txt";
		break;
	}

	GameImage::load(cg, fileName);
	return _images[cg];
}

string GameImage::load(IMAGE_CG cg, string file)
{
	if (_images[cg].empty())
	{
		ifstream inFile("resource/image" + file);
		string line;
		string image = "";
		while (getline(inFile, line))
		{
			image += line + "\n";
		}
		_images[cg] = image;
	}

	return _images[cg];
}