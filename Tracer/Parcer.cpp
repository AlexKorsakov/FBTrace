#include "stdafx.h"
#include "Parser.h"
#include "string.h"
#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

Parser::Parser(string input)
{
	this->filename = input;
	this->cursor = 0;
}

void Parser::Read() {
	regex regex_timestamp(R_TIMESTAMP);
	
	vector<string> lines_of_event;
	bool read_event = false;

	Event ev;
	ifstream file(this->filename);
	for (string line; getline(file, line); this->cursor++) {
		smatch match;
		
		if (regex_search(line, match, regex_timestamp)) {			
			ev.Lines = lines_of_event;
			this->Events.push_back(ev);

			const string timestamp = match[0];
			list<string> words = GetListWords(line);
			vector<string> str_array = GetStringVectorFromList(words);
			ev = Event(str_array);
			lines_of_event.clear();
		}
		else {
			if (line != "\t" || line != "") {

				regex regex_statement(R_STATEMENT_N);
				if(regex_search(line, match, regex_statement)) {
					//cout << line;
				}
				lines_of_event.push_back(line);
			}
		}
		
		//lines_of_event.push_back(line);
	}
	cout << "Done";
	file.close();
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
