#include "stdafx.h"
#include "startScene.h"


startScene::startScene()
{
}


startScene::~startScene()
{
}

HRESULT startScene::init()
{
	// 사용할 이미지 등록
	IMAGEMANAGER->addImage("시작화면", "image/startScene/intro.bmp", WINSIZEX, WINSIZEY, false, NULL);

	IMAGEMANAGER->addImage("게임시작_그레이", "image/startScene/button_gamestart_gray.bmp", 180, 100, true, RGB(255, 0, 255));
	_btnGameStartGray = IMAGEMANAGER->findImage("게임시작_그레이");
	
	IMAGEMANAGER->addImage("게임시작_화이트", "image/startScene/button_gamestart_white.bmp", 180, 100, true, RGB(255, 0, 255));
	_btnGameStartWhite = IMAGEMANAGER->findImage("게임시작_화이트");

	IMAGEMANAGER->addImage("설정_그레이", "image/startScene/button_setting_gray.bmp", 100, 80, true, RGB(255, 0, 255));
	_btnSettingGray = IMAGEMANAGER->findImage("설정_그레이");

	IMAGEMANAGER->addImage("설정_화이트", "image/startScene/button_setting_white.bmp", 100, 80, true, RGB(255, 0, 255));
	_btnSettingWhite = IMAGEMANAGER->findImage("설정_화이트");

	IMAGEMANAGER->addImage("종료_그레이", "image/startScene/button_end_gray.bmp", 100, 80, true, RGB(255, 0, 255));
	_btnEndGray = IMAGEMANAGER->findImage("종료_그레이");

	IMAGEMANAGER->addImage("종료_화이트", "image/startScene/button_end_white.bmp", 100, 80, true, RGB(255, 0, 255));
	_btnEndWhite = IMAGEMANAGER->findImage("종료_화이트");

	


	// *****************************
	// *                           *
	// *     이미지 위치 설정       *
	// *                           *
	// *****************************
	// 게임시작 버튼 포지션 세팅
	_btnGameStartGray->setX(WINSIZEX / 2 - _btnGameStartGray->getWidth() / 2);
	_btnGameStartGray->setY(WINSIZEY / 2 + 100);
	_btnGameStartWhite->setX(WINSIZEX / 2 - _btnGameStartWhite->getWidth() / 2);
	_btnGameStartWhite->setY(WINSIZEY / 2 + 100);

	// 설정 버튼 포지션 세팅
	_btnSettingGray->setX(WINSIZEX / 2 - _btnSettingGray->getWidth() / 2);
	_btnSettingGray->setY(WINSIZEY / 2 + 200);
	_btnSettingWhite->setX(WINSIZEX / 2 - _btnSettingWhite->getWidth() / 2);
	_btnSettingWhite->setY(WINSIZEY / 2 + 200);

	// 종료 버튼 포지션 세팅
	_btnEndGray->setX(WINSIZEX / 2 - _btnEndGray->getWidth() / 2);
	_btnEndGray->setY(WINSIZEY / 2 + 280);
	_btnEndWhite->setX(WINSIZEX / 2 - _btnEndWhite->getWidth() / 2);
	_btnEndWhite->setY(WINSIZEY / 2 + 280);

	return S_OK;
}

void startScene::release()
{
}

void startScene::update()
{
	// 게임시작 버튼에 마우스가 올라가 있고 마우스 왼클릭이 되면
	if (PtInRect(&_btnGameStartWhite->collisionBox(), _ptMouse) && _leftButtonDown)
	{
		SCENEMANAGER->changeScene("게임씬");
	}

	// 종료버튼에 마우스가 올라가 있고 마우스 왼클릭이 되면
	if (PtInRect(&_btnEndWhite->collisionBox(), _ptMouse) && _leftButtonDown)
	{
		PostQuitMessage(0);	// 종료메세지 전송
	}
}

void startScene::render()
{
	// 스타트 씬 배경화면 렌더
	IMAGEMANAGER->findImage("시작화면")->render(getMemDC(), 0, 0);

	// 게임시작 버튼 렌더
	_btnGameStartGray->render(getMemDC(), _btnGameStartGray->getX(), _btnGameStartGray->getY());
	if (PtInRect(&_btnGameStartGray->collisionBox(), _ptMouse))
	{
		_btnGameStartWhite->render(getMemDC(), _btnGameStartWhite->getX(), _btnGameStartWhite->getY());
	}

	// 설정 버튼 렌더
	_btnSettingGray->render(getMemDC(), _btnSettingGray->getX(), _btnSettingGray->getY());
	if (PtInRect(&_btnSettingGray->collisionBox(), _ptMouse))
	{
		_btnSettingWhite->render(getMemDC(), _btnSettingWhite->getX(), _btnSettingWhite->getY());
	}

	// 종료 버튼 렌더
	_btnEndGray->render(getMemDC(), _btnEndGray->getX(), _btnEndGray->getY());
	if (PtInRect(&_btnEndGray->collisionBox(), _ptMouse))
	{
		_btnEndWhite->render(getMemDC(), _btnEndWhite->getX(), _btnEndWhite->getY());
	}
}
