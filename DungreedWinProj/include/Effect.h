#pragma once
#ifndef _effect
#define _effect
#include <windows.h>
#include <vector>
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
	Effect(AnimationManager* animation_manager, POINT given_pos, int given_width, int given_height, const std::string& start_animation_name, const TCHAR* start_image_path);

	void Render(HDC scene_dc, const RECT& bit_rect) const;
	void UpdateAnimation(AnimationManager* animation_manager);

	friend class EffectManager;
};


typedef std::vector<Effect*> Effects;
typedef std::vector<Effect*>::iterator ViEffects;

class EffectManager {
private:
	Effects effects;
public:
	EffectManager();
	~EffectManager();
	void Insert(AnimationManager* animation_manager, POINT given_pos, int given_width, int given_height, const std::string& animation_name, const TCHAR* start_image_path);
	void Delete(AnimationManager* animation_manager, const std::string& animation_name);
	void Render(HDC scene_dc, const RECT& bit_rect) const;
	void Update(AnimationManager* animation_manager);
};

#endif