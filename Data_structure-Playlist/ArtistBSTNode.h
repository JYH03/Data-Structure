#pragma once
#include "ArtistBST.h"
#include "MusicQueueNode.h"
#include <vector>
#include <string>
using namespace std;

class ArtistBSTNode
{
private:
    string artist;
    vector<string> title;
    string run_time;
    vector<int> rt;
    int rtAll = 0;
    int count = 0;
    ArtistBSTNode *left;
    ArtistBSTNode *right;

public:
    ArtistBSTNode() // constructor
    {
        this->artist = "";
        this->count = 0;
        this->left = NULL;
        this->right = NULL;
        this->rtAll = 0;
        this->run_time = "0:00";
    }
    ArtistBSTNode(const string &artist, const string &title, const int &run_time) // parameterized constructor
    {
        this->left = NULL;
        this->right = NULL;
        this->artist = artist;
        this->title.push_back(title);
        this->rt.push_back(run_time);
        this->rtAll += run_time;
        time_set();
        this->count++;
    }
    ~ArtistBSTNode() // destructor
    {
        this->left = NULL;
        this->right = NULL;
    }
    void set(const string &title, const int &run_time, string artist = "") // insert data
    {
        if (artist != "")
        {
            this->artist = artist;
        }
        this->title.push_back(title);
        this->rt.push_back(run_time);
        this->rtAll += run_time;
        time_set();
        this->count++;
    }
    bool search(string title) // Verify that the corresponding Title exists
    {
        for (int i = 0; i < this->count; i++)
        {
            if (this->title[i] == title)
            {
                return true;
            }
        }
        return false;
    }
    void copy(ArtistBSTNode *node) // Copy data
    {
        this->artist = node->Artist();
        this->title = node->Title();
        this->run_time = node->Run_time();
        this->rt = node->Rt();
        this->count = node->Count();
        this->rtAll = 0;
        for (int value : this->rt)
            this->rtAll += value;
    }
    void deleteTitle(const string &title) // delete title
    {
        for (int i = 0; i < this->count; i++)
        {
            if (this->title[i] == title)
            {
                this->title.erase(this->title.begin() + i);
                this->rtAll -= this->rt[i];
                this->rt.erase(this->rt.begin() + i);
                this->count--;
                time_set(); // Setting the Time
                return;
            }
        }
    }
    void time_set() // Function that changes the entire time data when the node's time data changes
    {
        this->run_time = to_string(this->rtAll / 60) + ':'; // minutes
        if (this->rtAll % 60 < 10)                               // Add 0 for less than 10 seconds
            this->run_time += '0';
        this->run_time += to_string(this->rtAll % 60); // second
    }
    // The functions below return private member variables or link node to pointers.
    const string &Artist()
    {
        return this->artist;
    }
    const vector<string> &Title()
    {
        return this->title;
    }
    const string &Run_time()
    {
        return this->run_time;
    }
    const vector<int> &Rt()
    {
        return this->rt;
    }
    const int &Count()
    {
        return this->count;
    }
    ArtistBSTNode *Left()
    {
        return this->left;
    }
    ArtistBSTNode *Right()
    {
        return this->right;
    }
    void LinkLeft(ArtistBSTNode *node)
    {
        this->left = node;
    }

    void LinkRight(ArtistBSTNode *node)
    {
        this->right = node;
    }
    ArtistBSTNode *Compare(const string &artist) // Return by comparing priorities of artist
    {
        if (this->artist > artist)
        {
            return Left();
        }
        else
        {
            return Right();
        }
    }
};
