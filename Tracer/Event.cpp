#include "stdafx.h"
#include "Event.h"
#include <iostream>

Event::Event() {

}

Event::Event(vector<string> &event_string)
{
	int event_string_size = event_string.size();
	if(event_string_size == 3)
		this->Type = event_string[2];
	else if(event_string_size == 4)	//FAILED PREPARE_STATEMENT
		this->Type = event_string[2] + "_" + event_string[3];
	else if(event_string_size == 5)	//ERROR AT JStatement::prepare
		this->Type = event_string[2] + "_" + event_string[3] + "_" + event_string[4];
	else {
		cout << "ex";
	}

	string temp = event_string[1];
	temp.erase(temp.begin());
	temp.erase(temp.end() - 1);
	for (int i = 0; i < temp.length(); i++) {
		if (temp[i] == ':') {
			this->PID = temp.substr(0, i);
			this->Trace_number = temp.substr(i+1, temp.length()-1);
			break;
		}		
	}
	this->Timestamp = event_string[0];
}

bool Event::IsEmpty()
{
	return (this->Type == "" && this->PID == "" && this->Trace_number == ""
		&& this->Timestamp == "") ? true : false;
}


Event::~Event()
{
	//delete this;
}
