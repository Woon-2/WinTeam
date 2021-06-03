#pragma once
#ifndef _player
#define _player
#include "Character.h"

class Player : public Character
{
private:
	
public:
	void KeyMove(HDC, const TCHAR*);
};
#endif

