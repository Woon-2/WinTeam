#include "FileUtility.h"

Image::Image()
{
}

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

HBITMAP RotateImage(HDC scene_dc, Image* image, float angle)
{
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();

	HDC dest_dc = CreateCompatibleDC(scene_dc);
	HBITMAP hbm_result = CreateCompatibleBitmap(scene_dc, image_width, image_height);
	HBITMAP hbm_old_dest = (HBITMAP)SelectObject(dest_dc, hbm_result);

	float cosine = (float)cos(angle);
	float sine = (float)sin(angle);

	SetGraphicsMode(dest_dc, GM_ADVANCED);

	XFORM xform; // 방정식을 표현하는 3행3열의 행렬 선언
	xform.eM11 = cosine; // 1행 1열 성분 설정 (회전성분)
	xform.eM12 = sine; // 1행 2열 성분 설정 (회전성분)
	xform.eM21 = -sine; // 2행 1열 성분 설정 (회전성분)
	xform.eM22 = cosine; // 2행 2열 성분 설정 (회전성분)
	xform.eDx = (FLOAT)image_width / 2.0f; // 3행 1열 성분 설정 (X축 이동 성분)
	xform.eDy = (FLOAT)image_height / 2.0f; // 3행 2열 성분 설정 (Y축 이동 성분)

	SetWorldTransform(dest_dc, &xform);

	image->Draw(dest_dc, -(image_width / 2), -(image_height / 2), image_width, image_height, 0, 0, image_width, image_height);
	
	SelectObject(dest_dc, hbm_old_dest);
	DeleteObject(dest_dc);
	
	return hbm_result;
}

float Degree(const POINT& point1, const POINT& point2)
{
	float degree;
	if (point1.x < point2.x) {
		degree = atan2(point2.y - point1.y, point2.x - point1.x);
	}
	else {
		degree = -atan2(point1.y - point2.y, point1.x - point2.x);
	}
	return -degree;
}