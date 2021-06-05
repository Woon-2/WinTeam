#pragma once
#include "gameNode.h"

class character : public gameNode
{
protected:
	image*	_charImage;		  // 캐릭터 이미지

	int		_maxHp;		  // 최대 생명력
	int		_currentHp;	  // 현재 생명력
	int		_atk;		  // 공격력
	float	_atkSpeed;	  // 공격속도
	int		_def;		  // 방어력
	float	_moveSpeed;	  // 이동속도
	float	_jumpPower;	  // 점프력

	float	_gravity;	  // 중력

public:
	character();
	~character();

	// getter setter
	int getMaxHp() { return _maxHp; }
	void setMaxHp(int maxHp) { _maxHp = maxHp; }
	int getCurrentHp() { return _currentHp; }
	void setCurrnetHp(int currentHp) { _maxHp = currentHp; }

	int getAtk() { return _atk; }
	void setAtk(int atk) { _atk = atk; }

	int getDef() { return _def; }
	void setDef(int def) { _def = def; }

	float getMoveSpeed() { return _moveSpeed; }
	void setMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }

	float getJumpPower() { return _jumpPower; }
	void setJumpPower(float jumpPower) { _jumpPower = jumpPower; }

	float getAtkSpeed() { return _atkSpeed; }
	void setAtkSpeed(float atkSpeed) { _atkSpeed = atkSpeed; }

};

