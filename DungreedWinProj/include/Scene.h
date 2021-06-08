#pragma once
#ifndef _scene
#define _scene
#include "Dungeon.h"
#include "Player.h"
#include "Weapon.h"
#include "Monster.h"
#include "Camera.h"
#include "Crosshair.h"
#include "Uncopyable.h"

extern HWND h_wnd;
extern void DrawBuffer(HDC instant_dc, const RECT& rect);

class Scene : private Uncopyable
{
private:

	Dungeon* dungeon;
	Player* player;
	Weapon* weapon;
	// Monster mosnters[];
	Camera* camera;
	Crosshair* crosshair;
	void ChangeDungeon(const int dungeon_id);
	HRESULT Init();

public:
	Scene(const int dungeon_id);
	~Scene();

	void Render();
	void Update();

	void GoNextDungeon();
	void GoPrevDungeon();
};
#endif