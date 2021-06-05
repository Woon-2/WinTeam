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

class Image : public CImage
{
public:
	void Load(const TCHAR* file_name);
};

void CheckFileNameValidity(const TCHAR* file_name);
bool IsID(const std::string& id_string);
bool IsStringInt(const std::string& str);
bool IsLineFieldWithData(const std::string& line);
const std::string GetHeadString(const std::string& line);
const std::string GetRestString(const std::string& line);
#endif