#include "stdafx.h"
#include "database.h"

HRESULT database::init()
{
	_angle = 0;
	loadDatabase("database.txt");
	return S_OK;
}

void database::release()
{
}

void database::loadDatabase(string name)
{
	arrElements vTemp;
	//c_str == string을 char형으로 변환
	vTemp = TXTDATA->txtLoad(name.c_str());

	string str;
	int count = 0;

	for (int i = 0; i < vTemp.size(); ++i)
	{
		//만약에 vTemp가 메모장에 있는 |가 맞으면
		if (vTemp[i] == "|")
		{
			elements* em = new elements;
			//str 에 | 다음번째를 넣어라
			str = vTemp[i + 1];

			_mTotalElement.insert(pair<string,
				elements*>(vTemp[i + 1], em));

			//만약에 i가 0이아니면
			//다음 |(7번째)있는 곳으로 읽어라
			if (i != 0) count += 7;
			continue;
		}
		iterElement mIter = _mTotalElement.find(str);

		//파일이름
		if (i == count + 1) mIter->second->name = vTemp[i].c_str();
		//현재체력
		else if (i == count + 2) mIter->second->currentHP = atoi(vTemp[i].c_str());
		//최대체력
		else if (i == count + 3) mIter->second->maxHP = atoi(vTemp[i].c_str());
		//각도
		else if (i == count + 4) mIter->second->angle = (float)atof(vTemp[i].c_str());
		//최대스피드
		else if (i == count + 5) mIter->second->maxSpeed = (float)atof(vTemp[i].c_str());
		//가속도
		else if (i == count + 6) mIter->second->accelaration = (float)atof(vTemp[i].c_str());
	}
	vTemp.clear();
}

void database::setElementDataCurrentHP(string str, float ch)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->currentHP = ch;
}

void database::setElementDataMaxHP(string str, float mh)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->maxHP = mh;
}

void database::setElementDataAngle(string str, float a)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->angle = a;
}

void database::setElementDataAccelaration(string str, float accel)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->accelaration = accel;
}

void database::setElementDataMaxSpeed(string str, float ms)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->maxSpeed = ms;
}
