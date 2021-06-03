#include "Scene.h"

HRESULT Scene::Init()
{
	// 플레이어 위치 초기화
	delete player;
	player = new Player;			// 후에 생성자 생기면 수정 생성자에 들어갈 정보들은 일단 dungeon에 있음
	player->Init();
	// 몬스터 위치 초기화
	
	// 카메라 위치 초기화
	//CameraUpdate();

	return S_OK;
}

void Scene::Render(HDC& buf_m_dc)
{
	player->Render(buf_m_dc);
}

void Scene::Update()
{

}

void Scene::GoNextDungeon()
{
	try {
		int next_dungeon_id = dungeon->next_dungeon_id;
		delete(dungeon);
		dungeon = new Dungeon(next_dungeon_id, camera_x_half_range, camera_y_half_range);
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
	Init();
}

void Scene::GoPrevDungeon()
{
	try {
		int prev_dungeon_id = dungeon->prev_dungeon_id;
		delete(dungeon);
		dungeon = new Dungeon(prev_dungeon_id, camera_x_half_range, camera_y_half_range);
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
	Init();
}

void Scene::CameraUpdate()
{
	POINT player_pos = player->GetPos();

	if (player_pos.x < camera_x_half_range) {
		camera_x = camera_x_half_range;
	}
	else if (player_pos.x > dungeon->dungeon_width - camera_x_half_range) {
		camera_x = dungeon->dungeon_width - camera_x_half_range;
	}
	else {
		camera_x = player_pos.x;
	}

	if (player_pos.y < camera_y_half_range) {
		camera_y = camera_y_half_range;
	}
	else if (player_pos.y > dungeon->dungeon_height - camera_y_half_range) {
		camera_y = dungeon->dungeon_height - camera_y_half_range;
	}
	else {
		camera_y = player_pos.y;
	}
}

void Scene::PlayerMove(HDC h_dc, const TCHAR* map_name)
{
	player->KeyMove(h_dc, map_name);
	player->Move(h_dc, map_name);
}