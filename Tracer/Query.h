#pragma once
#include <string>

using namespace std;

class Query
{
public:
	unsigned short int Statement_Num;
	string Text;
	string Plan;
	unsigned short int Run_Time;

	Query();
	~Query();
};

