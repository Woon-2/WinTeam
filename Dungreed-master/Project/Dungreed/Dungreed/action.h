#pragma once
#include "gameNode.h"
/*
시작점에서 끝점으로 보낼 때 거리와 소요시간으로
보내는 속도를 구할려고 할 때
(시작점에서 끝점으로 갈 때 최적의 루트로 이동한다.)
*/
class action : public gameNode
{
private:
	image * _image;

	float _startX, _startY; //시작 지점
	float _endX, _endY;     //도착 지점

	float _angle;
	float _travelRange;     //여행거리(도달할 거리)
	float _worldTimeCount;  //실행되고 나서 월드 시간(컴파일 되고 실제 시간)
	float _time;            //시간

	bool _isMoving;         //움직였는가?
public:
	action() {}
	~action() {}

	HRESULT init();
	void release();
	void update();
	void render();

	//            이미지             도착지점(x, y)     시작지점에서 도착지점까지 걸리는 시간 설정 
	void moveTo(image* image, float endX, float endY, float time);
	void moving(void);
	bool getIsMoving() { return _isMoving; }
};

