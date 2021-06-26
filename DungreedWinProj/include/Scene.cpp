#include "Scene.h"

Scene::Scene()
{
	try {
		animation_manager = new AnimationManager;
		LoadPlayerAniamtion();

		effect_manager = new EffectManager;
		LoadBattleEffect();

		sound_manager = new SoundManager;
		sound_manager->Init();
		sound_manager->PlayBgm("sound\\bgm1.mp3");
		LoadBattleSound();

		dungeon = new Dungeon;
		player = new Player(dungeon, animation_manager);
		monster_manager = new MonsterManager(dungeon, animation_manager);
		missile_manager = new MissileManager;
		camera = new Camera(dungeon, player);
		crosshair = new Crosshair(camera);
		weapon = new Weapon(camera, player, crosshair, animation_manager);

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
		LoadPlayerAniamtion();

		effect_manager = new EffectManager;
		LoadBattleEffect();
		sound_manager = new SoundManager;

		sound_manager->Init();
		sound_manager->PlayBgm("sound\\bgm1.mp3");
		LoadBattleSound();

		dungeon = new Dungeon(dungeon_id);
		player = new Player(dungeon, animation_manager);
		monster_manager = new MonsterManager(dungeon, animation_manager);
		missile_manager = new MissileManager;
		camera = new Camera(dungeon, player);
		crosshair = new Crosshair(camera);
		weapon = new Weapon(camera, player, crosshair, animation_manager);

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
	delete missile_manager;
}

HRESULT Scene::Init()
{
	player->Init(dungeon, animation_manager);
	monster_manager->Init(dungeon, animation_manager);
	missile_manager->Init();
	camera->Init(dungeon, player);
	crosshair->Init(camera);
	weapon->Init(camera, player, crosshair, animation_manager);

	update_cnt = 0;

	return S_OK;
}

void Scene::Render() const
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->Render(dc_set.buf_dc, dc_set.bit_rect);
	player->Render(dc_set.buf_dc, dc_set.bit_rect);
	player->RenderPlayerHP(dc_set.buf_dc, dc_set.bit_rect, camera->Rect());
	monster_manager->Render(dc_set.buf_dc, dc_set.bit_rect);
	missile_manager->Render(dc_set.buf_dc, dc_set.bit_rect);
	crosshair->Render(dc_set.buf_dc, dc_set.bit_rect);
	weapon->Render(dc_set.buf_dc, dc_set.bit_rect);
	effect_manager->Render(dc_set.buf_dc, dc_set.bit_rect);

	DrawBuffer(dc_set.buf_dc, camera->Rect());
}

void Scene::Update()
{
	// player, monster 업데이트 루틴
	player->Update(dungeon, weapon, crosshair, missile_manager, animation_manager, sound_manager, effect_manager);
	monster_manager->Update(dungeon, player, animation_manager, missile_manager, sound_manager);
	missile_manager->Update(dungeon, animation_manager);
	camera->Update(dungeon, player);
	crosshair->Update(camera);
	weapon->Update(player, crosshair, animation_manager);
	effect_manager->Update(animation_manager);
	HitUpdate();
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

	if (update_cnt++ % 1000 == 0)
		monster_manager->Appear(5);
}

void Scene::HitUpdate()
{
	for (auto* monster : monster_manager->monsters)
		if (monster->IsAppeared()) {
			HitScan(player, monster, sound_manager);
			HitScan(monster, player, sound_manager);
			for (auto* missile : missile_manager->missiles)
				if (missile->host == player)
					HitScan(missile, monster, missile_manager, sound_manager);
				else
					HitScan(missile, player, missile_manager, sound_manager);
		}
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
		throw L"ChangeDungeon_dungeon_id was 0";
	delete(dungeon);
	dungeon = new Dungeon(dungeon_id);
}

void Scene::LoadPlayerAniamtion()
{
	animation_manager->Insert("player_stand");
	animation_manager->Insert("player_move");
	animation_manager->Insert("RustyGreatSwordAttack");
	animation_manager->Insert("SwordMissile");

	//
	// 던전 구분 않고 미사일 애니메이션들 여기서 로드
	// 나중에 구분하도록 바꾸기
	animation_manager->Insert("BansheeBullet");
	animation_manager->Insert("SkellBossBullet");
}

void Scene::LoadBattleSound()
{
	sound_manager->InsertEffectSound("sound\\walk.mp3");
	sound_manager->InsertEffectSound("sound\\dash.mp3");
	sound_manager->InsertEffectSound("sound\\jump.mp3");
	sound_manager->InsertEffectSound("sound\\swing1.mp3");
	sound_manager->InsertEffectSound("sound\\Slash2.ogg");
	sound_manager->InsertEffectSound("sound\\Slash8.ogg");
	sound_manager->InsertEffectSound("sound\\Thunder7.ogg");
	sound_manager->InsertEffectSound("sound\\Water1.ogg");
	sound_manager->InsertEffectSound("sound\\Explosion1.ogg");
}

void Scene::LoadBattleEffect()
{
	animation_manager->Insert("Dust");
}