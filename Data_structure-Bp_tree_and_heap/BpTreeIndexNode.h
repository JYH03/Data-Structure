#pragma once
#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"
#include <map>

class BpTreeIndexNode : public BpTreeNode
{
private:
	map<string, BpTreeNode *> mapIndex;

public:
	BpTreeIndexNode() {}
	~BpTreeIndexNode()
	{
		delete pMostLeftChild;
		for (auto &pair : mapIndex) // In the map data structure
		{
			delete pair.second; // Delete node pointer
		}
		mapIndex.clear(); // Clear the map data structure
	}

	void insertIndexMap(string name, BpTreeNode *pN)
	{
		mapIndex.insert(map<string, BpTreeNode *>::value_type(name, pN)); // Insert key and node pointer into the node
	}

	void deleteMap(string name)
	{
		mapIndex.erase(name); // Delete the data corresponding to the given name (key)
	}

	map<string, BpTreeNode *> *getIndexMap() { return &mapIndex; } // Return the map
	bool IsLeaf() { return false; }								   // Function to check whether it's a data node or index node
};

#endif