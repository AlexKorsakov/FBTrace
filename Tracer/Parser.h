#pragma once
#include <string>
#include <list>
#include <vector> 
#include "Event.h"

using namespace std;

class Parser
{
	public:
		vector <Event> Events;

		string filename;
		int cursor;
		int string_count;

		Parser(string input);
		void Read();
		list<string> GetListWords(string input);
		vector<string> GetStringVectorFromList(list<string> input);
		~Parser();
	private:
		//регулярки для парсинга лога
		string R_DATE = "([0-9]{4}-(0[1-9]|1[012])-(0[1-9]|1[0-9]|2[0-9]|3[01]))";
		string R_TIME = "([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9].[0-9]{4}";
		string R_TIMESTAMP = R_DATE + "T" + R_TIME;
		string R_STATEMENT_N = "Statement [0-9]{2,}:";
};

