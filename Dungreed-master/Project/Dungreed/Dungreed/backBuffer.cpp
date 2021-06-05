#include "stdafx.h"
#include "backBuffer.h"

backBuffer::backBuffer()
	: _backBufferInfo(NULL),
	_trans(false),
	_transColor(RGB(0, 0, 0))
{
}

backBuffer::~backBuffer()
{
}

HRESULT backBuffer::init(int width, int height)
{
	//이미지 정보에 뭔가 있다!? 면 해제를 해라
	if (_backBufferInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_backBufferInfo = new backBuffer_INFO;
	_backBufferInfo->resID = 0;
	_backBufferInfo->hMemDC = CreateCompatibleDC(hdc);//빈 DC영역을 생성
	_backBufferInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);//크기 만큼의 빈 비트맵 영역 생성
	_backBufferInfo->hOBit = (HBITMAP)SelectObject(_backBufferInfo->hMemDC, _backBufferInfo->hBit);
	_backBufferInfo->width = width;
	_backBufferInfo->height = height;

	if (_backBufferInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

void backBuffer::release(void)
{
	//이미지 정보가 있다면
	if (_backBufferInfo)
	{
		SelectObject(_backBufferInfo->hMemDC, _backBufferInfo->hOBit);
		DeleteObject(_backBufferInfo->hBit);
		DeleteDC(_backBufferInfo->hMemDC);

		SAFE_DELETE(_backBufferInfo);
	}
}

void backBuffer::render(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,				//복사될 영역의 DC == backDC
			destX,							//복사될 좌표 X	  == backDC의 영역의 좌표
			destY,							//복사될 좌표 Y
			_backBufferInfo->width,			//복사될 크기
			_backBufferInfo->height,		//복사될 크기
			_backBufferInfo->hMemDC,		//복사할 DC
			0, 0,							//복사해올 좌표
			_backBufferInfo->width,			//복사해올 크기
			_backBufferInfo->height,
			_transColor);					//뺄 칼라
	}
	else
	{
		//비트블릿 == 백버퍼에 있는 걸 앞쪽으로 고속복사 해주는 함수
		BitBlt(hdc, destX, destY,
			_backBufferInfo->width, _backBufferInfo->height,
			_backBufferInfo->hMemDC, 0, 0, SRCCOPY);
	}
}
