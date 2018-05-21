#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class csvfile;
inline static csvfile& endrow(csvfile& file);
inline static csvfile& flush(csvfile& file);

class csvfile
{
	std::ofstream fs_;
	bool is_first_;
	const std::string separator_;
	const std::string escape_seq_;
	const std::string special_chars_;
public:
	csvfile(const std::string filename, const std::string separator = ";")
		: fs_()
		, is_first_(true)
		, separator_(separator)
		, escape_seq_("\"")
		, special_chars_("\"")
	{
		fs_.exceptions(std::ios::failbit | std::ios::badbit);
		fs_.open(filename);
	}

	~csvfile();
	void flush();
	void endrow();
	csvfile& operator << (csvfile& (*val)(csvfile&));
	csvfile& operator << (const char * val);
	csvfile& operator << (const std::string & val);
	template<typename T> csvfile& operator << (const T& val);
private:
	template<typename T> csvfile& write(const T& val);
	std::string escape(const std::string & val);
};


inline static csvfile& endrow(csvfile& file)
{
	file.endrow();
	return file;
}

inline static csvfile& flush(csvfile& file)
{
	file.flush();
	return file;
}

