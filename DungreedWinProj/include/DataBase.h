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
	void MeetFirstID(IFStream& in) const;
	void MeetID(IFStream& in, const int subject_id) const;
	bool IsLineID(const std::string& str) const;
	bool IsLineEqualsID(const std::string& line, const int id) const;
	bool IsNumID(const int val) const;
};





class DB_ReadMetIndividual : private Uncopyable
{
private:
	const int END_OF_SUBJ = 1234;
	const int INVALID_FIELD = 4321;

	bool begin_flag;
	bool end_flag;

	const IDConfig id_config;
	const std::map<const std::string, void* const>* p_db_map;
	CheckList<const std::string> assign_checklist;


	void Begin();
	void End(const TCHAR* db_name);
	void InterpretLine(std::string line);
	const std::string GetLine(IFStream& in) const;

	bool hasLineSpace(const std::string& line) const;

	void AssignInstance(void* const field_instance, std::shared_ptr<DB_Data> p_data_instance) const;
	void AssignByCmdLine(std::string line);

	std::pair<const std::string, const std::string> Seperate(std::string line) const;
	void* const GetFieldInstance(std::string field, const std::map<const std::string, void* const>& db_map) const;
	std::shared_ptr<DB_Data> GetDataInstance(std::string data) const;

public:
	DB_ReadMetIndividual(std::map<const std::string, void* const>& db_map);
	DB_ReadMetIndividual(const DB_ReadMetIndividual& source);
	DB_ReadMetIndividual& operator=(const DB_ReadMetIndividual& source);
	void operator()(IFStream& in);
};






class DataBase
{
private:
	TCHAR db_file_name[FILE_NAME_LEN];
	std::map<const std::string, void* const> db_map;
	const IDConfig id_config;
	DB_ReadMetIndividual ReadMetIndividual;

public:
	DataBase(const TCHAR* db_file_name);

	void RegisterField(std::string field_name, void* const field_address);

	void LoadDataByFirstID();
	void LoadDataByID(const int id);
};

#endif