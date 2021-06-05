#include "stdafx.h"
#include "player.h"
#include "gameScene.h"

player::player()
{
}


player::~player()
{
}

HRESULT player::init(float x, float y)
{
	_state = DOWN;	// 시작상태 -> 다운
	_probeY = (float)(_rc.bottom + 10.0f);	// 다운 상태에서 탐사Y 설정

	_x = x;		// 시작위치 X
	_y = y;		// 시작위치 Y
	_rc = RectMakeCenter(_x, _y, 50, 50);

	_player = IMAGEMANAGER->addFrameImage("플레이어", "image/gameScene/플레이어.bmp", 160, 100, 5, 2, true, RGB(255, 0, 255));
	_player->setX(700.0f);
	_player->setY(250.0f);
	_player->setFrameX(0);
	_player->setFrameY(0);

	_moveSpeed = 40.0f;			// 이동속도 설정
	_jumpPower = 0.0f;			// 점프파워 초기화
	_jumpCount = 0.0f;			// 점프카운트 초기화
	_gravity = 0.6f;			// 중력 설정

	_anglePoint.x = WINSIZEX / 2;	// 각도 측정용 RECT 좌표 x
	_anglePoint.y = WINSIZEY / 2;	// 각도 측정용 RECT 좌표 y
	_angleRc = RectMakeCenter(_anglePoint.x, _anglePoint.y, 25, 25);
	_angle = getAngle(_anglePoint.x, _anglePoint.y, _ptMouse.x, _ptMouse.y);		// 마우스 방향에 맞춰 각도 설정
	_angleRcJumpPower = _jumpPower;
	_angleFlagDirectionRight = false;
	_angleFlagDirectionLeft = true;

	_dash = false;				// 처음엔 대쉬 상태가 아니게 시작
	_dashPower = 200.0f;		// 대쉬 파워 초기화
	_dashCount = 0.0f;			// 대쉬 시간 카운터 초기화
	
	_cameraX = _x - WINSIZEX / 2;	// 카메라 X 좌표 초기화
	_cameraY = _y - WINSIZEY / 2;	// 카메라 Y 좌표 초기화

	return S_OK;
}

void player::release()
{
}

void player::update()
{
	if (_rightButtonDown && !_dash)
	{
		_dash = true;	// 마우스 오른쪽 클릭되면 대쉬 상태로
		_angle = getAngle(_anglePoint.x, _anglePoint.y, _ptMouse.x, _ptMouse.y);		// 마우스 방향에 맞춰 각도 설정
	}
	
	if (_dash)
	{
		dash();		// 대쉬 기능 호출
		cameraMove();
	}

	else if (!_dash)
	{
		move();		// 캐릭터의 기본 상하좌우 이동과 점프 기능
		cameraMove();
		anglePointMove();
	}
		
	
}

void player::render()
{
	// 샘플 캐릭터 렌더
	Rectangle(this->getMemDC(), _rc.left, _rc.top, _rc.right, _rc.bottom);

	// 캐릭터 이미지 렌더
	_player->frameRender(getMemDC(), _x, _y, 0, 0);
	
	// 카메라를 이용한 보여주기 렌더
	_backBuffer->render(getMemDC(), 0, 0, _cameraX, _cameraY, WINSIZEX, WINSIZEY);

	// 각도 측정용 RECT 확인용 렌더
	Rectangle(getMemDC(), _angleRc.left, _angleRc.top, _angleRc.right, _angleRc.bottom);

	// 좌표 확인
	char str[100];
	sprintf_s(str, "%.0f %.d   %d    %f", _x, _rc.bottom, _state, _angle);
	TextOut(getMemDC(), 50, 50, str, sizeof(str));
}

void player::move()
{
	// ************************************* 캐릭터 조정 **********************************************

	// 좌로 이동
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_x -= _moveSpeed;				// 플레이어 x좌표 감소
		if (_x - 25.0f < 0.0f) _x = 25.0f;	// 왼쪽으로 맵 이탈 방지

		if (_state == LANDING && !pixelCollision(_probeY, 0, 255, 0))
		{
			_state = DOWN;
			_probeI = (float)_rc.bottom;
			_probeY = (float)_rc.bottom + 50.0f;

			_gravity = 9.0f;
		}
	}

	// 우로 이동
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_x += _moveSpeed;	// 플레이어 x좌표 증가
		if (_x + 25.0f > _map->getWidth()) _x = _map->getWidth() - 25.0f; // 오른쪽으로 맵 이탈 방지

		if (_state == LANDING && !pixelCollision(_probeY, 0, 255, 0))
		{
			_state = DOWN;
			_probeI = (float)_rc.bottom;
			_probeY = (float)_rc.bottom + 50.0f;

			_gravity = 9.0f;
		}
	}

	// 아래로 이동(점프)
	if (KEYMANAGER->isStayKeyDown('S') && KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state == LANDING)
	{
		// 초록 땅을 밟은 상태에서 하단 점프를 하는 경우
		if (pixelCollision(_probeY, 0, 255, 0))
		{
			_jumpPower = -10.0f;
			_gravity = 9.0f;

			_state = DOWNJUMP;			// 하단 점프상태(DOWNJUMP)로 변경
		}
		
		// LANDING 상태지만 빨강 바닥 위에선 더 아래로 갈 수 없음

	}
	// 위로 점프
	else if (KEYMANAGER->isOnceKeyDown(VK_SPACE) || KEYMANAGER->isStayKeyDown('W'))
	{
		_state = UP;			// 점프상태(UP)로 변경

		_jumpPower = 50.0f;		// 점프파워 설정
		_gravity = 9.0f;		// 중력 설정

		_jumpCount++;			// 점프 카운트 증가
	}

	
	_y -= _jumpPower;			// 남은 점프파워 만큼 캐릭터 점프(상하 이동)
	_angleRcJumpPower = _jumpPower;	// 각도 측정용 렉트도 점프파워만큼 점프
	_jumpPower -= _gravity;		// 점프파워가 중력에 의해 영향을 받음(감소함)
	if (_jumpPower < 0 && !pixelCollision(_probeY, 0,255, 0)) _state = DOWN;	// 점프파워가 0보다 작아서 캐릭터가 하강인 경우에 -> 상태를 다운으로 변경

	// 캐릭터 상태에 따른 픽셀충돌(_probeY) 조정
	// 위로 점프하는 상태
	if (_state == UP)
	{
		_probeI = (float)_rc.top;
		_probeY = (float)_rc.bottom;

		if (_y - 25 < 0.0f) _y = 25.0f; // 플레이어 맵 위로 이탈 방지

		if (_y <= WINSIZEY / 2)
		{
			_anglePoint.y -= _angleRcJumpPower;

			if (_anglePoint.y < 0)
			{
				_anglePoint.y = 25;
			}

			_angleRc = RectMakeCenter(_anglePoint.x, _anglePoint.y, 25, 25);
		}
	}

	// 바닥에 착지 상태
	if (_state == LANDING)
	{
		_probeI = (float)_rc.bottom - 80;
		_probeY = (float)_rc.bottom + 50;

		if (pixelCollision(_probeY, 0, 255, 0))
		{
			_y = _probeI - 25.0f;		// y좌표를 땅 위로 설정
		}
	}

	// 하강 상태
	if (_state == DOWN)
	{
		_probeI = (float)_rc.bottom;
		_probeY = (float)_rc.bottom + 70.0f;

		if (_y + 25 > _map->getHeight()) _y = _map->getHeight() - 25.0f;

		if (pixelCollision(_probeY, 0, 255, 0))	// 녹색 바닥과 충돌 했는가?
		{
			_jumpCount = 0;			// 녹색 바닥을 밟으면 점프카운트를 0으로 -> 착지상태로
			_jumpPower = 0.0f;		// 점프파워 설정	
			_gravity = 0.0f;		// 아래로 내려가지 않게 중력을 0으로 설정
		
			_y = _probeI - 25.0f;		// y좌표를 땅 위로 설정
			_state = LANDING;		// 착지 상태로 변경
		}

		_probeI = _rc.bottom;		// 탐사 시작 위치 재 설정
		if (pixelCollision(_probeY, 255, 0, 0)) // 빨강 바닥과 충돌 했는가?
		{
			_jumpCount = 0;			// 빨강 바닥을 밟으면 점프카운트를 0으로 -> 착지상태로
			_jumpPower = 0.0f;		// 점프파워 설정	
			_gravity = 0.0f;		// 아래로 내려가지 않게 중력을 0으로 설정
		
			_y = _probeI - 25.0f;		// y좌표를 땅 위로 설정
			_state = LANDING;		// 착지 상태로 변경
		}

		if (_y - _cameraY != WINSIZEY / 2)
		{
			_anglePoint.y -= _angleRcJumpPower;

			if (_anglePoint.y > WINSIZEY / 2)
			{
				_anglePoint.y = WINSIZEY / 2;
			}

			_angleRc = RectMakeCenter(_anglePoint.x, _anglePoint.y, 25, 25);
		}
	}

	// 하단 점프를 하고 있을 때
	if (_state == DOWNJUMP)
	{
		_probeI = (float)_rc.top;
		_probeY = (float)_rc.bottom;

		// 녹색 땅과 충돌에서 벗어난 경우
		if (!pixelCollision(_probeY, 0, 255, 0))
		{
			// 일반 다운 상태로 변경
			_state = DOWN;
		}
	}

	_rc = RectMakeCenter(_x, _y, 50, 50);		// 최종 좌표에 캐릭터의 포지션을 잡는다

	// end of 캐릭터 조정 *********************************************************************************

}

void player::dash()
{	
	_x += cosf(_angle) * _dashPower;
	_y += -sinf(_angle) * _dashPower;

	// 각도 조절용 _angleRc 조절해야 하는가?
	if (_x <= WINSIZEX / 2 || _x >= _map->getWidth() - WINSIZEX / 2 ||
		_y <= WINSIZEY / 2)
	{
		_anglePoint.x += cosf(_angle) * _dashPower;
		_anglePoint.y += -sinf(_angle) * _dashPower;		
	}

	_dashCount += 50.0f;
	_gravity = 6.0f;

	if (_angle <= PI)
	{
		_state = UP;
		_probeI = (float)_rc.top;
		_probeY = (float)_rc.bottom;
	}
	if (_angle > PI)
	{
		_state = DOWNJUMP;
		_probeI = (float)_rc.top;
		_probeY = (float)_rc.bottom;

		if (pixelCollision(_probeY, 255, 0, 0)) // 빨강 바닥과 충돌 했는가?
		{
			_jumpCount = 0;			// 빨강 바닥을 밟으면 점프카운트를 0으로 -> 착지상태로
			_jumpPower = 0.0f;		// 점프파워 설정	
			_gravity = 0.0f;		// 아래로 내려가지 않게 중력을 0으로 설정

			_y = _probeI - 25.0f;	// y좌표를 땅 위로 설정
			_state = LANDING;		// 착지 상태로 변경
		}
	}

	if (_x - 25 < 0.0f) _x = 25.0f;	// 왼쪽으로 맵 이탈 방지
	if (_x + 25 > _map->getWidth()) _x = _map->getWidth() - 25.0f; // 오른쪽으로 맵 이탈 방지

	_rc = RectMakeCenter(_x, _y, 50, 50);		// 최종 좌표에 캐릭터의 포지션을 잡는다

	anglePointMove();
	
	if (_dashCount > 100.0f)
	{
		_dashCount = 0.0f;
		_dash = false;
	}
	
}

bool player::pixelCollision(int probeY, int probeR, int probeG, int probeB)
{
	for (; _probeI <= probeY; ++_probeI)
	{			
		/*
		COLORREF = RGB색상을 표현
		GetPixel = 특정 위치의 (X,Y)에 있는 픽셀의
		RGB값을 돌려준다. 이작업을 하기 위해서는 SELECT된
		비트맵의 정보구조를 파악하고 비트맵의 이미지 버퍼를
		얻어와 해당 위치에 존재하는 BYTE값을 조사하여
		RGB값을 뻄
		*/
		COLORREF color = GetPixel(_mapLand->getMemDC(), _x, _probeI);

		//GetR(G)(B)Value  = 색상값 출력 코드
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == probeR && g == probeG && b == probeB)
		{
			return true;		// 픽셀 탐색 결과 원하는 색상과 일치함.			
		}
	}
	return false;	// 픽셀 탐색 결과 원하는 색상과 다름
}

void player::anglePointMove()
{
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		if (_x < WINSIZEX / 2)
		{
			_angleFlagDirectionRight = false;
		}
		if (_x - _cameraX != WINSIZEX / 2)
		{
			_anglePoint.x -= _moveSpeed;
		}

		if (_x - _cameraX == WINSIZEX / 2 && _angleFlagDirectionLeft == false)
		{
			_anglePoint.x -= _moveSpeed;
			_angleFlagDirectionLeft = true;
		}
		
		if (_anglePoint.x < 0)
		{
			_anglePoint.x = 25;
		}
	}

	if (KEYMANAGER->isStayKeyDown('D'))
	{
		if (_x > _map->getWidth() - WINSIZEX / 2)
		{
			_angleFlagDirectionLeft = false;
		}

		if (_x - _cameraX != WINSIZEX / 2)
		{
			_anglePoint.x += _moveSpeed;
		}

		if (_x - _cameraX == WINSIZEX / 2 && _angleFlagDirectionRight == false)
		{
			_anglePoint.x += _moveSpeed;
			_angleFlagDirectionRight = true;
		}

		if (_anglePoint.x > WINSIZEX - 25)
		{
			_anglePoint.x = WINSIZEX - 25;
		}
	}
	_angleRc = RectMakeCenter(_anglePoint.x, _anglePoint.y, 25, 25);

}

void player::cameraMove()
{
	// 카메라 조정 ****************************************************************************************
	_cameraX = _x - WINSIZEX / 2;
	_cameraY = _y - WINSIZEY / 2;

	// ********* 카메라가 맵을 벗어나지 않도록 위치 조정 **********************

	// 캐릭터가 왼쪽 끝에 가까워져 카메라가 왼쪽으로 이동하면 안될 때
	if (_x - WINSIZEX / 2 < 0) _cameraX = 0;
	// 캐릭터가 위쪽 끝에 가까워져 카메라가 위쪽으로 이동하면 안될 때
	if (_y - WINSIZEY / 2 < 0) _cameraY = 0;
	// 캐릭터가 오른쪽 끝에 가까워져 카메라가 오른쪽으로 이동하면 안될 때
	if (_x + WINSIZEX / 2 > _map->getWidth()) _cameraX = _map->getWidth() - WINSIZEX;
	//// 캐릭터가 아래쪽 끝에 가까워져 카메라가 아래쪽으로 이동하면 안될 때
	if (_y + WINSIZEY / 2 > _map->getWidth()) _cameraY = _map->getHeight() - WINSIZEY;

	// end of 카메라 조정 **********************************************************************************
}
