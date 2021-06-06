#pragma once
#ifndef _fileutility
#define _fileutility
#include <fstream>
#include <windows.h>
#include <atlimage.h>
#include <tchar.h>
#include <string>
#include <math.h>
#include <map>
//
#include <iostream>
//

const int ID_DIGIT = 7;
const int FILE_NAME_LEN = 200;
const int DEF_STR_LEN = 200;

class DB_Data;
class DB_String;
class DB_Point;
class DB_Int;

class Image : public CImage
{
private:
	TCHAR image_name[FILE_NAME_LEN];
public:
	Image(const TCHAR* file_name);
	Image(const Image& source);
	Image& operator=(const Image& source);
	~Image();

	void Load(const TCHAR* file_name);
};

class DataBase
{
private:
	TCHAR db_file_name[FILE_NAME_LEN];
	std::map<std::string, void*> db_map;

	void PutFileCursorOnFirstID(std::ifstream& in);
	void PutFileCursorOnID(std::ifstream& in, const int id);
	bool IsID(const std::string& id_string);
	void LoadDataAheadFileCursor(std::ifstream& in);
	void Interpret(std::string line);
	bool IsLineFieldWithData(const std::string& line);
	void Match(std::string field, std::string data);
	DB_Data* GetDataInstance(std::string data);

public:
	DataBase(const TCHAR* db_file_name);

	void RegisterField(std::string field_name, void* field_address);

	void LoadDataWithFirstID();
	void LoadDataWithID(const int id);
};

class DB_Data
{
public:
	virtual ~DB_Data() {}
};

class DB_Point : public DB_Data
{
public:
	POINT data;

	inline DB_Point(const int x, const int y) : data{ POINT{ x, y } } {}
};

class DB_String : public DB_Data
{
public:
	TCHAR data[DEF_STR_LEN];

	inline DB_String(const TCHAR* str) { lstrcpy(data, str); }
};

class DB_Int : public DB_Data
{
public:
	int data;

	inline DB_Int(const int val) : data{ val } {}
};

void CheckFileNameValidity(const TCHAR* file_name);
bool IsStringInt(const std::string& str);
const std::string GetHeadString(const std::string& line);
const std::string GetRestString(const std::string& line);
void str2Tstr(TCHAR t_str[], const std::string& str);
#endif