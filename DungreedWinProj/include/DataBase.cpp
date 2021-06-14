#include "DataBase.h"

DB::Catching_Return<POINT> DB::GetPointFromStr(const std::string& str)
{
	int idx = str.find(" ");
	if (idx == std::string::npos)
		return false;
	for (const char& c : str)
		if (!isdigit(c) && c != ' ')
			return false;

	return POINT{ std::stoi(str.substr(0, idx)), std::stoi(str.substr(idx + 1, str.length())) };
}

DB::Catching_Return<int> DB::GetIntFromStr(const std::string& str)
{
	for (const char& c : str)
		if (!isdigit(c))
			return Catching_Return<int>{false};
	return std::stoi(str);
}

DB::Catching_Return<int> DB::IDConfig::Acc2ID()
{
	if (!std::getline(*in, line))
		return false;
	if (!IsID(line))
		return Acc2ID();
	cur_id = std::stoi(line);
	return cur_id;
}

DB::Catching_Return<int> DB::IDConfig::Acc2ID(const int id)
{
	auto acc = Acc2ID();
	if (acc.return_success)
		if (acc.inst == id)
			return id;
		else
			return Acc2ID(id);
	else
		return false;
}

DB::Catching_Return<int> DB::IDConfig::GetNextID()
{
	int stored_cur_id = cur_id;
	auto next_id = Acc2ID();
	in->clear();
	Acc2ID(stored_cur_id);
	return next_id;
}

bool DB::IDConfig::IsID(const std::string& str) const
{
	for (const char& c : str)
		if (!isdigit(c))
			return false;
	if (str.length() == ID_DIGIT)
		return true;
	else
		return false;
}

void DB::DataBase::RegisterField(const std::string& field_name, void* const field_addr)
{
	data_kit.push_back({ field_name, field_addr, false });
}

void DB::DataBase::Init()
{
	data_kit.clear();
	in.seekg(0, std::ios::beg);
}

void DB::DataBase::Load()
{
	if (auto has_found_individual_to_load = id_config.Acc2ID())
		LoadIndividual();
	else
		assert(true, L"로드할 ID 못 찾음");
}

void DB::DataBase::Load(const int id)
{
	if (auto has_found_object_to_load = id_config.Acc2ID(id))
		LoadIndividual();
	else
		assert(true, L"로드할 ID 못 찾음");
}

void DB::DataBase::LoadIndividual()
{
	while (std::getline(in, line) && !id_config.IsID(line))
		InterpretLine();
	CompleteCheck();
	in.seekg(-static_cast<int>(line.length() + 2), std::ios::cur);
}

void DB::DataBase::InterpretLine()
{
	int idx = line.find(" ");
	if (idx != std::string::npos) {
		auto field_inst = GetFieldInst(line.substr(0, idx));
		auto data_inst = GetDataInst(line.substr(idx + 1, line.length()));
		if (field_inst.return_success)
			Match(field_inst, data_inst);
	}
}

DB::Catching_Return<void*> DB::DataBase::GetFieldInst(std::string field_str)
{
	for (auto acc_tuple : data_kit)						// 튜플 0. 필드 이름 1. 필드 주소 2. 로드 여부
		if (std::get<0>(acc_tuple) == field_str) {
			std::get<2>(acc_tuple) = true;
			return std::get<1>(acc_tuple);
		}
	return false;
}

const DB::Data_Inst DB::DataBase::GetDataInst(std::string data_str) const
{
	auto catch_point_first = GetPointFromStr(data_str);
	if (catch_point_first.return_success)
		return Data_Inst{ new DB_Point{catch_point_first.inst} };

	auto catch_int_second = GetIntFromStr(data_str);
	if (catch_int_second.return_success)
		return Data_Inst{ new DB_Int{catch_int_second.inst} };

	TCHAR catch_tstr_third[DEF_STR_LEN];
	Str2Tstr(data_str, catch_tstr_third);
	return Data_Inst{ new DB_String{catch_tstr_third} };
}

void DB::DataBase::Match(void* addr, const Data_Inst& inst)
{
	if (std::dynamic_pointer_cast<DB_String>(inst))
		lstrcpy(static_cast<TCHAR*>(addr), std::dynamic_pointer_cast<DB_String>(inst)->data);

	else if (std::dynamic_pointer_cast<DB_Point>(inst))
		*static_cast<POINT*>(addr) = std::dynamic_pointer_cast<DB_Point>(inst)->data;

	else if (std::dynamic_pointer_cast<DB_Int>(inst))
		*static_cast<int*>(addr) = std::dynamic_pointer_cast<DB_Int>(inst)->data;
}

void DB::DataBase::CompleteCheck() const
{
	for (auto iter : data_kit)
		assert(!std::get<2>(iter), L"로드되지 않은 데이터 존재");
}