#pragma once
#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"
#include "SelectionTree.h"

class BpTree
{
private:
	BpTreeNode *root;
	BpTreeNode *head;
	int order; // m children
	vector<string> printer;

public:
	BpTree(int order = 3) // Constructor
	{
		this->order = order; // Store the given order
		// Initialize pointers
		this->root = NULL;
		this->head = NULL;
	}

	~BpTree() // Destructor
	{
		// delete index node
		delete this->root;
	}

	/* essential */
	bool Insert(EmployeeData *newData);
	bool excessDataNode(BpTreeNode *pDataNode);
	bool excessIndexNode(BpTreeNode *pIndexNode);
	void splitDataNode(BpTreeNode *pDataNode);
	void splitIndexNode(BpTreeNode *pIndexNode);
	BpTreeNode *getRoot() { return root; }
	BpTreeNode *searchDataNode(string name);
	void searchRange(string start, string end);

	void searchAll();
	const vector<string> &printData(string start = "", string end = "");
	vector<EmployeeData *> searchDeptNo(int dept_no);
};

#endif
