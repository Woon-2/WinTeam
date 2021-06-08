#include "Scene.h"

Scene::Scene(const int dungeon_id)
{
	try {
		dungeon = new Dungeon(dungeon_id);
		player = new Player(dungeon);
		player->PlaceWithDungeonLeft(dungeon);
		// monsters = new Monster[];
		camera = new Camera(dungeon, player);
		crosshair = new Crosshair(camera->Rect(), dungeon, client);
		weapon = new Weapon(dungeon, &player->pos, &player->width, &player->height, &crosshair->GetPos());
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
	crosshair->Update(camera->Rect(), dungeon, client);	
	weapon->Update(&player->pos, &player->width, &player->height, &crosshair->GetPos());

	dungeon->Render(dc_set.buf_dc, dc_set.bit_rect);
	player->Render(dc_set.buf_dc, dc_set.bit_rect, crosshair->LookingDirection(&player->pos));
	crosshair->Render(dc_set.buf_dc, dc_set.bit_rect);
	weapon->Render(dc_set.buf_dc, dc_set.bit_rect, crosshair->LookingDirection(&player->pos));

	DrawBuffer(dc_set.buf_dc, camera->Rect());
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
	if (!dungeon_id)
		throw L"Loaded Dungeon ID was 0";
	delete(dungeon);
	dungeon = new Dungeon(dungeon_id);
}