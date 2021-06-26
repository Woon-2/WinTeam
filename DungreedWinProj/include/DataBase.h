#pragma once
#ifndef _database
#define _database
#include <string>
#include <tuple>
#include <vector>
#include <fstream>
#include <cctype>
#include <tchar.h>
#include <windows.h>
#include "Uncopyable.h"
#include "FileUtility.h"
#include <cassert>

namespace DB {
	constexpr int DEF_STR_LEN = 200;

	template<typename T>
	class Catching_Return		// 자유도가 높은 리턴 타입, 리턴이 실패하면 원래 타입 대신 return_success = false 를 반환한다
	{
	public:
		T inst;
		bool return_success = true;
		Catching_Return(T inst) : inst{ inst } {}
		Catching_Return(bool return_success) : return_success{ return_success } {}
		operator T() { return inst; }
		operator bool() { return return_success; }
	};

	Catching_Return<POINT> GetPointFromStr(const std::string& str);
	Catching_Return<int> GetIntFromStr(const std::string& str);

	// ------------------------------------------------- generic programming으로 개선할 부분 -----------
	// 가변인자 템플릿으로 데이터베이스 클래스를 생성하도록 한다.
	// 데이터베이스의 템플릿 인자는 데이터베이스에 쓰일 자료형이다.
	// 데이터베이스의 생성자의 인자로 []<typename T>(args){ return T(my_args); } 를 받는다.
	// 	   이때 args는 기본 자료형의 인자여야 한다. my_args는 args를 그대로 쓰거나 T 생성자에 쓰이는 인자로 변형하면 된다.
	// 데이터베이스의 한 튜플은 0.필드의 type_info 1. 필드의 주소 2. 로드 여부로 구성되도록 한다.
	// 로드는 field_type_info.name() == typeid(T).name()일 때 *static_cast<T*>(tuple_addr) = []<typename T>(args); 를 실행한다.
	// -----------------------------------------------------------------------------------------------

	interface DB_Data
	{
	public:
		virtual ~DB_Data() {}
	};

	class DB_Point : public DB_Data
	{
	public:
		POINT data;

		DB_Point(const int x, const int y) : data{ POINT{ x, y } } {}
		DB_Point(POINT pt) : data{ pt } {}
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

	// -----------------------------------------------------------------------------------------------




	// 튜플 0. 필드 이름 1. 필드 주소 2. 로드 여부
	typedef std::vector<std::tuple<std::string, void*, bool>> DataKit;

	class IDConfig : private Uncopyable
	{
	private:
		const int ID_DIGIT = 7;
		std::ifstream* in;
		std::string line;
		int cur_id = 0;

		IDConfig(const IDConfig& other) {}
		IDConfig& operator=(const IDConfig& other) {}

	public:
		IDConfig() = default;

		inline void AllocateFstream(std::ifstream* in) { this->in = in; }
		Catching_Return<int> Acc2ID();
		Catching_Return<int> Acc2ID(const int id);
		Catching_Return<int> GetNextID();
		bool IsID(const std::string& str) const;
		inline int GetCurID() const { return cur_id; }
	};

	class DataBase : private Uncopyable
	{
	private:
		DataKit data_kit;
		IDConfig id_config;

		std::ifstream in;
		std::string line;

		void LoadIndividual();

		void InterpretLine();

		Catching_Return<void*> GetFieldInst(std::string field_str);
		const Data_Inst GetDataInst(std::string data_str) const;
		void Match(void* addr, const Data_Inst& inst);

		void CompleteCheck() const;

	public:
		DataBase(const TCHAR* db_name) : in{ db_name } { id_config.AllocateFstream(&in); }

		void RegisterField(const std::string& field_name, void* const field_addr);

		void Init();
		void Load();
		void Load(const int id);
	};

}

#endif