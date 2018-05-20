// Tracer.cpp: определяет точку входа для консольного приложения.
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
	string filename = "";
	//filename = argv[1];
	filename = "c:\\Program Files (x86)\\Firebird\\Firebird_3_0\\default_trace.log";

	Parser item = Parser(filename);
	//ReadLog(ref(item));

	thread thread_1(ReadLog, ref(item), ref(m));
	thread thread_2(ParceEvents, ref(item), ref(m));
	
	thread_1.join();
	thread_2.join();
	


	/*
	if (argc > 1)
	{
		cout << argv[1] << endl;
	}
	else
	{
		cout << "Not arguments" << endl;
	}
	*/
	system("pause");
	return 0;
}
