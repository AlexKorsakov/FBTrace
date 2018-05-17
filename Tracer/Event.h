#pragma once
#include <string>
#include <list>
#include <vector> 

using namespace std;

class Event
{
public:
	string Type;
	string PID;
	string Trace_number;
	string Timestamp;
	vector <string> Lines;


	Event();
	Event(vector<string> &event_string);
	bool IsEmpty();
	~Event();
};

