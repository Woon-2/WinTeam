#include "stdafx.h"
#include "gameScene.h"


gameScene::gameScene()
{
}


gameScene::~gameScene()
{
}

HRESULT gameScene::init()
{
	_map = IMAGEMANAGER->findImage("마을_표면");
	_map->setX(0);
	_map->setY(0);

	_player = new player;
	_player->init(700.0f, 700.0f);
	
	return S_OK;
}

void gameScene::release()
{
}

void gameScene::update()
{
	_player->update();
	
	

	// 임시로 씬변환의 확인을 위해 z 키를 눌렀을 때 다른 씬(던전)으로 이동되는지 확인
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		SCENEMANAGER->changeScene("던전");
	}
}

void gameScene::render()
{
	// 실제 배경(등)이미지와 캐릭터 이미지가 그려진 이미지(배경) -> source
	_map->render(getMemDC(), _map->getX(), _map->getY());
	
	IMAGEMANAGER->findImage("마을_지형")->render(getMemDC());
	
	_player->render();
	
}

void gameScene::goDungeon()
{
	SCENEMANAGER->changeScene("던전");
}
