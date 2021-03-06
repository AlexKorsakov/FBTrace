// Main.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "string.h"
#include "Parser.h"
#include "Analysis.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <thread>
#include <mutex>

#define NON_COPYABLE_NOR_MOVABLE(T) \
      T(T const &) = delete; \
      void operator=(T const &t) = delete; \
      T(T &&) = delete;


using namespace std;

mutex m;

void ReadLog(Parser& input, mutex &m) {
	if (input.filename != "")
		input.ReadLog(m);
}

void ParseEvents(Parser& input, mutex &m) {
	if (input.filename != "")
		input.ParseEvent(m);
}

void ReportDone(string input) {
	cout << "Готово! Отчет тут: " << input << "\r\n";
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	string input_filename = "c:\\Program Files (x86)\\Firebird\\Firebird_3_0\\default_trace1.log";
	string output_filename = "e:\\Temp\\stats.csv";
	bool start_processing = false;
	Parser item = Parser(input_filename);

	cout << "-------------------------------------------------------------------------------\r\n";
	cout << "Аргументы командной строки:";
	cout << "\r\n -i [filename] \t- Путь к файлу лога, который будет обрабатываться, путь по умолчанию: \r\n\t\t" << input_filename;
	cout << "\r\n -o [filename] \t- Путь к файлу статистики, в который будет записываться результат обработки, по умолчанию: \r\n\t\t" << output_filename;
	cout << "\r\n -h \t\t- Справка";
	cout << "\r\n -sp \t\t- Начать обработку лога";
	cout << "\r\n";
	cout << "-------------------------------------------------------------------------------\r\n";

	for (int i = 1; i < argc; ++i) {
		if ((!strcmp(argv[i], "-i")) && (i + 1 < argc)) {
			input_filename = argv[i + 1];
		}
		if ((!strcmp(argv[i], "-o")) && (i + 1 < argc)) {
			output_filename = argv[i + 1];
		}
		/*
		if ((!strcmp(argv[i], "-h"))) {
		}
		*/
		if ((!strcmp(argv[i], "-sp"))) {
			start_processing = true;
		}
	}
	if (start_processing) {

		thread thread_1(ReadLog, ref(item), ref(m));
		thread thread_2(ParseEvents, ref(item), ref(m));
		//thread thread_3(StopParsing);

		thread_1.join();
		thread_2.join();
		
		
		//ParseEvents(ref(item), ref(m));	//дебаг
		//ReadLog(ref(item), ref(m));	//дебаг
	}
	else {
		goto quit;
	}

	char in;
	while (true) 
	{
		cout << "-------------------------------------------------------------------------------\r\n";
		cout << "Что делаем дальше? Введите число для дальнейших действий: \r\n";
		cout << "1 - Отчет по похожим запросам\r\n";
		cout << "2 - Отчет по суммарно долгим запросам\r\n";
		cout << "9 - Выход\r\n";
		cout << "->";
		cin >> in;

		if (in == '9') {
			goto quit;
		}
		Analysis a_item = Analysis(output_filename);

		if (in == '1') {
			a_item.AggregateQueryStatistics(item.Events);
			ReportDone(output_filename);
		}
		if (in == '2') {

			//ReportDone(output_filename);
		}
	}

quit:
	cout << "\r\nВыполнение программы завершено...\r\n";
	delete &item;
	system("pause");
	return 0;
}
