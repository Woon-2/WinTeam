#pragma once
#ifndef _scene
#define _scene
#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"

extern HWND h_wnd;

class Scene
{
private:
	Dungeon* dungeon;
	Player* player;

	int camera_x;
	int camera_y;

	int camera_x_half_range;
	int camera_y_half_range;

	void CameraUpdate(); // player À§Ä¡¿¡ µû¶ó camera_x, camera_y¸¦ ¹Ù²ãÁÜ
public:
	HRESULT Init();
	void Render();
	void Update();

	void GoNextDungeon();
	void GoPrevDungeon();
};
#endif