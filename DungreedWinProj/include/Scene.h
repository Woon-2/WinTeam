#pragma once
#ifndef _scene
#define _scene
#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"
#include "Camera.h"
#include "Uncopyable.h"
#include "Weapon.h"
#include "Crosshair.h"
#include <map>
#include <vector>
#include "FileUtility.h"
#include "Animation.h"

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
	AnimationManager* animation_manager;

	void GoNextDungeon();
	void GoPrevDungeon();
	void ChangeDungeon(const int dungeon_id);
	HRESULT Init();

public:
	Scene();
	Scene(const int dungeon_id);
	~Scene();

	void Render() const;
	void Update();
};
#endif