#include "FileUtility.h"

void Image::Load(const TCHAR* file_name)
{
	CheckFileNameValidity(file_name);
	this->CImage::Load(file_name);
	lstrcpy(image_name, file_name);
}

Image::Image(const TCHAR* file_name)
{
	Load(file_name);
}

Image::Image(const Image& source)
{
	Load(source.image_name);
}

Image& Image::operator=(const Image& source)
{
	if (this == &source)
		return *this;
	Load(source.image_name);
	return *this;
}

Image::~Image()
{
	this->Destroy();
	this->CImage::~CImage();
}







DataBase::DataBase(const TCHAR* file_name)
{
	lstrcpy(db_file_name, file_name);
}

void DataBase::RegisterField(std::string field_name, void* field_address)
{
	db_map.insert({ field_name, field_address });
}

void DataBase::LoadDataWithFirstID()
{
	CheckFileNameValidity(db_file_name);
	std::ifstream in{ db_file_name };
	PutFileCursorOnFirstID(in);
	LoadDataAheadFileCursor(in);
}

void DataBase::LoadDataWithID(const int id)
{
	CheckFileNameValidity(db_file_name);
	std::ifstream in{ db_file_name };
	PutFileCursorOnID(in, id);
	LoadDataAheadFileCursor(in);
}

void DataBase::PutFileCursorOnFirstID(std::ifstream& in)
{
	std::string line;
	while (std::getline(in, line))
		if (IsID(line))
			return;
	
	TCHAR error_message[DEF_STR_LEN];
	wsprintf(error_message, L"Cannot Find First ID At %s", db_file_name);
	throw error_message;
}

void DataBase::PutFileCursorOnID(std::ifstream& in, const int id)
{
	std::string line;
	while (std::getline(in, line))
		if (IsID(line))
			if (std::stoi(line) == id)
				return;

	TCHAR error_message[DEF_STR_LEN];
	wsprintf(error_message, L"Cannot Find Required ID At %s", db_file_name);
	throw error_message;
}

bool DataBase::IsID(const std::string& id_string)
{
	int id_num;
	int id_num_divided_by_id_digit;

	if (!IsStringInt(id_string))
		return false;

	id_num = std::stoi(id_string);
	id_num_divided_by_id_digit = id_num / pow(10, ID_DIGIT - 1);

	if (id_num_divided_by_id_digit >= 1 && id_num_divided_by_id_digit <= 9)
		return true;
	else
		return false;
}

void DataBase::LoadDataAheadFileCursor(std::ifstream& in)
{
	std::string line;
	if(!std::getline(in, line))
		return;

	if (IsID(line))
		return;

	Interpret(line);
	LoadDataAheadFileCursor(in);
}

void DataBase::Interpret(std::string line)
{
	if (IsLineFieldWithData(line)) {
		std::string field;
		std::string data;

		field = GetHeadString(line);
		data = GetRestString(line);
		Match(field, data);
	}
}

bool DataBase::IsLineFieldWithData(const std::string& line)
{
	if (GetHeadString(line) == line)
		return false;
	else
		return true;
}

void DataBase::Match(std::string field, std::string data)
{
	DB_Data* data_instance = GetDataInstance(data);

	for (auto iter : db_map)
		if (iter.first == field) {

			// 오류 처리 해야 함
			if (dynamic_cast<DB_String*>(data_instance))
				lstrcpy(static_cast<TCHAR*>(iter.second), dynamic_cast<DB_String*>(data_instance)->data);

			else if (dynamic_cast<DB_Point*>(data_instance))
				*static_cast<POINT*>(iter.second) = dynamic_cast<DB_Point*>(data_instance)->data;

			else if (dynamic_cast<DB_Int*>(data_instance))
				*static_cast<int*>(iter.second) = dynamic_cast<DB_Int*>(data_instance)->data;

			delete data_instance;
			return;
		}

	delete data_instance;
}

DB_Data* DataBase::GetDataInstance(std::string data)
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







void CheckFileNameValidity(const TCHAR* file_name)
{
	std::ifstream in{ file_name };
	if (!in) {
		TCHAR error_message[200];
		wsprintf(error_message, L"File \"%s\" has not found", file_name);
		throw error_message;
	}
}

bool IsStringInt(const std::string& str)
{
	if (str.empty())
		return false;
	for (int i = 0; i < str.length(); ++i)
		if (str[i] < '0' || str[i] > '9')
			return false;
	return true;
}

const std::string GetHeadString(const std::string& line)
{
	for (int i = 0; i < line.length(); ++i)
		if (line[i] == ' ')
			return line.substr(0, i);
	return line;
}

const std::string GetRestString(const std::string& line)
{
	for (int i = 0; i < line.length(); ++i)
		if (line[i] == ' ')
			return line.substr(i+1);
	throw L"GetRestString Failed";
}

void str2Tstr(TCHAR t_str[], const std::string& str)
{
	for (int i = 0; i < str.length(); ++i)
		t_str[i] = str[i];
	t_str[str.length()] = NULL;
}