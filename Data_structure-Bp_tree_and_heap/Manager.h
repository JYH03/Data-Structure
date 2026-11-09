#pragma once
#include "SelectionTree.h"
#include "BpTree.h"
#include <fstream>
#include <sstream>

using namespace std;

class Manager
{
private:
	string cmd;
	BpTree *bptree;
	SelectionTree *stree;
	vector<string> printer;

public:
	Manager(int bpOrder) // constructor
	{
		this->bptree = new BpTree(bpOrder); // Create B+ tree
		this->stree = new SelectionTree;	// Create selection tree
	}

	~Manager() // destructor
	{
		delete this->stree;	 // Delete selection tree
		delete this->bptree; // Delete B+ tree
	}

	ifstream fin;
	ifstream f_employee;
	ofstream flog;
	istringstream ss;

	void run(const char *command);
	void LOAD();
	void ADD_BP(string name, string dept_no, string id, string income);
	void SEARCH_BP_NAME(string name);
	void SEARCH_BP_RANGE(string start, string end);
	void PRINT_BP();
	void ADD_ST_DEPTNO(int dept_no);
	void ADD_ST_NAME(string name);
	void PRINT_ST(int dept_no);
	void DELETE();

	void printErrorCode(int n);
	void printSuccessCode(string success);
};
