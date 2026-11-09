#pragma once
#include "TitleBST.h"
#include "MusicQueueNode.h"
#include <vector>
#include <string>
using namespace std;

class TitleBSTNode
{
private:
	string title;
	vector<string> artist;
	string run_time;
	vector<int> rt;
	int rtAll = 0;
	int count = 0;
	TitleBSTNode *left;
	TitleBSTNode *right;

public:
	TitleBSTNode() // constructor
	{
		this->count = 0;
		this->left = NULL;
		this->right = NULL;
		this->rtAll = 0;
		this->run_time = "0:00";
	}
	TitleBSTNode(const string &title, const string &artist, const int &run_time) // parameterized constructor
	{
		this->left = NULL;
		this->right = NULL;
		this->title = title;
		this->artist.push_back(artist);
		this->rt.push_back(run_time);
		this->rtAll += run_time;
		time_set();
		this->count++;
	}
	~TitleBSTNode() // destructor
	{
		this->left = NULL;
		this->right = NULL;
	}
	void set(const string &artist, const int &run_time, string title = "") // insert data
	{
		if (title != "")
		{
			this->title = title;
		}
		this->artist.push_back(artist);
		this->rt.push_back(run_time);
		this->rtAll += run_time;
		time_set();
		this->count++;
	}
	bool search(string artist) // Verify that the corresponding Artist exists
	{
		for (int i = 0; i < this->count; i++)
		{
			if (this->artist[i] == artist)
			{
				return true;
			}
		}
		return false;
	}
	void copy(TitleBSTNode *node) // copy data
	{
		this->title = node->Title();
		this->artist = node->Artist();
		this->run_time = node->Run_time();
		this->rt = node->Rt();
		this->count = node->Count();
		this->rtAll = 0;
		for (int value : this->rt)
			this->rtAll += value;
	}
	void deleteArtist(const string &artist) // delete artist
	{
		for (int i = 0; i < this->count; i++)
		{
			if (this->artist[i] == artist)
			{
				this->artist.erase(this->artist.begin() + i);
				this->rtAll -= this->rt[i];
				this->rt.erase(this->rt.begin() + i);
				this->count--;
				time_set(); // Setting the Time
				return;
			}
		}
	}
	void time_set() // Function that changes the entire time data when the node's time data changes
	{
		this->run_time = to_string(this->rtAll / 60) + ':'; // minutes
		if (this->rtAll % 60 < 10)								 // Add 0 for less than 10 seconds
			this->run_time += '0';
		this->run_time += to_string(this->rtAll % 60); // second
	}
	// The functions below return private member variables or link node to pointers.
	const string &Title()
	{
		return this->title;
	}
	const vector<string> &Artist()
	{
		return this->artist;
	}
	const string &Run_time()
	{
		return this->run_time;
	}
	const vector<int> &Rt()
	{
		return this->rt;
	}
	const int &Count()
	{
		return this->count;
	}
	TitleBSTNode *Left()
	{
		return this->left;
	}
	TitleBSTNode *Right()
	{
		return this->right;
	}
	void LinkLeft(TitleBSTNode *node)
	{
		this->left = node;
	}

	void LinkRight(TitleBSTNode *node)
	{
		this->right = node;
	}
	TitleBSTNode *Compare(const string &title) // Return by comparing priorities of title
	{
		if (this->title > title)
		{
			return Left();
		}
		else
		{
			return Right();
		}
	}
};
