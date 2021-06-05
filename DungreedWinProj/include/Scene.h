#pragma once
#ifndef _scene
#define _scene
#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"
#include "Camera.h"
#include "Uncopyable.h"
//
#include <iostream>
//

extern HWND h_wnd;
extern void DrawBuffer(HDC instant_dc, const RECT& rect);

class Scene : private Uncopyable
{
private:

	Dungeon* dungeon;
	Player* player;
	// Monster mosnters[];
	Camera* camera;

	void ChangeDungeon(const int dungeon_id);
	HRESULT Init();

public:
	Scene(const int dungeon_id);
	~Scene();

	void Render();
	void Update();

	void GoNextDungeon();
	void GoPrevDungeon();

	void test();
};
#endif