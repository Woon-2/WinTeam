#pragma once
#include "gameNode.h"
class startScene : public gameNode
{
private:
	image* _btnGameStartGray;		// 게임시작 버튼 회색
	image* _btnGameStartWhite;		// 게임시작 버튼 흰색
	image* _btnSettingGray;			// 설정 버튼 회색
	image* _btnSettingWhite;		// 설정 버튼 흰색
	image* _btnEndGray;				// 종료 버튼 회색
	image* _btnEndWhite;			// 종료 버튼 흰색

public:
	startScene();
	~startScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

