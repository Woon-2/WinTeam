#pragma once
#ifndef _effect
#define _effect
#include <windows.h>
#include "FileUtility.h"
#include "Animation.h"

class Effect
{
protected:
	Image image;
	std::string old_animation_name;
	std::string cur_animation_name;

	int width;
	int height;
	POINT pos;
public:
	void Init(AnimationManager* animation_manager, POINT given_pos, int given_width, int given_height, const std::string& start_animation_name, const TCHAR* start_image_path);

	void Render(HDC scene_dc, const RECT& bit_rect) const;
	void UpdateAnimation(AnimationManager* animation_manager);
};
#endif