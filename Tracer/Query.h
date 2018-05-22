#pragma once
#include <string>

using namespace std;

class Query
{
public:
	string Text;
	string Plan;
	unsigned int TextHash;
	unsigned short int Statement_Num;
	short int Run_Time;

	void SetQueryText(const string& str);
	Query();
	~Query();
private:
	unsigned int RSHash(const string& str);
	unsigned int JSHash(const string& str);
};

