#pragma once
#include <string>
#include <list>
#include <vector> 
#include "Query.h"

using namespace std;

class EventInfo
{
public:
	string DB_Address;
	string DB_info;
	string Client_Address;
	string Client_info;
	Query query;

	unsigned short int GetQueryRuntime();
	EventInfo();
	~EventInfo();
};

