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

void FlipImage(HDC scene_dc, const RECT& bit_rect, Image* image, int x, int y, int width, int height)
{
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();

	HDC dest_dc = CreateCompatibleDC(scene_dc);
	HBITMAP hbitmap = CreateCompatibleBitmap(scene_dc, image_width, image_height);
	HBITMAP hbm_old_dest = (HBITMAP)SelectObject(dest_dc, hbitmap);

	HDC source_dc = CreateCompatibleDC(scene_dc);
	HBITMAP hbm_result = CreateCompatibleBitmap(scene_dc, image_width, image_height);
	HBITMAP hbm_old_source = (HBITMAP)SelectObject(source_dc, hbm_result);

	image->Draw(dest_dc, 0, 0, image_width, image_height, 0, 0, image_width, image_height);
	StretchBlt(source_dc, image_width, 0, -image_width, image_height, dest_dc, 0, 0, image_width, image_height, SRCCOPY);
	TransparentBlt(scene_dc, x, y, width, height, source_dc, 0, 0, image_width, image_height, RGB(0, 0, 0));	// RGB(34, 32, 52)

	SelectObject(source_dc, hbm_old_source);
	DeleteObject(source_dc);
	SelectObject(dest_dc, hbm_old_dest);
	DeleteObject(dest_dc);
}