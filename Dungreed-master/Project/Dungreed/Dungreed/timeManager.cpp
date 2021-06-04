#include "stdafx.h"
#include "timeManager.h"


timeManager::timeManager()
	: _timer(NULL)
{
}


timeManager::~timeManager()
{

}

HRESULT timeManager::init(void)
{
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release(void)
{
	if (_timer != NULL)
	{
		SAFE_DELETE(_timer);
	}
}

void timeManager::update(float lock)
{
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

void timeManager::render(HDC hdc)
{
	char str[256];
	string frameRate;
	//font 백그라운드 모드는 2개가 있다
	//TRANSPARENT : 투명
	//OPAQUE : 불투명
	SetBkMode(hdc, TRANSPARENT);
	/*
	https://blog.naver.com/alsdn726/221276453749
	전처리문 : 컴파일 이전에 미리 처리되는 문장을 말한다.선행처리기라고도 한다. 따라서 컴파일시
	작성한 코드보다 먼저 전처리문에서 한 내용이 먼저 수행된다. 종류로는
	파일 처리를 위한 전처리문 : #include = 헤더 파일과 같은 외부 파일을 읽어서 포함시킬 때 사용
	형태 정의를 위한 전처리문 : #define = 상수값을 지정하기 위한 예약어(매크로)
	#undef = #define으로 이미 정의된 매크로를 무효화시킨다
	조건 처리를 위한 전처리문 : #if ~ #endif = if문하고 비슷, 어떠한 구문을 컴파일 할지 안할지 정할 수 있다.
	#ifdef ~ #endif = 어떤 매크로가 정의되어있는지를 따진다. 매크로가 정의 되어 있다면 a를 컴파일 안되면  b를 컴파일
	#ifndef = 헤더파일이 겹치는 것을 막는 일종의 매크로
	#else, else와 같은 의미
	#elif else + if
	에러 처리를 위한 전처리문 : #error = 소스 라인에 직접 에러 메세지를 출력한다.
	디버깅을 위한 전처리문 : #line = 소스코드의 행번호를 지정하기 위한 것(디버깅 할 때 쓰면 편함)
	컴파일 옵션 처리를 위한 전처리문 : #pragma
	*/

	//위에 debug모드일때(릴리즈모드 보다 30배 느리다)
#ifdef _DEBUG
	{
		wsprintf(str, "framePerSec (FPS) : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));

		sprintf_s(str, "worldTime : %f", _timer->getWorldTime());
		TextOut(hdc, 0, 20, str, strlen(str));

		sprintf_s(str, "elapsedTime : %f", _timer->getElapsedTime());
		TextOut(hdc, 0, 40, str, strlen(str));
	}
#else
	{
		wsprintf(str, "framePerSec (FPS) : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	}
#endif

}
