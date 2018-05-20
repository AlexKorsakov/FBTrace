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
	this->filename = input;
	this->cursor = 0;
	this->events_parsed = 0;
	this->isReading = false;
}

void Parser::ParseEvent(mutex &m) {
	this->events_parsed = 0;
	this->isReading = true;
	this_thread::sleep_for(chrono::milliseconds(100));
	while (true) {
		cout << "ololo\r\n";
		if (this->events_parsed >= this->cursor) {
			this_thread::sleep_for(chrono::milliseconds(100));
			m.lock();
			this->events_parsed = this->cursor;
			m.unlock();
		}
		else {
			for (int i = this->cursor - this->events_parsed;
				i <= this->cursor; this->events_parsed++) {
				//������ ������� �����

			}
		}
		if (!this->isReading)
			break;
	}
}
void Parser::ReadLog(mutex &m) {
	regex regex_timestamp(R_DATE);

	vector<string> lines_of_event;
	bool read_event = false;

	Event ev;
	ifstream file(this->filename);
	this->isReading = true;
	int i = 0;
	for (string line; getline(file, line); i++) {
		smatch match;

		if (regex_search(line, match, regex_timestamp)) {
			ev.Lines = lines_of_event;
			this->Events.push_back(ev);
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
		this->cursor = i;
		m.unlock();
	}
	cout << "Reading is done...\r\n";
	file.close();
	this->isReading = false;
}

//��������� ������ �� �����, ���������� ������ string
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

//����������� ������ string � ������ string (����� ���������� �� ��������)
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
