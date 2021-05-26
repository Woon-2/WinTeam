#pragma once
#include "character.h"

class player : public character
{
	enum CHAR_STATE
	{
		UP,			// 캐릭터가 점프하면서 위로 올라가고 있는 상태
		LANDING,	// 캐릭터가 땅에 착지중인 상태
		DOWN,		// 캐릭터가 점프중에 아래로 하강하고 있는 상태
		DOWNJUMP,	// 캐릭터가 하단 점프를 사용하면서 땅(초록)과 겹쳐있는 상태
	};

private:
	CHAR_STATE _state;				// 현재 캐릭터의 상태를 저장

	image* _player;					// 플레이어 이미지

	RECT _rc;						// 테스트용 RECT
	float _x, _y;					// 플레이어 x, y 좌표
	float _jumpCount;				// 점프 횟수

	float _angle;					// 마우스 방향에 따른 각도 -> 대쉬와 공격에 사용
	POINT _anglePoint;				// 실제 캐릭터의 위치와 마우스의 위치는 사용자가 보는 모습과 것과 다르기 각도를 구하면 에러가 생김
									// 그래서 윈도우창 안에서만 움직이는(실제 플레이어는 윈도우창을 벗어나서 움직임), 대쉬와 원거리공격에 사용할 각도를 위한 
									// POINT(점)를 하나 만듬
	RECT _angleRc;					// 각도 RECT를 확인하기 위한 테스트용 렉트
	float _angleRcJumpPower;		// 각도 측정용 RECT의 점프파워
	bool _angleFlagDirectionRight;	// _angleRc가 오른쪽으로 움직일 때 위치를 보정해 줄 플래그
	bool _angleFlagDirectionLeft;	// _angleRc가 왼쪽으로 움직일 때 위치를 보정해 줄 플래그
	
	bool  _dash;					// 대쉬 상태인지 판별하는 변수
	float _dashPower;				// 대쉬 파워
	float _dashCount;				// 대쉬하는 시간 카운트


	float _cameraX, _cameraY;		// 카메라 x, y 좌표

	float _probeI;					// 픽셀충돌 반복에 사용할 I 변수 -> 탐색 시작 지점
	float _probeY;					// 픽셀충돌에 사용할 탐사 Y 좌표 -> 탐색 끝 지점

public:
	player();
	~player();

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	float getX() { return _x; }
	float getY() { return _y; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }

	void move();		// 상하좌우 이동 및 점프
	void dash();		// 마우스 우클릭시 대쉬 기능
	bool pixelCollision(int probeY, int probeR, int probeG, int probeB);

	void anglePointMove();
	void cameraMove();

	
};

