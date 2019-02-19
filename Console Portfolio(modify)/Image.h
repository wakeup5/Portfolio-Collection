#pragma once
#include <string>
#include <fstream>

using namespace std;

class Image
{
private:
	string _resourcePath;
	string _fileName;

	//ifstream _inFile;
	string _image;

	void loadFile();

public:
	Image();
	Image(string, string);
	~Image();

	void setFile(string);
	void setResource(string);

	string get();
	string get(string);
};

