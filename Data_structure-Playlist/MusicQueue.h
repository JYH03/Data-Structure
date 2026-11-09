#pragma once
#include "MusicQueueNode.h"
#include <string>
using namespace std;

class MusicQueue
{
private:
	MusicQueueNode *head;
	MusicQueueNode *rear;
	int size = 0;
	int capacity = 0;

public:
	MusicQueue();
	~MusicQueue();

	bool empty();
	bool exist(string &artist, string &title);
	bool push(string &artist, string &title, string &run_time);
	void pop();
	MusicQueueNode *front();
};