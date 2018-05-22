#include "stdafx.h"
#include "Query.h"

using namespace std;

unsigned int Query::RSHash(const string& str)
{
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		hash = hash * a + str[i];
		a = a * b;
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int Query::JSHash(const string& str)
{
	unsigned int hash = 1315423911;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		hash ^= ((hash << 5) + str[i] + (hash >> 2));
	}

	return (hash & 0x7FFFFFFF);
}


void Query::SetQueryText(const string & str)
{
	this->Text = str;
	if(str != "")
		this->TextHash = JSHash(str);
	//this->TextHash = RSHash(str);
}

Query::Query()
{
	this->Text = "";
	this->Plan = "";
	this->Run_Time = -1;
	this->Statement_Num = 0;
	this->TextHash = 0;
}


Query::~Query()
{
}
