#pragma once
#include "Dungeon.h"

class Scene
{
private:
	Dungeon* dungeon;
	//Player* player;
	//Monster monter[];

	int camera_x;
	int camera_y;

	const int camera_x_half_range;
	const int camera_y_half_range;

	void CameraUpdate();
	void CameraPosition();	// player À§Ä¡¿¡ µû¶ó camera_x, camera_y¸¦ ¹Ù²ãÁÜ
public:
	HRESULT Init();
	void Render();
	void Update();

	void GoNextDungeon();
	void GoPrevDungeon();
};