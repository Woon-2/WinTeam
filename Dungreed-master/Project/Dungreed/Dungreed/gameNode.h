#pragma once
#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", BACKDCSIZEX, BACKDCSIZEY);//추가

																					 //자식윈도우 메시지 번후 구분을 위한 이넘문
enum CTRL
{
	CTRL_SAVE,         //세이브 버튼메세지
	CTRL_LOAD,		   //로드 버튼메세지
	CTRL_TERRAINDRAW,  //지형 버튼메세지
	CTRL_OBJDRAW,	   //오브젝트 버튼메세지
	CTRL_ERASER,	   //지우개 버튼메세지
	CTRL_END
};
class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;		         //매니저등을 초기화 할 것이냐
public:
	int _ctrSelect;                 //컨트롤 버튼 관련 함수
	virtual HRESULT init(void);		//초기화 함수
	virtual HRESULT init(bool managerInit);
	virtual void release(void);		//메모리 관련 해제
	virtual void update(void);		//업데이트(연산)
	virtual void render(void);	    //그려주는 함수

	virtual void setMap(void);
	virtual void save(void);
	virtual void load(void);

	void setCtrlSelect(int num) { _ctrSelect = num; }

	image* getBackBuffer(void) { return _backBuffer; }
	//MemDC -> 출력해주는 메모리 역활
	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }


	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);

	gameNode();
	virtual ~gameNode();
};

