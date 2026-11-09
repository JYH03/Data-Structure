#pragma once
#include "TitleBSTNode.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class TitleBST
{
private:
	TitleBSTNode *root;
	TitleBSTNode *parent;
	vector<string> data;
	TitleBSTNode *target;

public:
	TitleBST();
	~TitleBST();

	void insert(const string &title, const string &artist, const int run_time);
	void search(const string &title);
	bool empty();
	const vector<string> &print(string title = string(""), string artist = string(""));
	string time_string(int time);
	void AllData(TitleBSTNode *node);
	void Delete(string title = string(""), string artist = string(""));
	void delete_node(TitleBSTNode *node);
	TitleBSTNode *findMin(TitleBSTNode *node);
	void preTravel(TitleBSTNode *node, const string &artist);
	void postTravel(TitleBSTNode *node);
	TitleBSTNode *Target();
	TitleBSTNode *Root();
};