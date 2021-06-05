#pragma once
#include "gameNode.h"
#include "player.h"

class dungeon : public gameNode
{
	player* _player;	// 던전에서 쓸 플레이어

public:
	dungeon();
	~dungeon();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

