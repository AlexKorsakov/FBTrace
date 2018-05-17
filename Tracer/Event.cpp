#include "stdafx.h"
#include "Event.h"

Event::Event() {

}

Event::Event(vector<string> &event_string)
{
	this->Type = event_string[2];
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
