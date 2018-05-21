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
	this_thread::sleep_for(chrono::milliseconds(100));
	EventInfo ev_info;
	while (true) {
		m.lock();
		int capacity = (int)this->Events.size();
		m.unlock();
		if (this->events_parsed >= capacity-1) {
			this_thread::sleep_for(chrono::milliseconds(50));
			//m.lock();
			//this->events_parsed = capacity;
			//m.unlock();
		}
		else {
			for (int i = this->events_parsed; i < capacity-1; i++) {
				//парсим событие далее
				ev_info = EventInfo();
				Query q = Query();
				Event ev = this->Events[i];
				bool is_query_text = false;
				for (int j = 0; j < ev.Lines.size();j++) {
					smatch match;

					regex regex_statement1(R_DBINFO);
					if (regex_search(ev.Lines[j], match, regex_statement1)) {
						//cout << line;
						ev_info.DB_Address = string(match.prefix()) + ".FDB";
						ev_info.DB_Name = GetDBNameFromAddress(ev_info.DB_Address);
						ev_info.DB_info = match.suffix();
						continue;
					}

					regex regex_statement5(R_TR_INFO);
					if (regex_search(ev.Lines[j], match, regex_statement5)) {
						ev_info.TR_Name = match[0];
						string temp = match.suffix();
						ev_info.TR_info = temp.substr(2, temp.length()-3);
						continue;
					}

					regex regex_statement2(R_QUERY_s);
					if (regex_search(ev.Lines[j], match, regex_statement2)) {
						is_query_text = true;
						continue;
					}

					regex regex_statement3(R_QUERY_e);
					if (regex_search(ev.Lines[j], match, regex_statement3)) {
						is_query_text = false;
						continue;
					}

					regex regex_statement(R_PLAN);
					if (regex_search(ev.Lines[j], match, regex_statement)) {
						q.Plan += match.suffix();
						continue;
					}

					regex regex_statement4(R_RUNTIME);
					if (regex_search(ev.Lines[j], match, regex_statement4)) {
						string runtime = match[0];
						string subline = "";
						for (int k = 0; k < runtime.length()-3; k++) 
							subline += runtime[k];
						q.Run_Time = stoi(subline);
						is_query_text = false;		//для FAILED PREPARE_STATEMENT
						continue;
					}

					if (is_query_text) {
						q.Text += ev.Lines[j] + " ";
					}

				}
				q.SetQueryText(DeleteParamsFromQuery(q.Text));
				ev_info.query = q;
				this->Events[i].Info = ev_info;
				//this_thread::sleep_for(chrono::milliseconds(50));
				m.lock();
				cout << "Найдено: строк - " << this->cursor << " | событий - " << this->Events.size() <<" | Обработано событий - " << this->events_parsed << "\r\n";
				m.unlock();
				this->events_parsed++;
			}
		}
		if (!this->isReading && this->events_parsed >= capacity-1)
			break;
	}
}
void Parser::ReadLog(mutex &m) {
	regex regex_timestamp(R_DATE);

	vector<string> lines_of_event;
	bool read_event = false;

	Event ev;
	ifstream file(this->filename);
	m.lock();
	this->isReading = true;
	m.unlock();
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
				regex regex_statement(R_PLAN);
				if (regex_search(line, match, regex_statement)) {
					cout << line;
				}
				*/
				lines_of_event.push_back(line);
				m.lock();
				cout << "Найдено: строк - " << this->cursor << " | событий - " << this->Events.size() << " | Обработано событий - " << this->events_parsed << "\r\n";
				m.unlock();
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

//Убирает параметры из запросов, нужно для их сравнения
string Parser::DeleteParamsFromQuery(string input)
{
	if (input == "")
		return "";
	string newstring = "";
	bool writyng = true;
	for (int i = 0; i < input.length()-1; i++) {
		if (input[i] == '\"' || input[i] == '\'' || input[i] == '\\\"' || input[i] == '\\\'') {
			writyng = !writyng;
			continue;
		}
		if (isdigit(input[i]))	//убираем все цифры из запроса. нужно чтобы для агрегации одинаковых запросов с разными параметрми
			continue;
		if(writyng)
			newstring += input[i];
	}
	/*
	smatch match;
	regex regex_statement1(".*=\s*\d+.*");
	if (regex_search(newstring, match, regex_statement1)) {
		newstring = newstring;
	}
	*/
	return newstring;
}

//Вытаскивает название БД из адреса БД
string Parser::GetDBNameFromAddress(string input) {
	string temp = "";
	for (int i = input.length()-1; i > 0; --i) {
		if (input[i] != '\\')
			temp += input[i];
		else
			break;
	}
	reverse(temp.begin(), temp.end());
	return temp;	
}

Parser::~Parser()
{
}
