#pragma once
#ifndef _animation
#define _animation
#include "FileUtility.h"
#include "DataBase.h"
#include <map>
#include <vector>

class Animation {
private:
	std::string name;
	int frame_per_cnt;
	int frame = 0;
	int cnt = 1;
	int end_cnt;
	BOOL will_loop;
	BOOL is_playing = FALSE;

public:
	Animation(const std::string& name, const int frame_per_cnt, const int end_cnt, const BOOL will_loop) : name{ name }, frame_per_cnt{ frame_per_cnt }, end_cnt{ end_cnt }, will_loop{ will_loop } {}
	void Update();

	friend class AnimationManager;
};

class AnimationManager {
private:
	std::map<const std::string, Animation> animations;
	ImageContainer images;

	TCHAR folder_path[FILE_NAME_LEN] = L"animation";
	TCHAR animation_name_tstr[FILE_NAME_LEN];
	std::string animation_name_cmp;
	int animation_frame_per_cnt;
	int animation_cnt;
	int animation_end_cnt;
	BOOL animation_will_loop;
	std::shared_ptr<DB::DataBase> BuildDB();
public:
	void Insert(const std::string& animation_name);
	void Delete(const std::string& animation_name);
	void Play(const std::string& animation_name);
	void Stop(const std::string& animation_name);
	void Update();
	BOOL IsEnd(const std::string& animation_name);
	const Image& GetImage(const std::string& animation_name) const;
};

#endif