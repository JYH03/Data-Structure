#pragma once
#include "ArtistBSTNode.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class ArtistBST
{
private:
	ArtistBSTNode *root;
	ArtistBSTNode *parent;
	vector<string> data;
	ArtistBSTNode *target;

public:
	ArtistBST();
	~ArtistBST();

	void insert(const string &artist, const string &title, const int run_time);
	void search(const string &artist);
	bool empty();
	const vector<string> &print(string artist = string(""), string title = string(""));
	void AllData(ArtistBSTNode *node);
	void Delete(string artist = string(""), string title = string(""));
	void delete_node(ArtistBSTNode *node);
	ArtistBSTNode *findMin(ArtistBSTNode *node);
	void preTravel(ArtistBSTNode *node, const string &title);
	void postTravel(ArtistBSTNode *node);
	string time_string(int time);
	ArtistBSTNode *Target();
	ArtistBSTNode *Root();
};