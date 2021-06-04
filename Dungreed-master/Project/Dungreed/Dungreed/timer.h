#pragma once
class timer
{
private:
	bool _isHardware;			//하드웨어가 좋은지 여부
	float _timeScale;           //CPU 에 신호를 보내고 받았을때의 시간량(얼마만큼 걸렸는지)
	float _timeElapsed;         //시간흐름 -> tickCount를 지칭함 보통 밀리세컨드 0.001fdklasjfdla 초
								/*
								__int64 = 64비트 정수(메모리8바이트)이며 long long으로 표현할 수 있다.
								-9223372036854775808 ~ 9223372036854775807

								unsigned long = 정수형이며 4바이트이며 0 ~ 4,294,967,295표현할수 있다.
								*/
	__int64 _curTime;              //현재 신호(?)를 보내는 빈도
	__int64 _lastTime;             //CPU에 신호를 던졌을때 마지막으로 받았을때 간격 값
	__int64 _periodFrequency;      //CPU 반응속도

	unsigned long _frameRate;      //프레임속도
	unsigned long _FPSframeCount;  //프레임카운터

	float _FPSTimeElapsed;      //프레임시간흐름 -> 밀리세컨
	float _worldTime;           //컴파일된 후 경과시간

public:
	timer();
	~timer();

	HRESULT init(void);
	void tick(float lockFPS = 0.0f);
	unsigned long getFrameRate(char* str = NULL) const;
	/*
	const를 붙이기 되면 (수정이 불가능한)참조만 가능한 읽기 전용 함수가 된다.
	간접적으로 접근하여 값의 수정을 막을수 있다.
	*/
	inline float getElapsedTime(void) const { return _timeElapsed; }
	inline float getWorldTime(void) const { return _worldTime; }

};


