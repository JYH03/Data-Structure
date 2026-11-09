#pragma once
#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"

class BpTreeDataNode : public BpTreeNode
{
private:
	map<string, EmployeeData *> mapData; // Data node stores employee information mapped to names as keys
	BpTreeNode *pNext;					 // Pointer to connect to the next data node
	BpTreeNode *pPrev;					 // Pointer to connect to the previous data node

public:
	BpTreeDataNode() // Constructor
	{
		// Initialize pointers
		pNext = NULL;
		pPrev = NULL;
	}
	~BpTreeDataNode() // Destructor
	{
		for (auto &pair : mapData) // In the map data structure
		{
			delete pair.second; // Delete employee information
		}
		mapData.clear(); // Clear the map data structure
	}

	void setNext(BpTreeNode *pN) { pNext = pN; } // Connect to the next data node
	void setPrev(BpTreeNode *pN) { pPrev = pN; } // Connect to the previous data node
	BpTreeNode *getNext() { return pNext; }		 // Return the next data node
	BpTreeNode *getPrev() { return pPrev; }		 // Return the previous data node

	void insertDataMap(string name, EmployeeData *pN)
	{
		mapData.insert(map<string, EmployeeData *>::value_type(name, pN)); // Insert key and data into the node
	}

	void deleteMap(string name)
	{
		mapData.erase(name); // Delete the data corresponding to the given name (key)
	}
	map<string, EmployeeData *> *getDataMap() { return &mapData; } // Return the map
	bool IsLeaf() { return true; }								   // Function to check whether it's a data node or index node
};

#endif