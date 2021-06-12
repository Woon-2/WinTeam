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
#include "InstantDCSet.h"
#include <vector>

constexpr int FILE_NAME_LEN = 200;
constexpr int IMAGE_DIGIT_IDX_MAX = 3;

class Image : public CImage
{
private:
	TCHAR image_name[FILE_NAME_LEN];

public:
	Image();
	Image(const TCHAR* file_name);
	Image(const Image& source);
	Image& operator=(const Image& source);
	~Image();

	void Load(const TCHAR* file_name);
};

class ImageContainer
{
private:
	std::map<std::string, std::vector<Image>> images;
	void Insert(const std::string& image_representative_name, const Image& image);
public:
	void Delete(const std::string& image_representative_name);
	const Image& Find(const std::string& image_representative_name, const int idx) const;
	void Insert(TCHAR* folder_path, const std::string& image_representative_name);
};

void CheckFileNameValidity(const TCHAR* file_name);
bool IsFileValid(const TCHAR* file_name);
void FlipImage(HDC scene_dc, const RECT& bit_rect, const Image* const image, int x, int y, int width, int height);
HBITMAP RotateImage(HDC scene_dc, Image* image, float angle);
void Str2Tstr(const std::string& str, TCHAR t_str[]);
void Tstr2Str(const TCHAR* t_str, std::string& str);

#endif