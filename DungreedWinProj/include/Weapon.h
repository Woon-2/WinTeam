#pragma once
#ifndef _weapon
#define _weapon
#include <windows.h>
#include <math.h>
#include "Crosshair.h"
#include "FileUtility.h"
#include "Player.h"
#include "Crosshair.h"
#include "Animation.h"
#include "Camera.h"
#include <string>

class Player;
class Camera;
class Crosshair;

class Weapon
{
private:
	const Image* image;
	const Image* start_image;

	Animation animation;
	std::string animation_name;

	POINT pos;
	int width;
	int height;
	float angle;
	BOOL looking_direction;

	void UpdateAnimation(AnimationManager* animation_manager);

public:  
	Weapon(const Camera* camera, const Player* player, const Crosshair* crosshair, AnimationManager* animation_manager);
	~Weapon();
	void Init(const Camera* camera, const Player* player, const Crosshair* crosshair, AnimationManager* animation_manager);
	void Update(const Player* player, const Crosshair* crosshair, AnimationManager* animation_manager);
	void Render(HDC scene_dc, const RECT& bit_rect);
	void StartAttack();
	bool IsAttackFinished();
};
#endif