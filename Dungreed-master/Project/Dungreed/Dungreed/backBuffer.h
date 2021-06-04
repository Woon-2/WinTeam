#pragma once
class backBuffer
{
	typedef struct tagBackBufferInfo
	{
		DWORD	resID;         //???????????????
		HDC		hMemDC;		   //핸들
		HBITMAP hBit;		   //비트맵핸들값
		HBITMAP hOBit;		   //비트맵핸들값
		float	x;
		float	y;
		int		width;
		int		height;
		BYTE	loadType;
		/*
		BYTE(unsigned char 0 ~255정수 이며 1바이트) -> WORD(BYTE가 2개 모이는 것 0 ~ 65535까지) -> DWORD(WORD가 2개 모이는 것 4바이트 정수)
		*/

		tagBackBufferInfo()
		{
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			loadType = NULL;
		}
	}backBuffer_INFO, *LPbackBuffer_INFO;

private:
	LPbackBuffer_INFO	_backBufferInfo;
	BOOL			_trans;
	COLORREF		_transColor;
public:
	backBuffer();
	~backBuffer();

	HRESULT init(int width, int height);
	void release(void);
	void render(HDC hdc, int destX, int destY);

	inline HDC getMemDC() { return _backBufferInfo->hMemDC; }
};

