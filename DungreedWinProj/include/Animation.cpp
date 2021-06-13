#include "Animation.h"

Animation::Animation()
{
}
Animation::~Animation()
{
}

void Animation::Update()
{
	// 프레임 오버플로우가 일어나도 괜찮은가?
	if (is_playing) {
		if (++frame % frame_per_cnt == 0)
			if (will_loop) {
				if (++cnt > end_cnt)
					cnt = 1;
			}
			else {
				if (cnt < end_cnt)
					++cnt;
			}
	}
}

void Animation::Play()
{
	is_playing = TRUE;
}

void Animation::Stop()
{
	is_playing = FALSE;
	cnt = 1;
}

const Image& Animation::GetImage(AnimationManager* animation_manager) const
{
	return animation_manager->images.Find(name, cnt - 1);
}

void Animation::LoadAnimation(AnimationManager* animation_manager, const std::string& animation_name)
{
	Animation animation = animation_manager->animations.find(animation_name)->second;
	name = animation_name;
	frame_per_cnt = animation.frame_per_cnt;
	frame = 0;
	cnt = 1;
	end_cnt = animation.end_cnt;
	will_loop = animation.will_loop;
	is_playing = FALSE;
}

BOOL Animation::IsEnd()
{
	if (cnt == end_cnt) {
		return TRUE;
	}
	return FALSE;
}


void AnimationManager::Insert(const std::string& animation_name)
{
	auto db = BuildDB();

	while (true) {
		db->Load();
		Tstr2Str(animation_name_tstr, animation_name_cmp);
		if (animation_name == animation_name_cmp)
			break;
	}

	images.Insert(folder_path, animation_name);

	animations.insert({ animation_name, {animation_name, animation_frame_per_cnt, animation_end_cnt, animation_will_loop} });
}

std::shared_ptr<DB::DataBase> AnimationManager::BuildDB()
{
	auto db = std::shared_ptr<DB::DataBase>(new DB::DataBase(L"AnimationData.txt"));

	db->RegisterField("animation_name", &animation_name_tstr);
	db->RegisterField("animation_frame_per_cnt", &animation_frame_per_cnt);
	db->RegisterField("animation_end_cnt", &animation_end_cnt);
	db->RegisterField("animation_will_loop", &animation_will_loop);

	return db;
}

void AnimationManager::Delete(const std::string& animation_name)
{
	animations.erase(animation_name);
	images.Delete(animation_name);
}

/*
void AnimationManager::Update()
{
	for (auto& animation : animations)
		animation.second.Update();
}

void AnimationManager::Play(const std::string& animation_name)
{
	animations.find(animation_name)->second.is_playing = TRUE;
}

void AnimationManager::Stop(const std::string& animation_name)
{
	Animation& animation = animations.find(animation_name)->second;
	animation.is_playing = FALSE;
	animation.cnt = 1;
}

const Image& AnimationManager::GetImage(const std::string& animation_name) const
{
	return images.Find(animation_name, animations.find(animation_name)->second.cnt - 1);
}
*/


