#pragma once
// #include "MusicQueue.h"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

class MusicQueueNode
{
private:
	string artist;
	string title;
	string run_time;

	MusicQueueNode *next; // Link toward the rear
	MusicQueueNode *prev; // Link toward the head

public:
	MusicQueueNode() // constructor
	{
		this->next = NULL;
		this->prev = NULL;
	}
	~MusicQueueNode() // destructor
	{
		this->next = NULL;
		this->prev = NULL;
	}

	void insert(string artist, string title, string run_time) // insert data
	{
		this->artist = artist;
		this->title = title;
		this->run_time = run_time;
	}

	bool exist(string artist, string title) // Verify that the song on the node is the same as the song entered
	{
		if (artist == this->artist && title == this->title)
			return true;
		else
			return false;
	}
	// The functions below return private member variables or link node to pointers.
	MusicQueueNode *Next()
	{
		return this->next;
	}
	MusicQueueNode *Prev()
	{
		return this->prev;
	}
	void LinkNext(MusicQueueNode *node)
	{
		this->next = node;
	}
	void LinkPrev(MusicQueueNode *node)
	{
		this->prev = node;
	}
	const string &Artist()
	{
		return this->artist;
	}
	const string &Title()
	{
		return this->title;
	}
	const string &Run_time()
	{
		return this->run_time;
	}
};
