#include "stdafx.h"
#include "action.h"

HRESULT action::init()
{
	_worldTimeCount = 0;
	_isMoving = false;

	return S_OK;
}

void action::release()
{
}

void action::update()
{
	moving();
}

void action::render()
{
}

void action::moveTo(image * image, float endX, float endY, float time)
{
	if (!_isMoving)
	{
		_image = image;

		_endX = endX;
		_endY = endY;

		_startX = image->getX();
		_startY = image->getY();

		//시작지점에서 끝지점까지의 거리를 구한다.
		_travelRange = getDistance(_startX, _startY, _endX, _endY);

		//시작지점에서 끝지점까지 날라가야될 물체 각도
		_angle = getAngle(_startX, _startY, _endX, _endY);

		//_worldTimeCount는 타임매니저꺼 갔다 집어 넣는다.
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		_time = time;

		_isMoving = true;
	}
}

void action::moving(void)
{
	if (!_isMoving) return;

	//경과시간
	float elapseTime = TIMEMANAGER->getElapsedTime();

	//속도구하는 식
	float moveSpeed = (elapseTime / _time) * _travelRange;
	//     내속도   =  (틱카운터(0.001) / 소요시간 * 거리

	//이미지 x y 축 움직이게 하자
	_image->setX(_image->getX() + cosf(_angle) * moveSpeed);
	_image->setY(_image->getY() + -sinf(_angle) * moveSpeed);

	//이미지 x y이 도착지점에 가면 멈추자
	if (_time + _worldTimeCount <= TIMEMANAGER->getWorldTime())
	{
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		_image->setX(_endX);
		_image->setY(_endY);

		_isMoving = false;
	}
}
