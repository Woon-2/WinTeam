#include "stdafx.h"
#include "mainGame.h"

mainGame::mainGame() {}

mainGame::~mainGame() {}

//초기화
HRESULT mainGame::init(void)
{
	//upcasting : 부모클래스를 자식클래스에서 호출한다
	gameNode::init(true);  //게임노드의 init 함수에 타이머 셋팅 및 키매니져 초기화 관련 함수를 실행한다.
	
	
	_startScene = new startScene;					// 스타트 씬 동적할당
	_startScene->init();							// 스타트 씬 초기화
	SCENEMANAGER->addScene("스타트씬", _startScene);	// 스타트 씬, 씬 리스트에 등록

	_gameScene = new gameScene;						// 게임 씬 동적할당
	SCENEMANAGER->addScene("게임씬", _gameScene);	// 게임 씬, 씬 리스트에 등록

	_dungeon = new dungeon;							// 던전 동적할당
	SCENEMANAGER->addScene("던전", _dungeon);		// 던전 씬, 씬 리스트에 등록

	SCENEMANAGER->changeScene("스타트씬");			// 처음 시작하면 스타트씬으로 시작

	return S_OK;
}

//메모리 해제
void mainGame::release(void)
{
	gameNode::release(); 
}

//연산은 여기서 한다
void mainGame::update(void)
{
	gameNode::update();  

	SCENEMANAGER->update();
	
}

//그리는 것은 이제 여기에다
void mainGame::render()
{
	PatBlt(getMemDC(), 0, 0, BACKDCSIZEX, BACKDCSIZEY, WHITENESS);
	//================== 이 위는 손대지 마시오 =========================
	
	SCENEMANAGER->render();

	//================== 이 아래는 손대지 마시오 ========================
	this->getBackBuffer()->render(getHDC(), 0, 0);
}