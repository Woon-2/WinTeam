#pragma once
#include "gameNode.h"
#include "player.h"

// 마을 씬
class gameScene : public gameNode
{
private:
	player*	_player;	// 플레이어

public:
	gameScene();
	~gameScene();

	HRESULT init();
	void release();
	void update();
	void render();
	
	void goDungeon();
};

