#pragma once
#include "singletonBase.h"
#include <vector>
#include <map>

//구조체 == public으로만 구성된 클래스
class elements
{
public:
	const char* name;
	int currentHP;
	int maxHP;
	float angle;
	float accelaration;
	float maxSpeed;

	elements() {};
	~elements() {};
};

class database : public singletonBase<database>
{
private:
	typedef vector<string>           arrElements;
	typedef vector<string>::iterator iterElements;

	typedef map<string, elements*>           arrElement;
	typedef map<string, elements*>::iterator iterElement;

private:
	arrElement _mTotalElement;
	float _angle;
public:
	database() {}
	~database() {}

	HRESULT init();
	void release();

	void loadDatabase(string name);

	//속성에 대한 접근자
	elements* getElementData(string str)
	{
		return _mTotalElement.find(str)->second;
	}

	//현재채력
	void setElementDataCurrentHP(string str, float ch);
	//최대체력
	void setElementDataMaxHP(string str, float mh);
	//각도
	void setElementDataAngle(string str, float a);
	//가속도
	void setElementDataAccelaration(string str, float accel);
	//최대스피드
	void setElementDataMaxSpeed(string str, float ms);

	void setAngle(float angle) { _angle = angle; }
	float getAngle() { return _angle; }
};

