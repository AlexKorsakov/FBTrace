// Tracer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "string.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[]) 
{
	setlocale(LC_ALL, "rus");
	string filename = "";
	//filename = argv[1];
	filename = "c:\\Program Files (x86)\\Firebird\\Firebird_3_0\\default_trace.log";


	Parser item = Parser(filename);
	item.Read();
	


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

