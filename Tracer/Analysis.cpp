#include "stdafx.h"
#include "Analysis.h"
#include "csvfile.h"
#include <string>
#include <map>


void Analysis::AggregateQueryStatistics(vector<Event>& input)
{
	vector <Event> AllEvents = input;
	vector <Event> SelectedEvents;
	vector <AggregateQuery> Result;

	//отбор
	for (int i = 0; i < AllEvents.size(); i++) {
		if (AllEvents[i].Info.query.TextHash != 0 && AllEvents[i].Info.query.Run_Time >= 0 && AllEvents[i].Type == "PREPARE_STATEMENT")
			SelectedEvents.push_back(AllEvents[i]);
	}

	//сортировка
	SelectedEvents = SortEventsDesc(SelectedEvents);
	/*
	cout << "-------------\r\n";
	for (int i = 0; i < SelectedEvents.size(); i++) {
		cout << i << " | " << SelectedEvents[i].Info.query.TextHash << "\r\n";
	}
	cout << "-------------\r\n";
	*/
	

	//совпадения
	std::map<int, int> equals_map;
	for (int i = 0; i < SelectedEvents.size(); i++) {
		int equals = 0;
		for (int j = 0; j < SelectedEvents.size(); j++) {
			if (SelectedEvents[i].Info.query.TextHash == SelectedEvents[j].Info.query.TextHash)
				equals++;
		}
		equals_map.insert(pair<int, int>(SelectedEvents[i].Info.query.TextHash, equals));
	}

	//сортировка по совпадениям
	/*
	cout << "-------------\r\n";
	for (auto it = equals_map.begin(); it != equals_map.end(); ++it)
	{
		cout << (*it).first << " : " << (*it).second << "\r\n";
	}
	cout << "-------------\r\n";
	*/

	for (auto it = equals_map.begin(); it != equals_map.end(); ++it)
	{
		AggregateQuery item;
		for (int i=0; i < SelectedEvents.size(); i++) {
			if (SelectedEvents[i].Info.query.TextHash == (*it).first) {
				item.hash = SelectedEvents[i].Info.query.TextHash;
				item.text = SelectedEvents[i].Info.query.Text;
				item.equals = (*it).second;
				item.sum_time += SelectedEvents[i].Info.query.Run_Time;
			}
		}
		item.avg_time = item.sum_time / item.equals;
		Result.push_back(item);
		cout << (*it).first << " : " << (*it).second << "\r\n";
	}


	//сортировка по совпадениям
	for (int i = 0; i < Result.size(); i++) {
		for (int j = 0; j < Result.size(); j++) {
			if (Result[i].equals > Result[j].equals)
			{
				AggregateQuery temp = Result[i];
				Result[i] = Result[j];
				Result[j] = temp;
			}
		}
	}

	try
	{
		csvfile csv(this->file);
		csv << "Текст запроса" << "Совпадений" << "Среднее время, мс" << "Суммарное время, мс" << endrow;
		for (int i = 0; i < Result.size(); i++) {
			csv << Result[i].text << to_string(Result[i].equals) << to_string(Result[i].avg_time) << to_string(Result[i].sum_time) << endrow;
		}
	}
	catch (const exception &ex)
	{
		cout << "\r\nОшибка при записи отчета: " << ex.what() << endl;
	}
}

vector<Event>& Analysis::SortEventsDesc(vector<Event>& input)
{
	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < input.size(); j++) {
			if (input[i].Info.query.TextHash > input[j].Info.query.TextHash)
			{
				Event temp = input[i];
				input[i] = input[j];
				input[j] = temp;
			}
		}
	}
	return input;
}



Analysis::Analysis(string filename)
{
	this->file = filename;
}

Analysis::Analysis()
{
}

Analysis::~Analysis()
{
}
