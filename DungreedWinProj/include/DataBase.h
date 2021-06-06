#pragma once
#ifndef _database
#define _database
#include "FileUtility.h"
#include "Uncopyable.h"
#include <map>


class DB_Data
{
public:
	virtual ~DB_Data() {}
};

class DB_Point : public DB_Data
{
public:
	POINT data;

	DB_Point(const int x, const int y) : data{ POINT{ x, y } } {}
};

class DB_String : public DB_Data
{
public:
	TCHAR data[DEF_STR_LEN];

	DB_String(const TCHAR* str) { lstrcpy(data, str); }
};

class DB_Int : public DB_Data
{
public:
	int data;

	DB_Int(const int val) : data{ val } {}
};





class IDConfig : private Uncopyable
{
private:
	const int ID_DIGIT = 7;

public:
	void FindFirstSubject(IFStream& in);
	void FindSubject(IFStream& in, const int subject_id);
	bool IsLineID(const std::string& str);
	bool IsLineEqualsID(const std::string& line, const int id);
	bool IsNumID(const int val);
};





class DB_ReceiveSubject : private Uncopyable
{
private:
	IDConfig id_config;
	const int END_OF_SUBJ = 1234;
	std::map<std::string, void*>* p_db_map;

	const std::string GetLine(IFStream& in);

	bool IsLineFieldWithData(const std::string& line);
	std::pair<const std::string, const std::string> Seperate(std::string line);

	void* GetFieldInstance(std::string field, const std::map<std::string, void*>& db_map);
	DB_Data* GetDataInstance(std::string data);

	void Match(void* field_instance, DB_Data* p_data_instance);

public:
	DB_ReceiveSubject() = default;
	DB_ReceiveSubject(std::map<std::string, void*>& db_map);
	DB_ReceiveSubject(const DB_ReceiveSubject& source);
	DB_ReceiveSubject& operator=(const DB_ReceiveSubject& source);
	void operator()(IFStream& in);
};






class DataBase
{
private:
	TCHAR db_file_name[FILE_NAME_LEN];
	std::map<std::string, void*> db_map;
	IDConfig id_config;
	DB_ReceiveSubject ReceiveSubject{ db_map };

public:
	DataBase(const TCHAR* db_file_name);

	void RegisterField(std::string field_name, void* field_address);

	void LoadDataByFirstID();
	void LoadDataByID(const int id);
};

#endif