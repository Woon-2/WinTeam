#pragma once
#include "gameNode.h"

#include "startScene.h"
#include "gameScene.h"
#include "dungeon.h"

class mainGame : public gameNode
{
private:
	gameNode* _startScene;		// 스타트 씬(시작화면)
	gameNode* _gameScene;		// 게임 씬(메인) -> 처음에 마을로 셋팅

	gameNode* _dungeon;			// 던전

public:
	mainGame();
	~mainGame();

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render();
};

