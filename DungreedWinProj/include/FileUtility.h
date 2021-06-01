#pragma once
#ifndef _fileutility
#define _fileutility
#include <fstream>
#include <windows.h>
#include <atlimage.h>
#include <tchar.h>

class Image : public CImage
{
public:
	void Load(const TCHAR* file_name);
};

void CheckFileNameValidity(const TCHAR* file_name);
#endif