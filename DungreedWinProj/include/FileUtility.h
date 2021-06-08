#pragma once
#ifndef _fileutility
#define _fileutility
#include <fstream>
#include <windows.h>
#include <atlimage.h>
#include <tchar.h>
#include <string>
#include <math.h>

const int ID_DIGIT = 7;
const int FILE_NAME_LEN = 200;
const int DEF_STR_LEN = 200;

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
bool IsID(const std::string& id_string);
bool IsStringInt(const std::string& str);
bool IsLineFieldWithData(const std::string& line);
const std::string GetHeadString(const std::string& line);
const std::string GetRestString(const std::string& line);
TCHAR* str2Tstr(std::string str);
#endif