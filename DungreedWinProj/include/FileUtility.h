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
#include "Uncopyable.h"

const int FILE_NAME_LEN = 200;
const int DEF_STR_LEN = 200;



class IFStream : private Uncopyable, public std::ifstream
{
public:
	TCHAR file_name[FILE_NAME_LEN];
	IFStream() = default;
	IFStream(const TCHAR* file_name) : std::ifstream(file_name) { lstrcpy(this->file_name, file_name); }
};



class Image : public CImage
{
private:
	TCHAR image_name[FILE_NAME_LEN];

	void Load(const TCHAR* file_name);

public:
	Image(const TCHAR* file_name);
	Image(const Image& source);
	Image& operator=(const Image& source);
	~Image();
};



template<class T>
class CheckList : private Uncopyable
{
private:
	std::map<T, bool> cl_map;

public:
	CheckList() = default;
	template<class R> CheckList(const std::map<T, R>& map);

	template<class R> void Make(const std::map<T, R>& map);
	void RegisterKey(T key);
	void Check(const T& key);
	bool IsCompletelyChecked() const;
};




template <class T>
template <class R>
CheckList<T>::CheckList(const std::map<T, R>& map)
{
	Make(map);
}

template <class T>
template <class R>
void CheckList<T>::Make(const std::map<T, R>& map)
{
	for (const std::pair<T, R>& pair : map)
		RegisterKey(pair.first);
}

template <class T>
void CheckList<T>::RegisterKey(T key)
{
	cl_map.insert({ key, false });
}

template <class T>
void CheckList<T>::Check(const T& key)
{
	cl_map[key] = true;
}

template <class T>
bool CheckList<T>::IsCompletelyChecked() const
{
	for (const std::pair<const T, bool>& pair : cl_map)
		if (!pair.second)
			return false;
	return true;
}





void CheckFileNameValidity(const TCHAR* file_name);
bool IsStringInt(const std::string& str);
const std::string GetHeadString(const std::string& line);
const std::string GetRestString(const std::string& line);
void str2Tstr(const std::string& str, TCHAR t_str[]);

#endif