#include "FileUtility.h"

Image::Image()
{
	this->CImage::CImage();
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
	CImage::Destroy();
	Load(source.image_name);
	return *this;
}

Image::~Image()
{
	this->Destroy();
	this->CImage::~CImage();
}


void ImageContainer::Insert(TCHAR* folder_path, const std::string& image_representative_name)
{
	int i = 0;
	TCHAR image_name[FILE_NAME_LEN];
	TCHAR idx[IMAGE_DIGIT_IDX_MAX];
	TCHAR image_path[FILE_NAME_LEN];

	while (++i) {
		lstrcpy(image_path, folder_path);
		lstrcat(image_path, L"/");

		Str2Tstr(image_representative_name, image_name);
		_itot(i, idx, 10);
		lstrcat(image_name, idx);

		lstrcat(image_path, image_name);
		lstrcat(image_path, L".png");

		if (!IsFileValid(image_path))
			break;

		Insert(image_representative_name, Image(image_path));
	}
}

void ImageContainer::Insert(const std::string& image_representative_name, const Image& image)
{
	auto destination = images.find(image_representative_name);

	if (images.find(image_representative_name) == images.end()) {
		std::vector<Image> tmp{ image };
		images.insert({ image_representative_name, tmp });
	}
	else
		destination->second.push_back(image);
}

void ImageContainer::Delete(const std::string& image_representative_name)
{
	images.erase(image_representative_name);
}

const Image& ImageContainer::Find(const std::string& image_representative_name, const int idx) const
{
	return images.find(image_representative_name)->second.at(idx);
}



HBITMAP FlipImage(HDC scene_dc, const Image* const _image)
{
	int image_width = _image->GetWidth();
	int image_height = _image->GetHeight();

	HDC dest_dc = CreateCompatibleDC(scene_dc);
	HBITMAP hbitmap = CreateCompatibleBitmap(scene_dc, image_width, image_height);
	HBITMAP hbm_old_dest = (HBITMAP)SelectObject(dest_dc, hbitmap);

	HDC source_dc = CreateCompatibleDC(scene_dc);
	HBITMAP hbm_result = CreateCompatibleBitmap(scene_dc, image_width, image_height);
	HBITMAP hbm_old_source = (HBITMAP)SelectObject(source_dc, hbm_result);
	/*
	HBRUSH hbr_back = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hbr_old = (HBRUSH)SelectObject(dest_dc, hbr_back);
	PatBlt(dest_dc, 0, 0, image_width, image_height, PATCOPY);
	DeleteObject(SelectObject(dest_dc, hbr_old));
	*/
	_image->Draw(dest_dc, 0, 0, image_width, image_height, 0, 0, image_width, image_height);
	StretchBlt(source_dc, image_width, 0, -image_width, image_height, dest_dc, 0, 0, image_width, image_height, SRCCOPY);
	//TransparentBlt(scene_dc, x, y, width, height, source_dc, 0, 0, image_width, image_height, RGB(0, 0, 0));	// RGB(34, 32, 52)

	SelectObject(source_dc, hbm_old_source);
	DeleteDC(source_dc);
	DeleteObject(hbitmap);
	SelectObject(dest_dc, hbm_old_dest);
	DeleteDC(dest_dc);
	//DeleteObject(hbm_result);
	return hbm_result;
}

void DrawFlip(HDC scene_dc, const RECT& bit_rect, const Image* _image, POINT pos, int width, int height)
{
	HBITMAP hbm_flip = FlipImage(scene_dc, _image);
	Image* rotate_image = new Image();
	rotate_image->Attach(hbm_flip);
	rotate_image->SetTransparentColor(RGB(0, 0, 0));

	rotate_image->Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, rotate_image->GetWidth(), rotate_image->GetHeight());

	DeleteObject(hbm_flip);
	delete rotate_image;
}

HBITMAP RotateImage(HDC scene_dc, const Image* image, float angle)
{
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();

	HDC source_dc = CreateCompatibleDC(scene_dc);
	HBITMAP hbm_source = CreateCompatibleBitmap(scene_dc, image_width, image_height);

	HDC dest_dc = CreateCompatibleDC(scene_dc);
	HBITMAP hbm_result = CreateCompatibleBitmap(scene_dc, image_width, image_height);

	HBITMAP hbm_old_source = (HBITMAP)SelectObject(source_dc, hbm_source);
	HBITMAP hbm_old_dest = (HBITMAP)SelectObject(dest_dc, hbm_result);

	image->Draw(source_dc, 0, 0, image_width, image_height, 0, 0, image_width, image_height);

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

	BitBlt(dest_dc, -(image_width / 2), -(image_height / 2), image_width, image_height, source_dc, 0, 0, SRCCOPY);

	SelectObject(source_dc, hbm_old_source);
	SelectObject(dest_dc, hbm_old_dest);
	DeleteDC(source_dc);
	DeleteObject(hbm_source);
	DeleteDC(dest_dc);
	//DeleteObject(hbm_result);

	return hbm_result;
}

void RedImage(HDC scene_dc, const RECT& bit_rect, const Image* _image, POINT pos, int width, int height, BOOL flip)
{
	Image* image = new Image((*_image));

	int R, G, B;
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();

	BYTE* byteptr = (BYTE*)image->GetBits();
	int pitch = image->GetPitch();

	for (int i = 0; i < image_width; i++) {
		for (int j = 0; j < image_height; j++) {
			R = *(byteptr + pitch * j + 4 * i);
			G = *(byteptr + pitch * j + 4 * i + 1);
			B = *(byteptr + pitch * j + 4 * i + 2);

			if (R != 0 || G != 0 || B != 0) {
				R = 255;
				G = 0;
				B = 0;
			}
			*(byteptr + pitch * j + 4 * i) = B;
			*(byteptr + pitch * j + 4 * i + 1) = G;
			*(byteptr + pitch * j + 4 * i + 2) = R;
		}
	}

	if (!flip) {
		image->AlphaBlend(scene_dc, pos.x, pos.y, width, height, 0, 0, image_width, image_height, 0xcc, AC_SRC_OVER);	// 0xcc 값 조정해 투명도 조절 가능 0xff면 불투명
	}
	else {
		BLENDFUNCTION bf;

		HDC hbm_dc = CreateCompatibleDC(scene_dc);
		HBITMAP hbm_flip_red = FlipImage(scene_dc, image);
		HBITMAP old_hbm = (HBITMAP)SelectObject(hbm_dc, hbm_flip_red);

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha
		bf.SourceConstantAlpha = 0xcc;  // opaque (disable constant alpha)

		TransparentBlt(hbm_dc, 0, 0, image_width, image_height, hbm_dc, 0, 0, image_width, image_height, RGB(0, 0, 0));
		AlphaBlend(scene_dc, pos.x, pos.y, width, height, hbm_dc, 0, 0, image_width, image_height, bf);
		SelectObject(hbm_dc, old_hbm);
		DeleteDC(hbm_dc);
		DeleteObject(hbm_flip_red);
	}

	delete image;
}


void CheckFileNameValidity(const TCHAR* file_name)
{
	if (!IsFileValid(file_name)) {
		TCHAR error_message[200];
		wsprintf(error_message, L"File \"%s\" has not found", file_name);
		throw error_message;
	}
}

bool IsFileValid(const TCHAR* file_name)
{
	std::ifstream in{ file_name };
	if (in)
		return true;
	return false;
}

void Str2Tstr(const std::string& str, TCHAR t_str[])
{
	for (int i = 0; i < str.length(); ++i)
		t_str[i] = str[i];
	t_str[str.length()] = NULL;
}

void Tstr2Str(const TCHAR* t_str, std::string& str)
{
	str.clear();
	for (int i = 0; i < lstrlen(t_str); ++i)
		str += t_str[i];
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
	return degree;
}