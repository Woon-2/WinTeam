#include "InstantDCset.h"

InstantDCSet::InstantDCSet()
{
	bit_rect = client;
	h_dc = GetDC(h_wnd);
	buf_dc = CreateCompatibleDC(h_dc);
	buf_bit = CreateCompatibleBitmap(h_dc, bit_rect.right, bit_rect.bottom);
	old_bit = (HBITMAP)SelectObject(buf_dc, buf_bit);
}

InstantDCSet::InstantDCSet(const RECT& bit_rect)
{
	this->bit_rect = bit_rect;
	h_dc = GetDC(h_wnd);
	buf_dc = CreateCompatibleDC(h_dc);
	buf_bit = CreateCompatibleBitmap(h_dc, bit_rect.right, bit_rect.bottom);
	old_bit = (HBITMAP)SelectObject(buf_dc, buf_bit);
}

InstantDCSet::~InstantDCSet()
{
	SelectObject(buf_dc, old_bit);
	DeleteObject(buf_bit);
	DeleteDC(buf_dc);
	ReleaseDC(h_wnd, h_dc);
}