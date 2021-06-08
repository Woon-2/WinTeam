#pragma once
#ifndef _fileutility
#define _fileutility
#include <fstream>
#include <windows.h>
#include <atlimage.h>
#include <tchar.h>
#include <string>
#include <math.h>
#include <map>
#include "Uncopyable.h"

constexpr int FILE_NAME_LEN = 200;

class Image : public CImage
{
private:
	TCHAR image_name[FILE_NAME_LEN];

public:
	Image(const TCHAR* file_name);
	Image(const Image& source);
	Image& operator=(const Image& source);
	~Image();

	void Load(const TCHAR* file_name);
};

void CheckFileNameValidity(const TCHAR* file_name);

#endif