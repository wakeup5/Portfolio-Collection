#include "Image.h"


Image::Image()
{
	_resourcePath = "resource/image/";
}

Image::Image(string resource, string file)
{
	setResource(resource);
	setFile(file);
	loadFile();
}

Image::~Image()
{
}

void Image::setFile(string file)
{
	_fileName = file;
}

void Image::setResource(string resource)
{
	_resourcePath = resource;
}

string Image::get()
{
	return _image;
}

string Image::get(string file)
{
	setFile(file);
	loadFile();
	return get();
}

void Image::loadFile()
{
	ifstream inFile(_resourcePath + _fileName);
	string line;
	string image = "";
	while (getline(inFile, line))
	{
		image += line + "\n";
	}
	_image = image;
}