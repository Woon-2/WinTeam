#include "stdafx.h"
#include "gameNode.h"

gameNode::gameNode()
{
}

gameNode::~gameNode()
{

}

HRESULT gameNode::init()
{
	_hdc = GetDC(_hWnd);
	_managerInit = false;

	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd);
	_managerInit = managerInit;

	if (_managerInit)
	{
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		SOUNDMANAGER->init();
		TIMEMANAGER->init();
		EFFECTMANAGER->init();
		SCENEMANAGER->init();
	}

	return S_OK;
}

//메모리 해제(내용 번부 변경)
void gameNode::release()
{
	if (_managerInit)
	{
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();
		EFFECTMANAGER->release();
		EFFECTMANAGER->releaseSingleton();
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();
		
	}

	ReleaseDC(_hWnd, _hdc);
}

void gameNode::update()
{
	SOUNDMANAGER->update();
}

void gameNode::render(void)
{
}

void gameNode::setMap(void)
{
}

void gameNode::save(void)
{
}

void gameNode::load(void)
{
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:

		// 게임에서 사용할 이미지 추가
		IMAGEMANAGER->addImage("마을_표면", "image/gameScene/마을_표면.bmp", BACKDCSIZEX, BACKDCSIZEY, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("마을_지형", "image/gameScene/마을_지형.bmp", BACKDCSIZEX, BACKDCSIZEY, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("테스트", "image/gameScene/testMap.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

		_map = static_cast<image*>(IMAGEMANAGER->findImage("마을_표면"));
		_mapLand = static_cast<image*>(IMAGEMANAGER->findImage("마을_지형"));
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		this->render();
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_TIMER:
		this->update();
		break;

	case WM_LBUTTONDOWN:
		_leftButtonDown = true;
		this->setMap();
		break;

	case WM_LBUTTONUP:
		_leftButtonDown = false;
		break;

	case WM_RBUTTONDOWN:
		_rightButtonDown = true;
		break;

	case WM_RBUTTONUP:
		_rightButtonDown = false;
		break;

	case WM_MOUSEMOVE:
	{
		_ptMouse.x = static_cast<float>LOWORD(lParam);
		_ptMouse.y = static_cast<float>HIWORD(lParam);

		//내가 마우스를 클릭한채 움직이면 타일셋의
		//타일이 해당 맵툴안에 그려져야 해서 여기에 호출
		if (_leftButtonDown) this->setMap();
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CTRL_SAVE:
			this->save();
			break;
		case CTRL_LOAD:
			this->load();
			InvalidateRect(hWnd, NULL, false);
			break;
		default:
			this->setCtrlSelect(LOWORD(wParam));
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
