#include "stdafx.h"
#include "dungeon.h"


dungeon::dungeon()
{
}


dungeon::~dungeon()
{
}

HRESULT dungeon::init()
{
	_map = IMAGEMANAGER->findImage("테스트");
	_map->setX(0);
	_map->setY(0);

	_player = new player;			// 던전 플레이어 동적할당
	_player->init(500.0f, 500.0f);	// 플레이어 x, y 값 넘겨주고 해당 좌표로 초기화

	return S_OK;
}

void dungeon::release()
{
}

void dungeon::update()
{
	_player->update();
}

void dungeon::render()
{
	_map->render(getMemDC(), _map->getX(), _map->getY());
	
	_player->render();
}
