#pragma once
#ifndef _dcset
#define _dcset
#include <windows.h>
#include "Uncopyable.h"

extern HWND h_wnd;
extern RECT client;

class InstantDCSet : private Uncopyable {
public:
	HDC h_dc;
	HDC buf_dc;
	HBITMAP buf_bit;
	HBITMAP old_bit;
	RECT bit_rect;

	InstantDCSet();
	InstantDCSet(const RECT& bit_rect);
	~InstantDCSet();
};

#endif