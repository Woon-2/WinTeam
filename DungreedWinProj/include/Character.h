#pragma once
#ifndef _character
#define _character
#include <windows.h>

class Character
{
private:
	int width;
	int height;
	POINT pos;
public:
	inline void SetPos(POINT pos) { this->pos = pos; }
	inline const POINT GetPos() const { return pos; }
};
#endif
