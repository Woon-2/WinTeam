#include "stdafx.h"
#include "button.h"

button::button() {}

button::~button() {}

HRESULT button::init(const char * imageName, int x, int y, POINT btnDownFramePoint, POINT btnUpFramePoint, CALLBACK_FUNCTION cbFunction)
{
	_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFunction);

	_direction = BUTTONDIRECTION_NULL;

	_x = x;
	_y = y;

	_btnUpFramePoint = btnUpFramePoint;
	_btnDownFramePoint = btnDownFramePoint;

	_imageName = imageName;
	_image = IMAGEMANAGER->findImage(imageName);
	_rc = RectMakeCenter(x, y, _image->getFrameWidth(),
		_image->getFrameHeight());

	return S_OK;
}

void button::release(void)
{
}

void button::update(void)
{
	if (PtInRect(&_rc, _ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_direction = BUTTONDIRECTION_DOWN;
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON) && _direction == BUTTONDIRECTION_DOWN)
		{
			_direction = BUTTONDIRECTION_UP;
			//버튼을 눌렀다 때게 되면 그때 
			//콜백함수가 실행이 된다.
			_callbackFunction();
		}
	}
	else _direction = BUTTONDIRECTION_NULL;
}

void button::render(void)
{
	switch (_direction)
	{
	case BUTTONDIRECTION_UP: case BUTTONDIRECTION_NULL:
		_image->frameRender(getMemDC(),
			_rc.left, _rc.top, _btnUpFramePoint.x,
			_btnUpFramePoint.y);
		break;
	case BUTTONDIRECTION_DOWN:
		_image->frameRender(getMemDC(),
			_rc.left, _rc.top, _btnDownFramePoint.x,
			_btnDownFramePoint.y);
		break;
	}
}
