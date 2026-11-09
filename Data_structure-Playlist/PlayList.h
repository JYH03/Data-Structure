#pragma once
#include "PlayListNode.h"
#include <vector>
#include <string>
using namespace std;

class PlayList
{
private:
	PlayListNode *head;
	PlayListNode *cursor;
	int count;
	int time;
	vector<string> data;

public:
	PlayList();
	~PlayList();
	void insert_node(const string &artist, const string &title, const int &run_time);
	void delete_node(string artist = string(""), string = string(""));
	bool empty();
	bool full();
	bool exist(const string &artist, const string &title);
	const vector<string> &print();
	int run_time();
	int Count();
	string time_string(int time,bool type);
};