#pragma once
#ifndef _character
#define _character
#include <windows.h>
#include "Dungeon.h"

class Character
{
protected:
	int width;
	int height;
	POINT pos;
public:
	Character(const Dungeon* dungeon);
	virtual void Render() = 0;
};
#endif
