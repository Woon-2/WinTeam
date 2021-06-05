#pragma once
#include "gameNode.h"

class animation;

class effect : public gameNode
{
protected:
	int			_x;
	int			_y;

	image*		_effectImage;
	animation*	_effectAnimation;
	BOOL		_isRunning;
	float		_elapsedTime;       //애니메이션 감을 틱 카운터

public:
	effect();
	~effect();

	virtual HRESULT init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime);

	virtual void release();
	virtual void update();
	virtual void render();

	void startEffect(int x, int y);

	virtual void killEffect(void);

	BOOL getIsRunning(void) { return _isRunning; }

};

