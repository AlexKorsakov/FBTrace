// Main.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "string.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include "Parser.h"
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

void ParceEvents(Parser& input, mutex &m) {
	if (input.filename != "")
		input.ParseEvent(m);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	string input_filename = "c:\\Program Files (x86)\\Firebird\\Firebird_3_0\\default_trace.log";
	string output_filename = "e:\\Temp\\stats.log";
	bool start_processing = true;

	cout << "-------------------------------------------------------------------------------\r\n";
	cout << "Аргументы командной строки:";
	cout << "\r\n -i - Путь к файлу лога, который будет обрабатываться, путь по умолчанию: " << input_filename;
	cout << "\r\n -o - Путь к файлу статистики, в который будет записываться результат обработки, по умолчанию: " << output_filename;
	cout << "\r\n -h - Справка";
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
		if ((!strcmp(argv[i], "-sp")) && !strcmp(argv[i+1], "false")) {
			start_processing = false;
		}
		if ((!strcmp(argv[i], "-q"))) {
			goto quit;
		}
	}
	if (start_processing) {
		Parser item = Parser(input_filename);

		thread thread_1(ReadLog, ref(item), ref(m));
		thread thread_2(ParceEvents, ref(item), ref(m));

		//ParceEvents(ref(item), ref(m));	//дебаг
		thread_1.join();
		thread_2.join();
	}

	//filename = argv[1];
	//while (true) 
	{

	}

quit:
	cout << "Выполнение программы завершено.";
	system("pause");
	return 0;
}
