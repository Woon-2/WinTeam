#include "Scene.h"

Scene::Scene(const int dungeon_id)
{
	try {
		dungeon = new Dungeon(dungeon_id);
		player = new Player(dungeon);
		player->PlaceWithDungeonLeft(dungeon);
		// monsters = new Monster[];
		camera = new Camera(dungeon, player);
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
}

Scene::~Scene()
{
	delete player;
	delete dungeon;
	// delete monsters[];
}

HRESULT Scene::Init()
{
	player->Init(dungeon);
	//monstersInit
	camera->Init(dungeon, player);

	return S_OK;
}

void Scene::Render(HDC& buf_m_dc)
{
	player->Render(buf_m_dc);
}

void Scene::Update()
{
	// player, monster 업데이트 루틴

	camera->Update(dungeon, player);
}

void Scene::GoNextDungeon()
{
	try {
		ChangeDungeon(dungeon->next_dungeon_id);
		Init();
		player->PlaceWithDungeonLeft(dungeon);
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
}

void Scene::GoPrevDungeon()
{
	try {
		ChangeDungeon(dungeon->prev_dungeon_id);
		Init();
		player->PlaceWithDungeonRight(dungeon);
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
}

void Scene::ChangeDungeon(const int dungeon_id)
{
	delete(dungeon);
	dungeon = new Dungeon(dungeon_id);
}

void Scene::PlayerMove(HDC h_dc, const TCHAR* map_name)
{
	player->KeyMove(h_dc, map_name);
	player->Move(h_dc, map_name);
}