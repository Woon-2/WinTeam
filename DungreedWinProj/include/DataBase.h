#pragma once
#ifndef _database
#define _database
#include "FileUtility.h"
#include "Uncopyable.h"
#include <map>

#define Call_End() _End(in.file_name)
#define _End(db_name) ConfirmCheckListAndSetEndFlag(db_name)
#define End() _End(const TCHAR* db_name)
#define Dy_P_Cast std::dynamic_pointer_cast
#define At_TStr static_cast<TCHAR*>(field_inst)
#define At_Pt *static_cast<POINT*>(field_inst)
#define At_Int *static_cast<int*>(field_inst)
typedef std::pair<const std::string, const std::string> Str_Str_Pair;
typedef void* const Field_Inst;
typedef std::map<const std::string, Field_Inst> DB_Map;
typedef class DB_ReadMetIndividual DB_RMI;




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


typedef std::shared_ptr<DB_Data> Data_Inst;





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
	const DB_Map* p_db_map;
	CheckList<const std::string> assign_checklist;


	void MakeCheckList();
	void End();
	void InterpretLine(std::string line);
	const std::string GetLine(IFStream& in) const;

	bool hasLineSpace(const std::string& line) const;

	void AssignInstance(Field_Inst field_instance, Data_Inst p_data_instance) const;
	void AssignByCmdLine(std::string line);

	Str_Str_Pair Seperate(std::string line) const;
	Field_Inst GetFieldInstance(std::string field, const DB_Map& db_map) const;
	Data_Inst GetDataInstance(std::string data) const;

public:
	DB_ReadMetIndividual(DB_Map& db_map);
	DB_ReadMetIndividual(const DB_RMI& source);
	DB_RMI& operator=(const DB_RMI& source);
	void operator()(IFStream& in);
};






class DataBase
{
private:
	TCHAR db_file_name[FILE_NAME_LEN];
	DB_Map db_map;
	const IDConfig id_config;
	DB_RMI ReadMetIndividual;

public:
	DataBase(const TCHAR* db_file_name);

	void RegisterField(std::string field_name, Field_Inst field_address);

	void LoadDataByFirstID();
	void LoadDataByID(const int id);
};

#endif