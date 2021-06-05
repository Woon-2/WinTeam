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

void Scene::Render()
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });
	camera->Update(dungeon, player);

	dungeon->Render(dc_set.buf_dc, dc_set.bit_rect);
	player->Render(dc_set.buf_dc, dc_set.bit_rect);

	//DrawBuffer(dc_set.buf_dc, dc_set.bit_rect);
	DrawBuffer(dc_set.buf_dc, camera->Rect());

	test();
}

void Scene::Update()
{
	// player, monster 업데이트 루틴
	player->KeyProc(dungeon);
	player->ForceGravity(dungeon);
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

void Scene::test()
{
	RECT client = camera->Rect();
	std::cout << "(" << player->pos.x << ", " << player->pos.y << ") (" << camera->pos.x << ", " << camera->pos.y << ")\n";
	std::cout << "LT(" << client.left << ", " << client.top << "), RB(" << client.right << ", " << client.bottom << ")\n";
}