#include "Scene.h"

HRESULT Scene::Init()
{
	// 플레이어 위치 초기화
	// player.x = dungeon->start_pos.x;
	// player.y = dungeon->start_pos.y;

	// 몬스터 위치 초기화 (맵마다 데이터 가져와서 초기)

	// 카메라 위치 초기화
	CameraPosition();

	return S_OK;
}

void Scene::Render()
{

}

void Scene::Update()
{

}

void Scene::GoNextDungeon()
{
	dungeon = dungeon->next;
	Init();
}

void Scene::GoPrevDungeon()
{
	dungeon = dungeon->prev;
	Init();
}

void Scene::CameraUpdate()
{

}

void Scene::CameraPosition()
{
	if (player.x < camera_x_half_range) {
		camera_x = camera_x_half_range;
	}
	else if (player.x > dungeon->dungeon_width - camera_x_half_range) {
		camera_x = dungeon->dungeon_width - camera_x_half_range;
	}
	else {
		camera_x = player.x;
	}

	if (player.y < camera_y_half_range) {
		camera_y = camera_y_half_range;
	}
	else if (player.y > dungeon->dungeon_height - camera_y_half_range) {
		camera_y = dungeon->dungeon_height - camera_y_half_range;
	}
	else {
		camera_y = player.y;
	}
}