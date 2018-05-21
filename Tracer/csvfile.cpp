#include "stdafx.h"
#include "csvfile.h"




csvfile::~csvfile()
{
	flush();
	fs_.close();
}

void csvfile::flush()
{
	fs_.flush();
}

void csvfile::endrow()
{
	fs_ << std::endl;
	is_first_ = true;
}

csvfile& csvfile::operator << (csvfile& (*val)(csvfile&))
{
	return val(*this);
}

csvfile& csvfile::operator << (const char * val)
{
	return write(escape(val));
}

csvfile& csvfile::operator << (const std::string & val)
{
	return write(escape(val));
}

template<typename T> csvfile& csvfile::operator << (const T& val)
{
	return write(val);
}

template<typename T> csvfile& csvfile::write(const T& val)
{
	if (!is_first_)
	{
		fs_ << separator_;
	}
	else
	{
		is_first_ = false;
	}
	fs_ << val;
	return *this;
}

std::string csvfile::escape(const std::string & val)
{
	std::ostringstream result;
	result << '"';
	std::string::size_type to, from = 0u, len = val.length();
	while (from < len &&
		std::string::npos != (to = val.find_first_of(special_chars_, from)))
	{
		result << val.substr(from, to - from) << escape_seq_ << val[to];
		from = to + 1;
	}
	result << val.substr(from) << '"';
	return result.str();
}