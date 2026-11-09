#include "MusicQueue.h"
using namespace std;

MusicQueue::MusicQueue() // constructor
{
    this->head = NULL;
    this->rear = NULL;
    this->size = 0;
    this->capacity = 100;
}

MusicQueue::~MusicQueue() // destructor, delete all node
{
    MusicQueueNode *ptr = this->head;
    while (ptr != NULL)
    {
        this->head = this->head->Next();
        delete ptr;
        ptr = this->head;
    }
}

bool MusicQueue::empty() // Verify that the Queue is empty
{
    if (this->size <= 0 || this->head == NULL)
        return true;
    else
        return false;
}

bool MusicQueue::exist(string &artist, string &title) // Verify that a song with the same artist and title exists
{
    MusicQueueNode *ptr = this->head;
    if (!empty())
    {
        for (int i = 0; i < this->size; i++)
        {

            if (ptr->exist(artist, title))
            {
                return true;
            }
            ptr = ptr->Next();
        }
    }
    return false;
}

bool MusicQueue::push(string &artist, string &title, string &run_time) // Insert node with data input into queue
{
    if (empty()) // When it's the first node, Connect head, rear pointer to new node
    {
        this->head = new MusicQueueNode;
        this->head->insert(artist, title, run_time);
        this->rear = this->head;
        this->size = 1;
        return true;
    }
    else if (this->capacity > this->size) // When size is less than capacity, Insert after the last node
    {
        MusicQueueNode *ptr = new MusicQueueNode;
        ptr->insert(artist, title, run_time);
        this->size++;
        this->rear->LinkNext(ptr);
        ptr->LinkPrev(this->rear);
        ptr->LinkNext(NULL);
        this->rear = ptr;
        return true;
    }
    else
        return false; // Fail
}
void MusicQueue::pop() // Delete front node if queue is not empty
{
    if (!empty())
    {
        this->size--;
        if (this->head != this->rear)
        {
            this->head = this->head->Next();
            delete this->head->Prev();
            this->head->LinkPrev(NULL);
        }
        else
        {
            delete this->head;
            this->head = NULL;
            this->rear = NULL;
        }
    }
    return;
}
MusicQueueNode *MusicQueue::front() // Returns data from the first node if the queue is not empty
{
    if (!empty())
    {
        return this->head;
    }
    return NULL;
}
