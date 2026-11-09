#pragma once
#include "SelectionTreeNode.h"

class SelectionTree
{
private:
    SelectionTreeNode *root;
    SelectionTreeNode *run[9]; // 1~8, not use 0
    vector<string> printer;
    SelectionTreeNode *buildTree(int depth, int &runIndex);
    void destroyTree(SelectionTreeNode *node);

public:
    SelectionTree()
    {
        setTree();
    }

    ~SelectionTree()
    {
        destroyTree(this->root);
    }

    void setRoot(SelectionTreeNode *pN) { this->root = pN; }
    SelectionTreeNode *getRoot() { return root; }

    void setTree();
    bool Insert(EmployeeData *newData);
    bool Delete();
    vector<string> &printEmployeeData(int dept_no);
    void Update(int runIndex);
    void Refresh();
};
