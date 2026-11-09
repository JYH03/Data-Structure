#include "SelectionTree.h"

void SelectionTree::setTree() // Set up the selection tree and allocate nodes
{
    int runIndex = 0;
    this->root = buildTree(0, runIndex);
}

SelectionTreeNode *SelectionTree::buildTree(int depth, int &runIndex)
{
    SelectionTreeNode *node = new SelectionTreeNode;
    if (depth == 3)
    {
        node->HeapInit();
        this->run[++runIndex] = node;
        return node;
    }

    SelectionTreeNode *left = buildTree(depth + 1, runIndex);
    SelectionTreeNode *right = buildTree(depth + 1, runIndex);
    node->setLeftChild(left);
    node->setRightChild(right);
    left->setParent(node);
    right->setParent(node);
    return node;
}

void SelectionTree::destroyTree(SelectionTreeNode *node)
{
    if (!node)
        return;
    destroyTree(node->getLeftChild());
    destroyTree(node->getRightChild());
    delete node;
}

bool SelectionTree::Insert(EmployeeData *newData) // Function to insert data
{
    int dept = newData->getDeptNo();                   // Store department code to check
    if (dept % 100 != 0 || (dept < 100 || dept > 800)) // If department code is invalid
        return false;                                  // Insertion faile
    dept /= 100;
    this->run[dept]->getHeap()->Insert(newData);                         // Insert data into heap of corresponding run
    this->run[dept]->setEmployeeData(this->run[dept]->getHeap()->Top()); // Store top of sorted heap into run
    Update(dept);                                                        // Tournament update due to data change
    return true;                                                         // Insertion successful
}

bool SelectionTree::Delete() // Function to delete data
{
    if (this->root->IsEmpty()) // If root node is empty
        return false;            // Deletion failed
    for (int i = 1; i <= 8; i++) // For each run
    {
        if (this->run[i]->getHeap()->Top() == this->root->getEmployeeData()) // Find run with data matching root node
        {
            this->run[i]->getHeap()->Delete();                             // Delete top data from heap
            this->run[i]->setEmployeeData(this->run[i]->getHeap()->Top()); // Store new top of heap into run
            Update(i);                                                     // Tournament update due to data change
            return true;
        }
    }
    return false;
}

vector<string> &SelectionTree::printEmployeeData(int dept_no) // Print all data (heap) for given department code
{
    this->printer.clear();        // Clear vector to return
    if (!this->root->IsEmpty()) // If root node is not empty
    {
        if (dept_no % 100 == 0 && (dept_no / 100 >= 1 && dept_no / 100 <= 8)) // If department code is valid
        {
            EmployeeHeap *heap = this->run[dept_no / 100]->getHeap();
            vector<EmployeeData *> data;
            for (int i = 1; i <= heap->getSize(); ++i)
            {
                data.push_back(heap->getData(i));
            }

            sort(data.begin(), data.end(), [](EmployeeData *left, EmployeeData *right)
                 { return left->getName() < right->getName(); });

            for (EmployeeData *employee : data)
            {
                string tmp = employee->getName() + '/' +
                             to_string(employee->getDeptNo()) + '/' +
                             to_string(employee->getID()) + '/' +
                             to_string(employee->getIncome());
                this->printer.push_back(tmp);
            }
        }
    }
    return printer; // Return vector
}

void SelectionTree::Update(int runIndex) // Function to run selection tree tournament
{
    SelectionTreeNode *parent = this->run[runIndex]->getParent();
    while (parent)
    {
        SelectionTreeNode *leftChild = parent->getLeftChild();
        SelectionTreeNode *rightChild = parent->getRightChild();
        EmployeeData *leftData = leftChild->getEmployeeData();
        EmployeeData *rightData = rightChild->getEmployeeData();

        if (leftChild->IsEmpty())
        {
            parent->setEmployeeData(rightData);
        }
        else if (rightChild->IsEmpty())
        {
            parent->setEmployeeData(leftData);
        }
        else if (leftData->getIncome() >= rightData->getIncome())
        {
            parent->setEmployeeData(leftData);
        }
        else
        {
            parent->setEmployeeData(rightData);
        }
        parent = parent->getParent();
    }
}

void SelectionTree::Refresh()
{
    for (int i = 1; i <= 8; ++i)
    {
        this->run[i]->getHeap()->Rebuild();
        this->run[i]->setEmployeeData(this->run[i]->getHeap()->Top());
    }
    for (int i = 1; i <= 8; ++i)
        Update(i);
}
