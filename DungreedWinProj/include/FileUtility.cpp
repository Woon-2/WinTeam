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

bool IsID(const std::string& id_string)
{
	int id_num;
	int id_num_divided_by_digit;

	if (!IsStringInt(id_string))
		return false;

	id_num = std::stoi(id_string);
	id_num_divided_by_digit = id_num / pow(10, ID_DIGIT - 1);

	if (id_num_divided_by_digit >= 1 && id_num_divided_by_digit <= 9)
		return true;
	else
		return false;
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

bool IsLineFieldWithData(const std::string& line)
{
	if (GetHeadString(line) == line)
		return false;
	else
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

TCHAR* str2Tstr(std::string str)
{
	TCHAR t_str[DEF_STR_LEN];
	for (int i = 0; i < str.length(); ++i)
		t_str[i] = str[i];
	t_str[str.length()] = NULL;
	return t_str;
}