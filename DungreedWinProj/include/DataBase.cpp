#include "DataBase.h"

DataBase::DataBase(const TCHAR* file_name) : ReadMetIndividual{db_map}
{
	lstrcpy(db_file_name, file_name);
}

void DataBase::RegisterField(std::string field_name, void* const field_address)
{
	db_map.insert({ field_name, field_address });
}

void DataBase::LoadDataByFirstID()
{
	CheckFileNameValidity(db_file_name);
	IFStream in{ db_file_name };

	id_config.MeetFirstID(in);
	ReadMetIndividual(in);
}

void DataBase::LoadDataByID(const int id)
{
	CheckFileNameValidity(db_file_name);
	IFStream in{ db_file_name };

	id_config.MeetID(in, id);
	ReadMetIndividual(in);
}







void IDConfig::MeetFirstID(IFStream& in) const
{
	std::string line;
	while (std::getline(in, line))
		if (IsLineID(line))
			return;

	TCHAR error_message[DEF_STR_LEN];
	wsprintf(error_message, L"Cannot Find Any ID On \"%s\"", in.file_name);
	throw error_message;
}

void IDConfig::MeetID(IFStream& in, const int subject_id) const
{
	std::string line;
	while (std::getline(in, line))
		if (IsLineEqualsID(line, subject_id))
			return;

	TCHAR error_message[DEF_STR_LEN];
	wsprintf(error_message, L"ID \"%d\" Not Found On \"%s\"", subject_id, in.file_name);
	throw error_message;
}

bool IDConfig::IsLineID(const std::string& line) const
{
	if (IsStringInt(line))
		if (IsNumID(std::stoi(line)))
			return true;
	return false;
}

bool IDConfig::IsLineEqualsID(const std::string& line, const int id) const
{
	if (IsLineID(line))
		if (std::stoi(line) == id)
			return true;
	return false;
}

bool IDConfig::IsNumID(const int val) const
{
	int id_head = val / pow(10, ID_DIGIT - 1);

	if (id_head >= 1 && id_head <= 9)
		return true;
	else
		return false;
}










DB_ReadMetIndividual::DB_ReadMetIndividual(std::map<const std::string, void* const>& db_map) : p_db_map{ &db_map }, begin_flag{ true }, end_flag{ false }
{

}

DB_ReadMetIndividual::DB_ReadMetIndividual(const DB_ReadMetIndividual& source) : p_db_map{ source.p_db_map }, begin_flag{ true }, end_flag{ false }
{

}

DB_ReadMetIndividual& DB_ReadMetIndividual::operator=(const DB_ReadMetIndividual& source)
{
	this->p_db_map = p_db_map;
	return *this;
}

void DB_ReadMetIndividual::operator()(IFStream& in)
{
	if (begin_flag)
		Begin();
	try {
		InterpretLine(GetLine(in));
	}
	catch (const int error_code) {				// INVALID_FIELD 에러 코드는 굳이 처리하지 않음으로써 DB 작성 자유도를 높임
		if (error_code == END_OF_SUBJ)
			End(in.file_name);
	}

	if (!end_flag)
		operator()(in);
}

void DB_ReadMetIndividual::Begin()
{
	assign_checklist.Make(*p_db_map);
	begin_flag = false;
}

void DB_ReadMetIndividual::End(const TCHAR* db_name)
{
	if (!assign_checklist.IsCompletelyChecked()) {
		TCHAR error_message[DEF_STR_LEN];
		wsprintf(error_message, L"Data Not Completely Loaded In \"%s\"", db_name);
		throw error_message;
	}
	end_flag = true;
}

void DB_ReadMetIndividual::InterpretLine(std::string line)
{
	if (hasLineSpace(line))
		AssignByCmdLine(line);
}

const std::string DB_ReadMetIndividual::GetLine(IFStream& in) const
{
	std::string line;

	if (!std::getline(in, line))
		throw END_OF_SUBJ;
	if (id_config.IsLineID(line))
		throw END_OF_SUBJ;

	return line;
}

void DB_ReadMetIndividual::AssignInstance(void* const field_instance, std::shared_ptr<DB_Data> data_instance) const
{
	if (std::dynamic_pointer_cast<DB_String>(data_instance))
		lstrcpy(static_cast<TCHAR*>(field_instance), std::dynamic_pointer_cast<DB_String>(data_instance).get()->data);

	else if (std::dynamic_pointer_cast<DB_Point>(data_instance))
		*static_cast<POINT*>(field_instance) = std::dynamic_pointer_cast<DB_Point>(data_instance).get()->data;

	else if (std::dynamic_pointer_cast<DB_Int>(data_instance))
		*static_cast<int*>(field_instance) = std::dynamic_pointer_cast<DB_Int>(data_instance).get()->data;
}

void DB_ReadMetIndividual::AssignByCmdLine(std::string line)
{
	auto field_data_pair = Seperate(line);
	auto field_instance = GetFieldInstance(field_data_pair.first, *p_db_map);
	auto data_instance = GetDataInstance(field_data_pair.second);

	AssignInstance(field_instance, data_instance);
	assign_checklist.Check(field_data_pair.first);
}


std::pair<const std::string, const std::string> DB_ReadMetIndividual::Seperate(std::string line) const
{
	std::string field = GetHeadString(line);
	std::string data = GetRestString(line);
	return std::pair<const std::string, const std::string>{field, data};
}

bool DB_ReadMetIndividual::hasLineSpace(const std::string& line) const
{
	if (GetHeadString(line) == line)
		return false;
	else
		return true;
}

void* const DB_ReadMetIndividual::GetFieldInstance(std::string field, const std::map<const std::string, void* const>& db_map) const
{
	for (auto iter : db_map) {
		std::string acc_str = iter.first;
		void* acc_addr = iter.second;

		if (acc_str == field)
			return acc_addr;
	}
	throw INVALID_FIELD;
}

std::shared_ptr<DB_Data> DB_ReadMetIndividual::GetDataInstance(std::string data) const
{
	std::string args_origin[2];
	try {
		args_origin[1] = GetRestString(data);
		// GetRestString은 매개변수 string에 띄어쓰기가 없으면 오류를 던짐, 따라서 인자의 개수 구분 가능
		args_origin[0] = GetHeadString(data);
		return std::shared_ptr<DB_Data>{ new DB_Point{ std::stoi(args_origin[0]), std::stoi(args_origin[1]) } };
	}
	catch (const TCHAR* error_message) {
		if (IsStringInt(data))
			return std::shared_ptr<DB_Data>{ new DB_Int(std::stoi(data)) };
		else {
			TCHAR rearranged_data[DEF_STR_LEN];
			str2Tstr(data, rearranged_data);
			return std::shared_ptr<DB_Data>{new DB_String(rearranged_data)};
		}
	}
}