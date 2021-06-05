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
	//c_str == string�� char������ ��ȯ
	vTemp = TXTDATA->txtLoad(name.c_str());

	string str;
	int count = 0;

	for (int i = 0; i < vTemp.size(); ++i)
	{
		//���࿡ vTemp�� �޸��忡 �ִ� |�� ������
		if (vTemp[i] == "|")
		{
			elements* em = new elements;
			//str �� | ������°�� �־��
			str = vTemp[i + 1];

			_mTotalElement.insert(pair<string,
				elements*>(vTemp[i + 1], em));

			//���࿡ i�� 0�̾ƴϸ�
			//���� |(7��°)�ִ� ������ �о��
			if (i != 0) count += 7;
			continue;
		}
		iterElement mIter = _mTotalElement.find(str);

		//�����̸�
		if (i == count + 1) mIter->second->name = vTemp[i].c_str();
		//����ü��
		else if (i == count + 2) mIter->second->currentHP = atoi(vTemp[i].c_str());
		//�ִ�ü��
		else if (i == count + 3) mIter->second->maxHP = atoi(vTemp[i].c_str());
		//����
		else if (i == count + 4) mIter->second->angle = (float)atof(vTemp[i].c_str());
		//�ִ뽺�ǵ�
		else if (i == count + 5) mIter->second->maxSpeed = (float)atof(vTemp[i].c_str());
		//���ӵ�
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