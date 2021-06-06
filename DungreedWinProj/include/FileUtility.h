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

const int FILE_NAME_LEN = 200;
const int DEF_STR_LEN = 200;



class IFStream : private Uncopyable, public std::ifstream
{
public:
	TCHAR file_name[FILE_NAME_LEN];
	IFStream() = default;
	IFStream(const TCHAR* file_name) : std::ifstream(file_name) { lstrcpy(this->file_name, file_name); }
};



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
bool IsStringInt(const std::string& str);
const std::string GetHeadString(const std::string& line);
const std::string GetRestString(const std::string& line);
void str2Tstr(TCHAR t_str[], const std::string& str);
#endif