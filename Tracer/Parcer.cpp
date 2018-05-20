#include "stdafx.h"
#include "Parser.h"
#include "string.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

Parser::Parser(string input)
{
	filename = input;
	cursor = 0;
	events_parsed = 0;
	isReading = false;

	 R_DATE = "([0-9]{4}-(0[1-9]|1[012])-(0[1-9]|1[0-9]|2[0-9]|3[01]))";
	 R_TIME = "([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9].[0-9]{4}";
	 R_TIMESTAMP = R_DATE + "T" + R_TIME;

	 R_STATEMENT_N = "Statement [0-9]{2,}:";
	 R_PLAN = "PLAN+(+)";
}

void Parser::ParseEvent(mutex &m) {
	events_parsed = 0;
	isReading = true;
	this_thread::sleep_for(chrono::milliseconds(100));
	while (true) {
		cout << "ololo\r\n";
		if (events_parsed >= cursor) {
			this_thread::sleep_for(chrono::milliseconds(100));
			m.lock();
			events_parsed = cursor;
			m.unlock();
		}
		else {
			for (int i = cursor - events_parsed;
				i <= cursor; events_parsed++) {
				//парсим событие далее

			}
		}
		if (!isReading)
			break;
	}
}

//Читает лог-файл
void Parser::ReadLog(mutex &m) {
	regex regex_timestamp(R_DATE);
	
	vector<string> lines_of_event;
	bool read_event = false;

	Event ev;
	ifstream file(filename);
	isReading = true;
	int i = 0;
	for (string line; getline(file, line); i++) {
		smatch match;

		if (regex_search(line, match, regex_timestamp)) {			
			ev.Lines = lines_of_event;
			Events.push_back(ev);
			list<string> words = GetListWords(line);
			vector<string> str_array = GetStringVectorFromList(words);
			ev = Event(str_array);
			lines_of_event.clear();
		}
		else {
			bool line_is_slash_t = (line[0] == '\t' && line.size() == 1);
			bool line_is_empty = (line == "" && line.size() == 0);
			if (!(line_is_slash_t xor line_is_empty)) {
				/*
				regex regex_statement(R_STATEMENT_N);
				if(regex_search(line, match, regex_statement)) {
					//cout << line;
				}
				*/
				lines_of_event.push_back(line);
			}
		}
		m.lock();
		cursor = i;
		m.unlock();
	}
	cout << "Reading is done...\r\n";
	file.close();
	isReading = false;
}

//Разбивает строку на слова, возвращает список string
list<string> Parser::GetListWords(string input)
{
	list<string> words;
	input += " ";
	string subline = "";
	for (int i = 0; i < input.length(); i++) {
		if (input[i] != ' ')
			subline += input[i];
		else {
			string word(subline);
			words.push_back(subline);
			subline = "";
		}
	}
	return words;
}

//Преобразует список string в вектор string (чтобы обратиться по индексам)
vector< string > Parser::GetStringVectorFromList(list<string> input) {
	vector<string> str_vect(input.size());
	int i = 0;
	for (list<string>::iterator iter = input.begin(); iter != input.end(); ++iter)
	{
		std::advance(iter, 0);
		str_vect[i] = *iter;
		i++;
	}
	return str_vect;
}



Parser::~Parser()
{
}
