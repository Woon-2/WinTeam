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


	void ChangeDungeon(const int dungeon_id);

public:
	Scene(const int dungeon_id);
	~Scene();

	HRESULT Init();
	void Render(HDC&);
	void Update();

	void GoNextDungeon();
	void GoPrevDungeon();

	void PlayerMove(HDC, const TCHAR*);
};
#endif