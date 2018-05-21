#pragma once
#include <string>
#include <list>
#include <vector> 
#include "Parser.h"

using namespace std;

class Analysis
{
public:
	string file;
	void AggregateQueryStatistics(vector<Event>& input);
	vector<Event>& SortEventsDesc(vector<Event>& input);

	Analysis();
	Analysis(string filename);
	~Analysis();
};

struct AggregateQuery 
{
	unsigned int hash;
	string text;
	int equals;
	float avg_time;
	short int sum_time;

	AggregateQuery() {
		this->sum_time = 0;
	}

	AggregateQuery(unsigned int _hash, string _text, int _equals, float _avg_time, float _sum_time) {
		hash = _hash;
		text = _text;
		equals = _equals;
		avg_time = _avg_time;
		sum_time = _sum_time;
	}
};
