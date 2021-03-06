#pragma once
#include <string>
#include <list>
#include <vector> 
#include "Event.h"
#include <mutex>

using namespace std;

class Parser
{
public:
	vector <Event> Events;
	bool isReading;
	string filename;
	unsigned int cursor;
	unsigned int events_parsed;
	//int string_count;

	Parser(string input);
	void ParseEvent(mutex &m);
	void ReadLog(mutex &m);
	list<string> GetListWords(string input);
	vector<string> GetStringVectorFromList(list<string> input);
	string DeleteParamsFromQuery(string input);
	string GetDBNameFromAddress(string input);
	void CoutParseStats(mutex &m);
	~Parser();
	//��������� ��� �������� ����
	string R_DATE = "([0-9]{4}-(0[1-9]|1[012])-(0[1-9]|1[0-9]|2[0-9]|3[01]))";
	string R_TIME = "([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9].[0-9]{4}";
	string R_TIMESTAMP = R_DATE + "T" + R_TIME;

	string R_DBINFO = "\.FDB";
	string R_TR_INFO = "TRA_[0-9]{0,6}";
	string R_STATEMENT_N = "Statement [0-9]{2,}:";
	string R_PLAN = "PLAN\\s+";
	string R_QUERY_s = "-{79}";
	string R_QUERY_e = "\\^{79}";
	string R_RUNTIME = "\\d+\\sms";
};

