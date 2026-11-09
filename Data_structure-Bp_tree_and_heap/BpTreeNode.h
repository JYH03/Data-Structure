#pragma once
#include "EmployeeData.h"
#include <vector>

class BpTreeNode
{
private:
	BpTreeNode *pParent;

protected:
	BpTreeNode *pMostLeftChild;

public:
	BpTreeNode()
	{
		pParent = NULL;
		pMostLeftChild = NULL;
	}
	virtual ~BpTreeNode()
	{
		pParent = NULL;
		pMostLeftChild = NULL;
	}

	void setMostLeftChild(BpTreeNode *pN) { pMostLeftChild = pN; }
	void setParent(BpTreeNode *pN) { pParent = pN; }

	BpTreeNode *getParent() { return pParent; }
	BpTreeNode *getMostLeftChild() { return pMostLeftChild; }

	virtual void setNext(BpTreeNode *) {}
	virtual void setPrev(BpTreeNode *) {}
	virtual BpTreeNode *getNext() { return NULL; }
	virtual BpTreeNode *getPrev() { return NULL; }

	virtual void insertDataMap(string, EmployeeData *) {}
	virtual void insertIndexMap(string, BpTreeNode *) {}
	virtual void deleteMap(string) {}

	virtual map<string, BpTreeNode *> *getIndexMap() { return {}; }
	virtual map<string, EmployeeData *> *getDataMap() { return {}; }
	virtual bool IsLeaf()
	{
		return false;
	}
};
