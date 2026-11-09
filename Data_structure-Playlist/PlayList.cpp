#include "PlayList.h"
#include "PlayListNode.h"
using namespace std;

PlayList::PlayList() // constructor
{
    this->cursor = NULL;
    this->head = NULL;
    this->count = 0;
    this->time = 0;
}
PlayList::~PlayList() // destructor, Delete all nodes within Circular Linked List
{
    if (!empty())
    {
        while (this->count > 0)
        {
            this->cursor = this->head;
            this->head = this->head->Next();
            delete this->cursor;
            this->count--;
        }
        this->head = NULL;
        this->cursor = NULL;
    }
}
void PlayList::insert_node(const string &artist, const string &title, const int &run_time)
{
    if (!empty()) // if Circular Linked List is not empty
    {
        // create a new node after the last node, and connect with the previous last node and head node
        this->cursor = new PlayListNode(artist, title, run_time);
        this->head->Prev()->LinkNext(this->cursor);
        this->cursor->LinkPrev(this->head->Prev());
        this->head->LinkPrev(this->cursor);
        this->cursor->LinkNext(this->head);
        // Change the play time and number of nodes in the Circular Linked List
        this->time += run_time;
        this->count++;
    }
    else // If the Circular Linked List is empty
    {
        // After creating a node, set the node's next pointer and prev pointer to point to the node itself
        this->head = new PlayListNode(artist, title, run_time);
        this->head->LinkNext(this->head);
        this->head->LinkPrev(this->head);
        this->time = run_time;
        this->count = 1;
    }
    return;
}

void PlayList::delete_node(string artist, string title)
{
    PlayListNode *ptr;
    this->cursor = this->head;
    for (int i = 0; i < this->count; i++)
    {
        if ((artist == this->cursor->Artist() || artist == "") && (title == this->cursor->Title() || title == "")) // Delete only nodes that correspond to the given data
        {
            bool deleting_head = (this->cursor == this->head);
            ptr = this->cursor->Next();
            ptr->LinkPrev(this->cursor->Prev());
            this->cursor->Prev()->LinkNext(ptr);
            // setting time and delete
            this->time -= this->cursor->Runtime_sec();
            delete this->cursor;
            if (deleting_head) // If the head node is deleted, change the head to the next node
            {
                this->head = ptr;
            }
            this->count--;
            i--;
            if (empty()) // when PlayList is empty
            {
                this->head = NULL;
                this->cursor = NULL;
            }
            else
                this->cursor = ptr;
        }
        else
        {
            this->cursor = this->cursor->Next();
        }
    }
}

bool PlayList::empty() // Verify that the Circular Linked List is empty
{
    if (this->count <= 0)
        return true;
    else
        return false;
}

bool PlayList::full() // Verify that the Circular Linked List is full
{
    if (this->count >= 10)
        return true;
    else
        return false;
}

bool PlayList::exist(const string &artist, const string &title) // Verify that a song with the same artist and title exists
{
    if (!empty())
    {
        this->cursor = this->head;
        for (int i = 0; i < this->count; i++)
        {
            if (this->cursor->Artist() == artist && this->cursor->Title() == title)
            {
                return true;
            }
            this->cursor = this->cursor->Next();
        }
    }
    return false;
}

const vector<string> &PlayList::print() // Returns a song corresponding to the given data
{
    this->cursor = this->head;
    this->data.clear();
    for (int i = 0; i < this->count; i++)
    {
        this->data.push_back(this->cursor->Artist() + '/' + this->cursor->Title() + '/' + time_string(this->cursor->Runtime_sec(), 0));
        this->cursor = this->cursor->Next();
    }
    return this->data;
}

int PlayList::run_time() // return PlayList's Run_time
{
    return time;
}
int PlayList::Count() // return Number of songs on PlayList
{
    return this->count;
}
string PlayList::time_string(int time, bool type) // Converting time in int format to string
{
    string stringtime;
    if (type == true) // min, sec format
    {
        stringtime += to_string(time / 60) + "min " + to_string(time % 60) + "sec";
    }
    else // ':' format
    {
        stringtime += to_string(time / 60) + ':';
        if (time % 60 < 10)
            stringtime += '0';
        stringtime += to_string(time % 60);
    }
    return stringtime;
}
