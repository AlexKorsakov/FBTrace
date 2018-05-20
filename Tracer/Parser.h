#pragma once
#include <string>
#include <list>
#include <vector> 
#include "Event.h"
#include <mutex>

using namespace std;

static class Parser
{
public:
	static vector <Event> Events;
	static bool isReading;
	static string filename;
	static unsigned int cursor;
	static unsigned int events_parsed;
	//int string_count;

	Parser(string input);
	static void ParseEvent(mutex &m);
	static void ReadLog(mutex &m);
	static list<string> GetListWords(string input);
	static vector<string> GetStringVectorFromList(list<string> input);
	~Parser();
	//регулярки для парсинга лога
	static string R_DATE;
	static string R_TIME;
	static string R_TIMESTAMP;

	static string R_STATEMENT_N;
	static string R_PLAN;
};

