#include "Scene.h"

Scene::Scene()
{
	try {
		animation_manager = new AnimationManager;
		animation_manager->Insert("player_stand");
		animation_manager->Insert("player_move");

		effect_manager = new EffectManager;
		sound_manager = new SoundManager;
		sound_manager->Init();
		sound_manager->PlayBgm("sound\\bgm1.mp3");
		sound_manager->InsertEffectSound("sound\\dash.mp3");

		dungeon = new Dungeon;
		player = new Player(dungeon, animation_manager);
		monster_manager = new MonsterManager(dungeon, animation_manager);
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
		sound_manager = new SoundManager;
		sound_manager->Init();
		sound_manager->PlayBgm("sound\\bgm1.mp3");
		sound_manager->InsertEffectSound("sound\\dash.mp3");

		dungeon = new Dungeon(dungeon_id);
		player = new Player(dungeon, animation_manager);
		monster_manager = new MonsterManager(dungeon, animation_manager);
		camera = new Camera(dungeon, player);
		crosshair = new Crosshair(camera);
		weapon = new Weapon(camera, player, crosshair);

		player->PlaceWithDungeonLeft(dungeon);
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
	delete monster_manager;
	delete effect_manager;
	delete sound_manager;
}

HRESULT Scene::Init()
{
	player->Init(dungeon, animation_manager);
	monster_manager->Init(dungeon, animation_manager);
	camera->Init(dungeon, player);
	crosshair->Init(camera);
	weapon->Init(camera, player, crosshair);

	update_cnt = 0;

	return S_OK;
}

void Scene::Render() const
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->Render(dc_set.buf_dc, dc_set.bit_rect);
	player->Render(dc_set.buf_dc, dc_set.bit_rect);
	monster_manager->Render(dc_set.buf_dc, dc_set.bit_rect);
	crosshair->Render(dc_set.buf_dc, dc_set.bit_rect);
	weapon->Render(dc_set.buf_dc, dc_set.bit_rect);
	effect_manager->Render(dc_set.buf_dc, dc_set.bit_rect);

	DrawBuffer(dc_set.buf_dc, camera->Rect());
}

void Scene::Update()
{
	// player, monster 업데이트 루틴
	player->Update(dungeon, crosshair, animation_manager, sound_manager);
	monster_manager->Update(dungeon, player, animation_manager);
	camera->Update(dungeon, player);
	crosshair->Update(camera);
	weapon->Update(player, crosshair);
	effect_manager->Update(animation_manager);
	/*HitUpdate();*/
	DungeonChangeProc();
}

void Scene::DungeonChangeProc()
{
	if (player->IsOut_Right(dungeon))
		if (monster_manager->AreMonsterAllDied())
			GoNextDungeon();
		else
			player->NoOut(dungeon);
	else if (player->IsOut_Left(dungeon))
		if (monster_manager->AreMonsterAllDied())
			GoPrevDungeon();
		else
			player->NoOut(dungeon);

	if (update_cnt++ % 100 == 0)
		monster_manager->Appear(5);
}

//void Scene::HitUpdate()
//{
//	for (Monster* monster : monsters) {
//		HitScan(dynamic_cast<Character*>(player), dynamic_cast<Character*>(monster), weapon->AtkRect());
//		HitScan(dynamic_cast<Character*>(monster), dynamic_cast<Character*>(player), monster->AtkRect());
//	}
//}

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
		throw L"ChangeDungeon_dungeon_id was 0";
	delete(dungeon);
	dungeon = new Dungeon(dungeon_id);
}