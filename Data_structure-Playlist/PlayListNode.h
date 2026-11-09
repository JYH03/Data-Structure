#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "PlayList.h"
using namespace std;

class PlayListNode
{
private:
	string artist;
	string title;
	int runtime_sec;

	PlayListNode *prev;
	PlayListNode *next;

public:
	PlayListNode() // constructor
	{
		this->runtime_sec = 0;
		this->prev = NULL;
		this->next = NULL;
	}
	PlayListNode(const string &artist, const string &title, const int &run_time) // parameterized constructor
	{
		this->prev = NULL;
		this->next = NULL;
		this->artist = artist;
		this->title = title;
		this->runtime_sec = run_time;
	}
	~PlayListNode() // destructor
	{
		this->prev = NULL;
		this->next = NULL;
	}

	void set(string &artist, string &title, int &run_time) // insert data
	{
		this->artist = artist;
		this->title = title;
		this->runtime_sec = run_time;
	}
	// The functions below return private member variables or link node to pointers.
	const string &Artist()
	{
		return this->artist;
	}
	const string &Title()
	{
		return this->title;
	}
	const int &Runtime_sec()
	{
		return this->runtime_sec;
	}
	PlayListNode *Next()
	{
		return this->next;
	}
	PlayListNode *Prev()
	{
		return this->prev;
	}
	void LinkNext(PlayListNode *node)
	{
		this->next = node;
	}
	void LinkPrev(PlayListNode *node)
	{
		this->prev = node;
	}
};