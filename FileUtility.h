#pragma once
#ifndef _fileutility
#define _fileutility
#include <fstream>
#include <windows.h>
#include <atlimage.h>
#include <tchar.h>

void LoadCImage(CImage& image, const TCHAR* file_name);
void CheckFileNameValidity(const TCHAR* file_name);
#endif