#include "FileUtility.h"

void Image::Load(const TCHAR* file_name)
{
	CheckFileNameValidity(file_name);
	this->CImage::Load(file_name);
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
	std::string str_repaired;
	int value = std::stoi(str);
	str_repaired = std::to_string(value);
	if (str_repaired == str)
		return true;
	else
		return false;
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
			return line.substr(0, i - 1);
	return line;
}

const std::string GetRestString(const std::string& line)
{
	for (int i = 0; i < line.length(); ++i)
		if (line[i] == ' ')
			return line.substr(i+1);
	throw L"GetRestString Failed";
}