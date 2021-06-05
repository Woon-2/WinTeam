#pragma once
#include "gameNode.h"

typedef void(*CALLBACK_FUNCTION)(void);
//콜백함수 == 함수포인터를 저장해서 변수로 가지고 있다가
//함수포인터를 이용해서 호출할 때 사용
//장점 호출한 함수의 러닝타임에 변행해서 사용이 가능

//공부를 하면 참 좋은게 있다
//<Functional> 이라는 라이브러리가 있는데
//여기에 std::Function, std::move, std::bind 등등
//콜백함수는 지금 전역으로 빼놔서 static 상태이므로
//매개변수를 사용할 수 없는 상태이다.
//하지만 Functional 라이브러리를 이용한다면 일반 매개변수도
//콜백함수에 넣어서 사용할 수 있다.

//함정) 의외로 쉽다
//함정2) 의외로 헤메기도 쉽다 

enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,
	BUTTONDIRECTION_UP,
	BUTTONDIRECTION_DOWN
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;

	const char* _imageName;
	image* _image;
	RECT _rc;
	float _x, _y;
	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction;
public:
	button();
	~button();

	HRESULT init(const char* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction);

	void release(void);
	void update(void);
	void render(void);
};

