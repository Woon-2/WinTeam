#include "Scene.h"

Scene::Scene()
{
	try {
		animation_manager = new AnimationManager;
		animation_manager->Insert("player_stand");
		animation_manager->Insert("player_move");

		effect_manager = new EffectManager;

		dungeon = new Dungeon;
		player = new Player(dungeon, animation_manager);
		// monsters = new Monster[];
		camera = new Camera(dungeon, player);
		crosshair = new Crosshair(camera);
		weapon = new Weapon(camera, player, crosshair);

		player->PlaceWithDungeonLeft(dungeon);
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
}

Scene::Scene(const int dungeon_id)
{
	try {
		animation_manager = new AnimationManager;
		animation_manager->Insert("player_stand");
		animation_manager->Insert("player_move");
		effect_manager = new EffectManager;

		dungeon = new Dungeon(dungeon_id);
		player = new Player(dungeon, animation_manager);
		player->PlaceWithDungeonLeft(dungeon);
		// monsters = new Monster[];
		camera = new Camera(dungeon, player);
		crosshair = new Crosshair(camera);
		weapon = new Weapon(camera, player, crosshair);
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
}

Scene::~Scene()
{
	delete player;
	delete dungeon;
	delete camera;
	delete crosshair;
	delete weapon;
	delete animation_manager;
	delete effect_manager;

	// delete monsters[];
}

HRESULT Scene::Init()
{
	player->Init(dungeon, animation_manager);
	//monstersInit
	camera->Init(dungeon, player);
	crosshair->Init(camera);
	weapon->Init(camera, player, crosshair);

	return S_OK;
}

void Scene::Render() const
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->Render(dc_set.buf_dc, dc_set.bit_rect);
	player->Render(dc_set.buf_dc, dc_set.bit_rect);
	crosshair->Render(dc_set.buf_dc, dc_set.bit_rect);
	weapon->Render(dc_set.buf_dc, dc_set.bit_rect);
	effect_manager->Render(dc_set.buf_dc, dc_set.bit_rect);

	DrawBuffer(dc_set.buf_dc, camera->Rect());
}

void Scene::Update()
{
	// player, monster 업데이트 루틴
	if (player->IsOut_Right(dungeon))
		GoNextDungeon();
	else if (player->IsOut_Left(dungeon))
		GoPrevDungeon();

	animation_manager->Update();
	player->Update(dungeon, crosshair, animation_manager);
	camera->Update(dungeon, player);
	crosshair->Update(camera);
	weapon->Update(player, crosshair);
	effect_manager->Update(animation_manager);
}

void Scene::GoNextDungeon()
{
	try {
		ChangeDungeon(dungeon->next_dungeon_id);
		Init();
		player->PlaceWithDungeonLeft(dungeon);
		// 테스트
		//effect_manager->Insert(animation_manager, dungeon->left_start_pos, 100, 100, "player_move", L"animation/player_move1.png");
		//effect_manager->Insert(animation_manager, dungeon->left_start_pos, 100, 100, "portal_create", L"animation/portal_create1.png");
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
		throw L"ChangeDungeon_dungeon_id was 0";
	delete(dungeon);
	dungeon = new Dungeon(dungeon_id);
}