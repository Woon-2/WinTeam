#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h> //timeGetTime() == 밀리세컨

#pragma comment(lib, "winmm.lib")

timer::timer()
{
}


timer::~timer()
{

}

HRESULT timer::init(void)
{
	//QueryPerformanceFrequency = 고성능 타이머 함수
	//LARGE_INTEGER = 32비트의 한계를 넘어 표현할 수 없는 것을 데이터로 표현하기 위해 씀

	//현제 cpu반응속도값이 들어오게 한다.
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		//반응속도가 좋으면 true가 되고
		_isHardware = true;
		//lastTime에 cou의 연산속도(1바퀴)값이 들어가게 한다.
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
		/*
		64비트로 카운트를 조사하므로 최대 18446744073709551616(1844경)이라는 어마어마한 수까지 카운팅할 수 있다. 주파수가 120만이라고 했을 때 하루는 3784조가 되며 64비트 정수로 최대 4873년 동안 카운팅을 할 수 있다.
		고해상도 타이머를 사용하면 특정 작업을 하는데 걸리는 시간을 정확하게 측정할 수 있다. 작업 시작전의 차운트를 조사해 놓고 작업이 끝난 후 다시 카운트를 조사해 두 값의 차를 구하고 이 차를 주파수로 니누변 정확하게 몇 초가 걸렸는지 계산 가능하다.

		단점 :
		1. 하드웨어 버그
		이것은 마이크로 소프트의 문제가 아니고 하드웨어 업체의 문제이다. BIOS나 드라이버에 버그가 있으면 스레드가 프로세서 간 이동할 때 이들 루틴에서 서로 다른 값을 넘기는 경우가 있어서 이런 하드웨어 버그를 피하기 위해서 특정 프로세서에 고정시키는 것이(SetThreadAffinityMask 사용) 좋다.
		2. 병목
		각 워커 스레드에서 시간 계산을 하면 각 스레드로부터 동기화 하기 위해 락을 걸어야 하므로 이것에 의한 병목이 발생하는데 이것을 막기 위해서 하나의 스레드에서 시간 계산을 하도록 하여 락을 사용하지 않도록 해야한다.

		그렇다면 이 두가지 문제만 해결하면 끝난것일까?
		인텔에서 또다른 문제를 제공한다. 바로 "터보 부스터"라는 cpu의 기능이다. 인텔의 네할렘 계열 CPU의 i3, i5, i7이 있는데 이들 CPU는 "터보 부스터" 기능이 있다.
		"터보 부스터"는 특정 코어가 부하가 걸리고 나머지 코어가 여유가 있다면 부하가 걸린 코어의 클록 주파수를 올린다.
		기능만 봐도 왜 문제가 생기는지 인지할 수 있다. 우리는 코어의 주파수로 타이머를 관리하는데 이 "터보 부스터"기능을 사용하게 되면 코어의 주파수가 바뀌어 버린다.
		우리가 QueryPerformanceCounter함수를 사용하기 전의 주파수와 사용한 후의 주파수가 달라져버리는 문제가 생긴다. 그래서 사용할 때에는 이 "터보 부스터"기능을 꺼야한다.
		*/
		//초당 cpu반응속도(1.0f)
		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		//펜티엄이라서 안좋을 때 사용(강제로 밀리세컨드로 사용)
		_isHardware = false;
		/*
		timeGetTime() = CPU속도에 따라 명령을 처리하는 속도가 다르기 때문에 컴퓨터마다 또는
		컴퓨터 상태마다 다른 속도로 실행되게 된다.
		이를 일정하게 처리하기 위해서 시간을 얻어와서 그 시간에 대해 처리해 줘야 한다.
		시간을 사용하기 위한 함수로 timeGetTime함수를 이용한다.​
		*/
		_lastTime = timeGetTime();	//강제로 0.001초로 맞춤 잘가라 CPU
		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSframeCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

//특정 프레임 고정
void timer::tick(float lockFPS)
{
	if (_isHardware)
	{
		//고성능 지원하면 cpu반응속도로 측정
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		//안되면 엡실론 수치
		_curTime = timeGetTime();
	}
	//반응속도 - 마지막반응속도 * 초당 속도 = 현제 반응속도(프레임)
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//프레임수치를 고정한다 == 프레임 스키핑
	if (lockFPS > 0.0f)
	{
		//프레임이 유지가 된다면0
		while (_timeElapsed < 1.0f / lockFPS)
		{
			//지속적으로 연산한다.
			if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;
	_FPSframeCount++;
	_FPSTimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	//초당 프레임이 유지되면~
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSframeCount;
		_FPSframeCount = 0;
		_FPSTimeElapsed = 0.0f;
	}

}

unsigned long timer::getFrameRate(char* str) const
{
	if (str != NULL)
	{
		wsprintf(str, "FPS : %d", _frameRate);
	}

	return _frameRate;
}
