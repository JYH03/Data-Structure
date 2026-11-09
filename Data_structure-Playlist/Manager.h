#pragma once
#include "MusicQueue.h"
#include "TitleBST.h"
#include "ArtistBST.h"
#include "PlayList.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Manager
{
private:
	MusicQueue q;
	ArtistBST ab;
	TitleBST tb;
	PlayList pl;
	ifstream fcmd;
	ifstream fmusic;
	ofstream flog;
	istringstream ss;
	string Music_List;
	string Log;

public:
	Manager();
	~Manager();

	void run(const char *command);

	void LOAD();
	void ADD(string data);
	void QPOP();
	void SEARCH(string Order, string Data);
	void MAKEPL(string Order, string Data);
	void PRINT(string Order);
	void DELETE(string &Order, string &Data);
	int time_int(const string &time);
	void SuccessPrint(string Order);
	void ErrorPrint(int ErrorValue);
	void EXIT();
};
