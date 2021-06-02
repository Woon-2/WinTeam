#pragma once
#ifndef _scene
#define _scene
#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"
#include "Camera.h"

extern HWND h_wnd;

class Scene
{
private:
	Dungeon* dungeon;
	Player* player;
	// Monster mosnters[];
	Camera* camera;

	HRESULT Init();

	void ChangeDungeon(const int dungeon_id);

public:
	Scene(const int dungeon_id);
	~Scene();

	void Render();
	void Update();

	void GoNextDungeon();
	void GoPrevDungeon();
};
#endif