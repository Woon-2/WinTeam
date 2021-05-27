#include "FileUtility.h"

void LoadCImage(CImage& image, const TCHAR* file_name)
{
	CheckFileNameValidity(file_name);
	image.Load(file_name);
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