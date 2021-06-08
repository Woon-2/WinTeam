#include "FileUtility.h"

void Image::Load(const TCHAR* file_name)
{
	CheckFileNameValidity(file_name);
	this->CImage::Load(file_name);
	lstrcpy(image_name, file_name);
}

Image::Image(const TCHAR* file_name)
{
	Load(file_name);
}

Image::Image(const Image& source)
{
	Load(source.image_name);
}

Image& Image::operator=(const Image& source)
{
	if (this == &source)
		return *this;
	Load(source.image_name);
	return *this;
}

Image::~Image()
{
	this->Destroy();
	this->CImage::~CImage();
}







void CheckFileNameValidity(const TCHAR* file_name)
{
	std::ifstream in{ file_name };
	if (!in) {
		TCHAR error_message[200];
		wsprintf(error_message, L"File \"%s\" has not found", file_name);
		throw error_message;
	}
}