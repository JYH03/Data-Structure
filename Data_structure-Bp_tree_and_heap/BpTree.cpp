#include "BpTree.h"

bool BpTree::Insert(EmployeeData *newData) // B+ Tree insertion function
{
    BpTreeNode *ptr;
    string tmp_name = newData->getName(); // Store the name of the data to be inserted as the key
    if (this->root)                       // When there is at least one data in the B+ Tree
    {
        ptr = searchDataNode(tmp_name);         // Search for the appropriate data node based on key comparison
        if (ptr->getDataMap()->count(tmp_name)) // If the data already exists
        {
            // ADD_BP updates only the income when the employee name already exists.
            ptr->getDataMap()->at(tmp_name)->setIncome(newData->getIncome());
            delete newData;
            return true;
        }
        ptr->insertDataMap(tmp_name, newData); // Insert data
        if (excessDataNode(ptr))               // If the data exceeds the limit after insertion
        {
            splitDataNode(ptr); // Split
        }
    }
    else // When inserting the first data into the B+ Tree
    {
        this->root = new BpTreeDataNode;              // Create a new data node and set it as root
        this->head = this->root;                      // Set it as the head of the data node
        this->root->insertDataMap(tmp_name, newData); // Insert data into the node
    }
    return true; // Return true as insertion succeeded
}

bool BpTree::excessDataNode(BpTreeNode *pDataNode) // Function to check if data node exceeds the limit
{
    if (pDataNode->getDataMap()->size() == static_cast<size_t>(this->order)) // If the number of data in the node equals the order
    {
        return true; // Exceeds the limit
    }
    return false; // Does not exceed the limit
}

bool BpTree::excessIndexNode(BpTreeNode *pIndexNode) // Function to check if index node exceeds the limit
{
    if (pIndexNode->getIndexMap()->size() == static_cast<size_t>(this->order)) // If the number of data in the node equals the order
    {
        return true; // Exceeds the limit
    }
    return false; // Does not exceed the limit
}

void BpTree::splitDataNode(BpTreeNode *pDataNode) // Data node split function
{
    // step 1 create new node
    string key;                                   // Temporary variable to store the key to be promoted
    BpTreeNode *currentnode = new BpTreeDataNode; // Create a new node to be split
    // step 2 transfer key and Data
    auto it = pDataNode->getDataMap()->begin();  // Iterator pointing to the first data in the map
    advance(it, (this->order) / 2);              // Move iterator to the middle data
    key = it->first;                             // Store the key to be promoted
    while (it != pDataNode->getDataMap()->end()) // Traverse from the middle data to the end
    {
        currentnode->insertDataMap(it->first, it->second); // Copy data to the new node
        pDataNode->deleteMap(it->first);                   // Delete data from the original node
        it = pDataNode->getDataMap()->begin();             // Reset iterator to the beginning
        advance(it, this->order / 2);                      // Move iterator to the middle again (due to map reordering after deletion)
    }
    // step 3 link each node
    if (pDataNode->getNext()) // If the original node has a next node
    {
        currentnode->setNext(pDataNode->getNext());   // Link the new node between the original and next node
        currentnode->getNext()->setPrev(currentnode); // Update previous link of the next node
    }
    pDataNode->setNext(currentnode); // Set next of original node to new node
    currentnode->setPrev(pDataNode); // Set previous of new node to original node
    // step 4 set the parent node;
    BpTreeNode *p = pDataNode->getParent(); // Store the parent of the original node
    if (p)                                  // If parent exists
    {
        p->insertIndexMap(key, currentnode); // Insert key and new node into parent index node
        currentnode->setParent(p);           // Set parent of new node
        if (excessIndexNode(p))              // Check if parent exceeds limit after insertion
        {
            splitIndexNode(p); // Split if exceeded
        }
    }
    else // If parent does not exist
    {
        p = new BpTreeIndexNode;             // Create new index node as parent
        p->setMostLeftChild(pDataNode);      // Set original node as MostLeftChild
        pDataNode->setParent(p);             // Set parent of original node
        p->insertIndexMap(key, currentnode); // Insert key and new node into new index node
        currentnode->setParent(p);           // Set parent of new node
        if (this->root == pDataNode)         // If original node was root, update root to new index node
            this->root = p;
    }
}

void BpTree::splitIndexNode(BpTreeNode *pIndexNode) // Index node split function
{
    // step 1 create new node
    string key;                                    // Temporary variable to store the key to be promoted
    BpTreeNode *currentnode = new BpTreeIndexNode; // Create a new node to be split
    // step 2 transfer key and Data
    auto it = pIndexNode->getIndexMap()->begin(); // Iterator pointing to the first data in the map
    advance(it, this->order / 2);                 // Move iterator to the middle data

    key = it->first;                           // Move the key to be promoted
    currentnode->setMostLeftChild(it->second); // Set MostLeftChild of new node
    it->second->setParent(currentnode);
    pIndexNode->deleteMap(key);
    it = pIndexNode->getIndexMap()->begin();
    advance(it, this->order / 2);

    while (it != pIndexNode->getIndexMap()->end())
    {
        currentnode->insertIndexMap(it->first, it->second); // Copy data to the new node
        it->second->setParent(currentnode);                 // Update parent of the node pointed by the data
        pIndexNode->deleteMap(it->first);                   // Delete data from the original node
        it = pIndexNode->getIndexMap()->begin();            // Reset iterator to the beginning
        advance(it, this->order / 2);                       // Move iterator to the middle again
    }
    // step 3 set the parent node;
    BpTreeNode *p = pIndexNode->getParent(); // Store the parent of the original node
    if (p)                                   // If parent exists
    {
        p->insertIndexMap(key, currentnode); // Insert key and new node into parent index node
        currentnode->setParent(p);           // Set parent of new node
        if (excessIndexNode(p))              // Check if parent exceeds limit after insertion
        {
            splitIndexNode(p); // Split if exceeded
        }
    }
    else // If parent does not exist
    {
        p = new BpTreeIndexNode;             // Create new index node as parent
        p->setMostLeftChild(pIndexNode);     // Set original node as MostLeftChild
        pIndexNode->setParent(p);            // Set parent of original node
        p->insertIndexMap(key, currentnode); // Insert key and new node into new index node
        currentnode->setParent(p);           // Set parent of new node
        if (pIndexNode == this->root)        // If original node was root, update root to new index node
        {
            this->root = p;
        }
    }
}

BpTreeNode *BpTree::searchDataNode(string name) // Data search function
{
    if (!this->root) // If the B+ tree has no data, return NULL
        return NULL;
    BpTreeNode *currentnode = this->root; // Start searching from the root node
    while (!currentnode->IsLeaf())        // Repeat until reaching a leaf node (data node)
    {
        if (name < currentnode->getIndexMap()->begin()->first) // If the key is smaller than the smallest key in the node
        {
            currentnode = currentnode->getMostLeftChild(); // Move to the node pointed by MostLeftChild
            continue;
        }
        // Compare from the largest key to the smallest key in the node
        for (auto it = currentnode->getIndexMap()->rbegin(); it != currentnode->getIndexMap()->rend(); ++it)
        {
            if (name >= it->first) // If the key is greater than or equal to the current key
            {
                currentnode = it->second; // Move to the node pointed by the key
                break;
            }
        }
    }
    return currentnode; // Return the data node corresponding to the given key
}

void BpTree::searchRange(string start, string end) // Function to find (for output) all data between two given keys
{
    if (!this->root || start > end)
        return;

    string arr;
    BpTreeNode *node = searchDataNode(start);
    bool firstNode = true;
    while (node)
    {
        auto *dataMap = node->getDataMap();
        auto it = firstNode ? dataMap->lower_bound(start) : dataMap->begin();
        while (it != dataMap->end())
        {
            if (it->first > end)
                return;

            EmployeeData *data = it->second;
            arr = data->getName();
            arr += '/' + to_string(data->getDeptNo());
            arr += '/' + to_string(data->getID());
            arr += '/' + to_string(data->getIncome());
            this->printer.push_back(arr);
            ++it;
        }
        firstNode = false;
        node = node->getNext();
    }
}

void BpTree::searchAll() // Search all data (linear search)
{
    string arr;
    BpTreeNode *st = this->head;
    if (st) // If head exists = B+ tree has data
    {
        while (st) // Repeat until NULL
        {
            auto it = st->getDataMap()->begin();  // From the first data in the map
            while (it != st->getDataMap()->end()) // Repeat until the end of the map
            {
                // Store data in the vector
                arr.clear();
                arr = it->second->getName();
                arr += '/' + to_string(it->second->getDeptNo());
                arr += '/' + to_string(it->second->getID());
                arr += '/' + to_string(it->second->getIncome());
                this->printer.push_back(arr);
                ++it;
            }
            st = st->getNext(); // Move to the next node
        }
    }
}

const vector<string> &BpTree::printData(string start, string end) // Function for outputting data; stores data in a vector and returns it
{
    this->printer.clear();
    if (start == "" && end == "") // If no arguments are given
    {
        searchAll(); // Output all data
    }
    else if (start != "" && end != "") // If two arguments are given
    {
        searchRange(start, end); // Output all data between the two keys
    }
    return this->printer; // Return the vector with stored data; if invalid arguments are given, returns an empty vector for external error handling
}

vector<EmployeeData *> BpTree::searchDeptNo(int dept_no) // Function to find data by department code for adding to selection tree
{
    vector<EmployeeData *> Data; // Vector to store multiple data
    BpTreeNode *st = this->head; // Start from the head of the data node
    while (st)                           // Until the end of the data node (while not NULL)
    {
        auto it = st->getDataMap()->begin();  // From the first data in the data node
        while (it != st->getDataMap()->end()) // Repeat until the last data in the data node
        {
            if (it->second->getDeptNo() == dept_no) // If it matches the department code
            {
                Data.push_back(it->second); // Store the data in the vector
            }
            ++it;
        }
        st = st->getNext(); // Move to the next node
    }
    return Data; // Return the vector containing the data
}
