#include "stdafx.h"
#include "txtData.h"


txtData::txtData()
{
}


txtData::~txtData()
{

}

HRESULT txtData::init()
{
	return S_OK;
}

void txtData::release()
{

}


//Save
void txtData::txtSave(const char* saveFileName, vector<string> vStr)
{
	HANDLE file;

	char str[128];
	DWORD write;
	//strncpy_s = 문자열을 복사하는 함수(strncpy와는 같지만 다름 차이는 뒤에 NULL값을 넣냐가 다름)
	//벡터를 결합하다.
	strncpy_s(str, 128, vectorArrayCombine(vStr), 126);
	//                               쓰기권환                     항상새로운파일   속성지원NO
	file = CreateFile(saveFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//strlen 등 문자열 길이 함수를 쓰면 먹통된다
	WriteFile(file, str, 128, &write, NULL);

	CloseHandle(file);
}

char* txtData::vectorArrayCombine(vector<string> vArray)
{
	//버퍼를 정해두지 말고, 속성이 들어오면 속성이 들어온만큼
	//잡을 수 있게 고쳐봅시다 ^_^*

	//버퍼는 우선 128로 한다. 나중에 늘여도 무관
	char str[128];

	ZeroMemory(str, sizeof(str));
	//X Axis, Y Axis, CurrentHP, MaxHP
	//100, 100, 100, 100
	for (int i = 0; i < vArray.size(); i++)
	{
		strncat_s(str, 128, vArray[i].c_str(), 126);
		if (i + 1 < vArray.size()) strcat(str, ",");
	}

	return str;
}


//Load
vector<string> txtData::txtLoad(const char* loadFileName)
{
	HANDLE file;

	char str[128];
	DWORD read;
	//                 파일이름      쓰기 권환  공유모드 ,상속(자식)에게 값을 넘길것인가, 항상 새로운 파일을 만든다, 모든 속성(읽기 전용, 숨김, 운영체제전용)을 지정하지 않는다, 생성된 템플릿 파일의 유효한 핸들에 속성을 넣는 BOOL값
	file = CreateFile(loadFileName, GENERIC_READ, FALSE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//DWORD = 32비트 64비트 관계없이 컴파일하여 1개를 나타내는 것
	//대상파일의 핸들, 데이터가 들어있는 버퍼쓰고자 하는 바이트수, 쓰고자 하는 바이트 수를 리턴하기위한DWORD변수의 포인터, 비동기입출력을 할때 사용 BOOL값
	ReadFile(file, str, 128, &read, NULL);

	//열려진 핸들을 닫는다.(버퍼, 프로세스, 파일 등) WIN32환경에서 핸들로 표현되는 모든 것을 닫는다.
	//그러나 아이콘, 펜 브러쉬등 GDI오브젝트 는 닫을 수 없다.
	CloseHandle(file);

	return charArraySeparation(str);
}

//안의 데이터 값을 잘게잘게 나눌수 있는 만큼 나누자
vector<string> txtData::charArraySeparation(char charArray[])
{
	vector<string> vArray;

	char* temp;
	const char* separator = ",";
	char* token;

	token = strtok(charArray, separator);
	vArray.push_back(token);

	while (NULL != (token = strtok(NULL, separator)))
	{
		vArray.push_back(token);
	}

	return vArray;
}

