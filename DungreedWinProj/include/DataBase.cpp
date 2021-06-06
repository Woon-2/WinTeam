#include "DataBase.h"

DataBase::DataBase(const TCHAR* file_name)
{
	lstrcpy(db_file_name, file_name);
}

void DataBase::RegisterField(std::string field_name, void* field_address)
{
	db_map.insert({ field_name, field_address });
}

void DataBase::LoadDataByFirstID()
{
	CheckFileNameValidity(db_file_name);
	IFStream in{ db_file_name };

	id_config.FindFirstSubject(in);
	ReceiveSubject(in);
}

void DataBase::LoadDataByID(const int id)
{
	CheckFileNameValidity(db_file_name);
	IFStream in{ db_file_name };

	id_config.FindSubject(in, id);
	ReceiveSubject(in);
}






DB_ReceiveSubject::DB_ReceiveSubject(std::map<std::string, void*>& db_map) : p_db_map{ &db_map }
{

}

DB_ReceiveSubject::DB_ReceiveSubject(const DB_ReceiveSubject& source) : p_db_map{ source.p_db_map }
{

}

DB_ReceiveSubject& DB_ReceiveSubject::operator=(const DB_ReceiveSubject& source)
{
	this->p_db_map = p_db_map;
	return *this;
}

void DB_ReceiveSubject::operator()(IFStream& in)
{
	try {
		std::string line = GetLine(in);

		if (IsLineFieldWithData(line)) {
			auto FieldAndData = Seperate(line);
			auto field_instance = GetFieldInstance(FieldAndData.first, *p_db_map);
			auto data_instance = GetDataInstance(FieldAndData.second);

			Match(field_instance, data_instance);

			// data_instance는 new로 생성되었음!!
			delete data_instance;
		}

		operator()(in);
	}
	catch (const int END_OF_SUBJ) {}
}

const std::string DB_ReceiveSubject::GetLine(IFStream& in)
{
	std::string line;

	if (!std::getline(in, line))
		throw END_OF_SUBJ;
	if (id_config.IsLineID(line))
		throw END_OF_SUBJ;

	return line;
}

std::pair<const std::string, const std::string> DB_ReceiveSubject::Seperate(std::string line)
{
	std::string field = GetHeadString(line);
	std::string data = GetRestString(line);
	return std::pair<const std::string, const std::string>{field, data};
}

bool DB_ReceiveSubject::IsLineFieldWithData(const std::string& line)
{
	if (GetHeadString(line) == line)
		return false;
	else
		return true;
}

void* DB_ReceiveSubject::GetFieldInstance(std::string field, const std::map<std::string, void*>& db_map)
{
	for (auto iter : db_map) {
		std::string acc_str = iter.first;
		void* acc_addr = iter.second;

		if (acc_str == field)
			return acc_addr;
	}
}

DB_Data* DB_ReceiveSubject::GetDataInstance(std::string data)
{
	std::string args_origin[2];
	try {
		args_origin[1] = GetRestString(data);
		// GetRestString은 매개변수 string에 띄어쓰기가 없으면 오류를 던짐, 따라서 인자의 개수 구분 가능
		args_origin[0] = GetHeadString(data);
		return new DB_Point{ std::stoi(args_origin[0]), std::stoi(args_origin[1]) };
	}
	catch (const TCHAR* error_message) {
		if (IsStringInt(data))
			return new DB_Int(std::stoi(data));
		else {
			TCHAR rearranged_data[DEF_STR_LEN];
			str2Tstr(rearranged_data, data);
			return new DB_String(rearranged_data);
		}
	}
}

void DB_ReceiveSubject::Match(void* field_instance, DB_Data* p_data_instance)
{
	if (dynamic_cast<DB_String*>(p_data_instance))
		lstrcpy(static_cast<TCHAR*>(field_instance), dynamic_cast<DB_String*>(p_data_instance)->data);

	else if (dynamic_cast<DB_Point*>(p_data_instance))
		*static_cast<POINT*>(field_instance) = dynamic_cast<DB_Point*>(p_data_instance)->data;

	else if (dynamic_cast<DB_Int*>(p_data_instance))
		*static_cast<int*>(field_instance) = dynamic_cast<DB_Int*>(p_data_instance)->data;
}






void IDConfig::FindFirstSubject(IFStream& in)
{
	std::string line;
	while (std::getline(in, line))
		if (IsLineID(line))
			return;

	TCHAR error_message[DEF_STR_LEN];
	wsprintf(error_message, L"Cannot Find Any ID On \"%s\"", in.file_name);
	throw error_message;
}

void IDConfig::FindSubject(IFStream& in, const int subject_id)
{
	std::string line;
	while (std::getline(in, line))
		if (IsLineEqualsID(line, subject_id))
			return;

	TCHAR error_message[DEF_STR_LEN];
	wsprintf(error_message, L"ID \"%d\" Not Found On \"%s\"", subject_id, in.file_name);
	throw error_message;
}

bool IDConfig::IsLineID(const std::string& line)
{
	if (IsStringInt(line))
		if (IsNumID(std::stoi(line)))
			return true;
	return false;
}

bool IDConfig::IsLineEqualsID(const std::string& line, const int id)
{
	if (IsLineID(line))
		if (std::stoi(line) == id)
			return true;
	return false;
}

bool IDConfig::IsNumID(const int val)
{
	int id_head = val / pow(10, ID_DIGIT - 1);

	if (id_head >= 1 && id_head <= 9)
		return true;
	else
		return false;
}