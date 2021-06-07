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

bool IsStringInt(const std::string& str)
{
	if (str.empty())
		return false;
	for (int i = 0; i < str.length(); ++i)
		if (str[i] < '0' || str[i] > '9')
			return false;
	return true;
}

const std::string GetHeadString(const std::string& line)
{
	for (int i = 0; i < line.length(); ++i)
		if (line[i] == ' ')
			return line.substr(0, i);
	return line;
}

const std::string GetRestString(const std::string& line)
{
	for (int i = 0; i < line.length(); ++i)
		if (line[i] == ' ')
			return line.substr(i+1);
	throw L"GetRestString Failed";
}

void str2Tstr(const std::string& str, TCHAR t_str[])
{
	for (int i = 0; i < str.length(); ++i)
		t_str[i] = str[i];
	t_str[str.length()] = NULL;
}