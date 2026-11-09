#include "ArtistBST.h"
using namespace std;

ArtistBST::ArtistBST() // Constructor
{
    this->root = NULL;
    this->parent = NULL;
    this->target = NULL;
}
ArtistBST::~ArtistBST() // Destructor, Delete all nodes within BST
{
    Delete(); // Delete nodes using the Postorder traversal (left - right -> parent)
}

void ArtistBST::insert(const string &artist, const string &title, const int run_time) // Always set target through search function
{
    if (this->target == NULL) // Create a node and connect if the artist does not exist
    {
        this->target = new ArtistBSTNode(artist, title, run_time); // Create a new node
        if (this->root == NULL)                                    // When it's the first node
        {
            this->root = this->target;
        }
        else // Not the first node == When a node has a parent node
        {
            // Compare the size with the parent node and connect it to the left if it is smaller. Connect it to the right if it is larger.
            if (this->parent->Artist() > artist)
            {
                this->parent->LinkLeft(this->target);
            }
            else
            {
                this->parent->LinkRight(this->target);
            }
        }
    }
    else // When there is a duplicate artist node
    {
        this->target->set(title, run_time);
    }
}

void ArtistBST::search(const string &artist) // Explore if ArtistBST has duplicate nodes
{
    this->target = this->root;
    this->parent = NULL;
    while (this->target != NULL)
    {
        if (this->target->Artist() == artist)
        {
            break;
        }
        this->parent = this->target;
        this->target = this->target->Compare(artist);
    }
}
bool ArtistBST::empty() // Verify that the BST is empty
{
    if (this->root == NULL)
        return true;
    else
        return false;
}
const vector<string> &ArtistBST::print(string artist, string title) // Returns a song corresponding to the given data
{
    // Always set target through search function
    this->data.clear();              // Initialize the variable to return
    if (artist == "" && title == "") // print all data of ArtistBST
    {
        this->target = this->root;
        AllData(this->target); // Save all data using Inorder traversal
    }
    else if (artist != "" && title != "") // Save song data corresponding to artist and Title
    {
        for (int i = 0; i < this->target->Count(); i++)
        {
            if (this->target->Title()[i] == title)
            {
                string arr = artist + '/' + title + '/' + time_string(this->target->Rt()[i]);
                this->data.push_back(arr);
            }
        }
    }
    else if (artist != "") // save data of Artist
    {
        for (int i = 0; i < this->target->Count(); i++)
        {
            string arr = artist + '/' + this->target->Title()[i] + '/' + time_string(this->target->Rt()[i]);
            this->data.push_back(arr);
        }
    }
    return this->data; // return data
}
void ArtistBST::AllData(ArtistBSTNode *node) // Inorder traversal[left->parent->right], Save all song data for each node
{
    if (node == NULL)
        return;
    AllData(node->Left());
    for (int i = 0; i < node->Count(); i++)
    {
        string arr = node->Artist() + '/' + node->Title()[i] + '/' + time_string(node->Rt()[i]);
        this->data.push_back(arr);
    }
    AllData(node->Right());
}
void ArtistBST::Delete(string artist, string title) // Delete Management Function
{
    if (!empty())
    {
        if (artist != "")
        {
            search(artist);
            if (title == "" && this->target != NULL) // If only artist information is entered, find the node and delete it
            {
                delete_node(this->target);
            }
            else if (artist != "" && title != "" && this->target != NULL) // If both artist and Title information are entered, find the node and delete it
            {
                this->target->deleteTitle(title);
                if (this->target->Count() == 0)
                    delete_node(this->target);
            }
        }
        else
        {
            this->target = this->root;
            this->parent = NULL;
            if (title != "") // If only title information is entered, Delete the song data corresponding to the title on all nodes
            {
                preTravel(this->target, title); // preTraversal
            }
            else
            {
                postTravel(this->target); // postTraversal, delete all node
                this->root = NULL;
                this->target = NULL;
                this->parent = NULL;
            }
        }
    }
    else
    {
        return; // if BST is empty
    }
}
void ArtistBST::delete_node(ArtistBSTNode *node) // Delete node function, delete according to number of child nodes (0-2)
{
    ArtistBSTNode *tmp;
    // leaf node (degree: 0), delete and initialize the parent node's corresponding child pointer
    if (node->Left() == NULL && node->Right() == NULL)
    {
        if (node != this->root) // When not the root node
        {
            // Disconnect from the parent node
            if (this->parent->Left() == node)
                this->parent->LinkLeft(NULL);
            else
                this->parent->LinkRight(NULL);
        }
        else
            this->root = NULL; // BST is empty
        delete node;
    }
    // When there is only one child node(degree: 1), Connect the child node of the node to delete with the parent node
    else if (node->Left() == NULL) // Right child node
    {
        if (node != this->root) // When not the root node
        {
            // Connect parent and child node of the node to delete
            tmp = node->Right();
            if (this->parent->Left() == node)
                this->parent->LinkLeft(tmp);
            else
                this->parent->LinkRight(tmp);
        }
        else
        {
            // change root
            this->root = node->Right();
            this->parent = NULL;
        }
        delete node;
    }
    else if (node->Right() == NULL) // Left child node
    {
        if (node != this->root) // When not the root node
        {
            // Connect parent and child node of the node to delete
            tmp = node->Left();
            if (this->parent->Left() == node)
                this->parent->LinkLeft(tmp);
            else
                this->parent->LinkRight(tmp);
        }
        else
        {
            // change root
            this->root = node->Left();
            this->parent = NULL;
        }
        delete node;
    }
    // When there are all child nodes(degree: 2)
    else
    {
        this->parent = node;
        tmp = findMin(node->Right()); // Find the node with the smallest value (leftmost child node) among the right child nodes
        node->copy(tmp);              // Copy data to the node to delete
        delete_node(tmp);             // Delete the node with the smallest value
    }
}

ArtistBSTNode *ArtistBST::findMin(ArtistBSTNode *node) // Find the node with the smallest value (leftmost child node) among the right child nodes
{
    while (node && node->Left())
    {
        this->parent = node;
        node = node->Left();
    }
    return node;
}
void ArtistBST::preTravel(ArtistBSTNode *node, const string &title) // preTraversal and Delete title data
{
    if (node == NULL)
        return;
    ArtistBSTNode *L = node->Left();
    ArtistBSTNode *R = node->Right();
    node->deleteTitle(title);
    if (node->Count() <= 0) // Delete an empty node
    {
        search(node->Artist());
        if (node->Left() != NULL && node->Right() != NULL) // When you have two child nodes
        {
            delete_node(node);
            preTravel(node, title); // Restart on the node because the data has been copied without being deleted
            return;                 // return to avoid duplication
        }
        else
        {
            delete_node(node);
        }
    }
    preTravel(L, title);
    preTravel(R, title);
}
void ArtistBST::postTravel(ArtistBSTNode *node) // postTraversal, delete all node
{
    if (node == NULL)
        return;
    postTravel(node->Left());
    postTravel(node->Right());
    delete node;
}
string ArtistBST::time_string(int time) // Converting time in int format to string
{
    string stringtime;
    stringtime += to_string(time / 60) + ':';
    if (time % 60 < 10)
        stringtime += '0';
    stringtime += to_string(time % 60);
    return stringtime;
}

ArtistBSTNode *ArtistBST::Target() // return target pointer
{
    return this->target;
}

ArtistBSTNode *ArtistBST::Root() // return root pointer
{
    return this->root;
}
