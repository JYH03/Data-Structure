#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <set>

using namespace std;

Manager::Manager()
{
    // Declares the .txt file to be used as Music_List and Log files.
    this->Music_List = "Music_List.txt";
    this->Log = "log.txt";
}

Manager::~Manager()
{
    // Close both input and output streams.
    this->fcmd.close();
    this->flog.close();
}

void Manager::run(const char *command)
{
    // Open both input and output streams.
    this->fcmd.open(command);
    this->flog.open(this->Log, ios::app);
    if (this->fcmd.is_open())
    {
        // Save a line from the command to tmp.
        string tmp, Order, subOrder, data, dummy;
        // Get Order, subOrder (ex: artist), data (ex: PSY) from tmp and run it according to Order.
        while (getline(this->fcmd, tmp))
        {
            this->ss.clear();
            this->ss.str(tmp);
            dummy = "";
            data = "";
            getline(this->ss, Order, ' ');
            if (Order == "LOAD")
            {
                getline(this->ss, dummy);
                if (dummy == "")
                    LOAD();
                else
                    ErrorPrint(1000);
            }
            else if (Order == "ADD")
            {
                getline(this->ss, data);
                ADD(data);
            }
            else if (Order == "QPOP")
            {
                getline(this->ss, dummy);
                if (dummy == "")
                    QPOP();
                else
                    ErrorPrint(1000); // error print when more data is entered than necessary
            }
            else if (Order == "SEARCH")
            {
                getline(this->ss, subOrder, ' ');
                getline(this->ss, data);
                if (subOrder != "" && data != "")
                    SEARCH(subOrder, data);
                else
                    ErrorPrint(1000); // error print when more data is entered than necessary
            }
            else if (Order == "MAKEPL")
            {
                getline(this->ss, subOrder, ' ');
                getline(this->ss, data);
                if (subOrder != "" && data != "")
                    MAKEPL(subOrder, data);
                else
                    ErrorPrint(1000); // error print when more data is entered than necessary
            }
            else if (Order == "PRINT")
            {
                getline(this->ss, subOrder, ' ');
                getline(this->ss, dummy);
                if (dummy == "")
                    PRINT(subOrder);
                else
                    ErrorPrint(1000); // error print when more data is entered than necessary
            }
            else if (Order == "DELETE")
            {
                getline(this->ss, subOrder, ' ');
                getline(this->ss, data);
                if (subOrder != "" && data != "")
                    DELETE(subOrder, data);
                else
                    ErrorPrint(1000);
            }
            else if (Order == "EXIT")
            {
                getline(this->ss, dummy);
                if (dummy == "")
                    EXIT();
                else
                    ErrorPrint(1000); // error print when more data is entered than necessary
            }
            else
            {
                ErrorPrint(1000); // When an invalid command is entered
            }
        }
    }
    else
    {
        ErrorPrint(1000);
    }
}

void Manager::LOAD()
{
    if (!this->q.empty())
    {
        ErrorPrint(100);
        return;
    }

    // Open Music_List with fmusic
    this->fmusic.open(this->Music_List);
    if (!this->fmusic.is_open())
    {
        ErrorPrint(100);
        this->fmusic.close();
        return;
    }
    else
    {
        // Validate all input before changing the queue.
        string tmp, t_artist, t_title, t_run_time;
        vector<string> artists, titles, run_times;
        set<pair<string, string>> songs;
        while (getline(this->fmusic, tmp))
        {
            if (count(tmp.begin(), tmp.end(), '|') != 2)
            {
                ErrorPrint(100);
                this->fmusic.close();
                return;
            }
            this->ss.clear();
            this->ss.str(tmp);
            getline(ss, t_artist, '|');
            getline(ss, t_title, '|');
            getline(ss, t_run_time);
            if (t_artist.empty() || t_title.empty() || time_int(t_run_time) < 0 ||
                !songs.insert(make_pair(t_artist, t_title)).second || artists.size() >= 100)
            {
                ErrorPrint(100);
                this->fmusic.close();
                return;
            }
            artists.push_back(t_artist);
            titles.push_back(t_title);
            run_times.push_back(t_run_time);
        }
        if (artists.empty())
        {
            ErrorPrint(100);
            this->fmusic.close();
            return;
        }

        this->flog << "========LOAD========\n";
        for (size_t i = 0; i < artists.size(); ++i)
        {
            this->q.push(artists[i], titles[i], run_times[i]);
            this->flog << artists[i] << "/" << titles[i] << "/" << run_times[i] << '\n';
        }
        this->flog << "=======================" << endl;
    }
    this->fmusic.close(); // Exit ifstream.
}

void Manager::ADD(string data)
{
    string t_artist, t_title, t_run_time, dummy;
    this->ss.clear();
    this->ss.str(data);
    getline(this->ss, t_artist, '|');
    getline(this->ss, t_title, '|');
    getline(this->ss, t_run_time, '|');
    getline(this->ss, dummy);
    if (count(data.begin(), data.end(), '|') != 2 || dummy != "" ||
        t_artist == "" || t_title == "" || time_int(t_run_time) < 0)
    {
        ErrorPrint(1000);
        return;
    }
    // If the music_List normally gives data and there is no duplicate data in the queue
    if (!this->q.exist(t_artist, t_title))
    {
        if (this->q.push(t_artist, t_title, t_run_time)) // Insert into queue. If the queue is full, it will fail.
        {
            this->flog << "========ADD========\n";
            this->flog << t_artist << "/" << t_title << "/" << t_run_time << '\n';
            this->flog << "====================\n";
        }
        else
        {
            // Output error messages
            ErrorPrint(200);
            return;
        }
    }
    else
    {
        // Output error messages
        ErrorPrint(200);
        return;
    }
}

void Manager::QPOP()
{
    vector<string> tmp_artist, tmp_title;
    vector<int> tmp_time;
    // If the queue is full, output an error message.
    if (this->q.empty())
    {
        ErrorPrint(300);
        return;
    }
    MusicQueueNode *node;
    int Size;
    node = this->q.front();
    while (node != NULL)
    {
        tmp_artist.push_back(node->Artist());
        tmp_title.push_back(node->Title());
        int parsed_time = time_int(node->Run_time());
        if (parsed_time < 0)
        {
            ErrorPrint(300);
            return;
        }
        tmp_time.push_back(parsed_time);
        node = node->Next();
    }
    Size = tmp_artist.size();
    for (int i = 0; i < Size; i++)
    {
        // Search for the corresponding artist in BST, output error message when Titles overlap, otherwise insert
        this->ab.search(tmp_artist[i]);
        if (this->ab.Target() != NULL)
        {
            if (this->ab.Target()->search(tmp_title[i]))
            {
                ErrorPrint(300);
                return;
            }
        }
    }
    // insert data
    for (int i = 0; i < Size; i++)
    {
        this->ab.search(tmp_artist[i]);
        this->ab.insert(tmp_artist[i], tmp_title[i], tmp_time[i]);
        this->tb.search(tmp_title[i]);
        this->tb.insert(tmp_title[i], tmp_artist[i], tmp_time[i]);
    }
    while (!this->q.empty())
        this->q.pop();
    SuccessPrint("QPOP"); // Output successful message because all qpop is successful
}

void Manager::SEARCH(string Order, string Data)
{
    vector<string> printer; // Variable printer to receive data to be output from BST
    // extracting artist, Title information
    string A, T, dummy;
    this->ss.clear();
    this->ss.str(Data);
    // 1. ARTIST
    if (Order == "ARTIST")
    {
        getline(this->ss, A, '|');
        getline(this->ss, T);
        if (A.empty() || T != "" || Data.find('|') != string::npos)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->ab.search(A); // Explore the artist node that matches the data, ab.target points to the corresponding node
        if (this->ab.Target() != NULL)
        {
            printer = this->ab.print(A); // Get all the song data of that artist through print function
        }
        else
        {
            ErrorPrint(400); // Output error messages
            return;
        }
    }
    // 2. TITLE
    else if (Order == "TITLE")
    {
        getline(this->ss, T, '|');
        getline(this->ss, A);
        if (T.empty() || A != "" || Data.find('|') != string::npos)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->tb.search(T); // Explore the artist node that matches the data, tb.target points to the corresponding node
        if (this->tb.Target() != NULL)
        {
            printer = this->tb.print(T); // Get all the song data of that title through print function
        }
        else
        {
            ErrorPrint(400); // Output error messages
            return;
        }
    }
    // 3.SONG
    else if (Order == "SONG")
    {
        getline(this->ss, A, '|');
        getline(this->ss, T, '|');
        getline(this->ss, dummy);
        if (A.empty() || T.empty() || dummy != "" ||
            count(Data.begin(), Data.end(), '|') != 1)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->ab.search(A);                                            // Implemented to find in artistBST
        if (this->ab.Target() != NULL && this->ab.Target()->search(T)) // Explore the artist node that matches the data, ab.target points to the corresponding node
        {
            printer = this->ab.print(A, T); // Get song data that matches artist and Title through print function
        }
        else
        {
            ErrorPrint(400); // Output error messages
            return;
        }
        if (printer.empty()) // If there's no song that matches
        {
            ErrorPrint(400); // Output error messages
            return;
        }
    }
    else // invalid command
    {
        ErrorPrint(1000); // Output error messages
        return;
    }
    // Output song information searched through printer
    this->flog << "========SEARCH========\n";
    for (unsigned int i = 0; i < printer.size(); i++)
        flog << printer[i] << '\n';
    this->flog << "======================\n";
}

void Manager::MAKEPL(string Order, string Data)
{
    vector<string> printer;
    string A, T, dummy;
    this->ss.clear();
    this->ss.str(Data);
    // 1. ARTIST
    if (Order == "ARTIST")
    {
        // extracting artist
        getline(this->ss, A, '|');
        getline(this->ss, T); // use dummy data
        if (A.empty() || T != "" || Data.find('|') != string::npos)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->ab.search(A); // Find the artist node corresponding to the given data

        // When ArtistBST has the corresponding artist node and pl is not full
        if (this->ab.Target() != NULL && this->ab.Target()->Count() + this->pl.Count() <= 10 && this->ab.Target()->Count() > 0)
        {
            // check duplicate data
            printer = this->ab.Target()->Title();
            for (int i = 0; i < this->ab.Target()->Count(); i++)
            {
                if (this->pl.exist(A, printer[i]))
                {
                    ErrorPrint(500); // Failed when duplicate data comes in, Output error messages
                    return;
                }
            }
            // insert
            for (int i = 0; i < this->ab.Target()->Count(); i++)
            {
                this->pl.insert_node(A, printer[i], this->ab.Target()->Rt()[i]);
            }
        }
        else
        {
            ErrorPrint(500); // Output error messages
            return;
        }
    }
    // 2. TITLE
    else if (Order == "TITLE")
    {
        // extracting title
        getline(this->ss, T, '|');
        getline(this->ss, A); // use dummy data
        if (T.empty() || A != "" || Data.find('|') != string::npos)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->tb.search(T); // Find the title node corresponding to the given data

        // When TitleBST has the corresponding title node and pl is not full
        if (tb.Target() != NULL && this->tb.Target()->Count() + this->pl.Count() <= 10 && this->tb.Target()->Count() > 0)
        {
            // check duplicate data
            printer = this->tb.Target()->Artist();
            for (int i = 0; i < this->tb.Target()->Count(); i++)
            {
                if (this->pl.exist(printer[i], T))
                {
                    ErrorPrint(500); // Failed when duplicate data comes in, Output error messages
                    return;
                }
            }
            // insert
            for (int i = 0; i < this->tb.Target()->Count(); i++)
            {
                this->pl.insert_node(printer[i], T, this->tb.Target()->Rt()[i]); // Add this song to the PlayList
            }
        }
        else
        {
            ErrorPrint(500); // Output error messages
            return;
        }
    }
    // 3.SONG
    else if (Order == "SONG")
    {
        // extracting artist, Title information
        getline(this->ss, A, '|');
        getline(this->ss, T, '|');
        getline(this->ss, dummy);
        if (A.empty() || T.empty() || dummy != "" ||
            count(Data.begin(), Data.end(), '|') != 1)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->ab.search(A);
        if (this->ab.Target() != NULL && this->pl.Count() + 1 <= 10 && this->ab.Target()->search(T))
        {
            if (this->pl.exist(A, T)) //// check duplicate data
            {
                ErrorPrint(500); // Failed when duplicate data comes in, Output error messages
                return;
            }
            for (int i = 0; i < this->ab.Target()->Count(); i++)
            {
                if (this->ab.Target()->Title()[i] == T)
                {
                    this->pl.insert_node(A, T, this->ab.Target()->Rt()[i]);
                    break;
                }
            }
        }
        else
        {
            ErrorPrint(500); // Output error messages
            return;
        }
    }
    else // invalid command
    {
        ErrorPrint(1000); // Output error messages
        return;
    }
    // The part that outputs the information in the PlayList
    printer = this->pl.print();
    if (!printer.empty())
    {
        this->flog << "========MAKEPL========\n";
        for (string it : printer)
            flog << it << '\n';
        flog << "Count : " << this->pl.Count() << " / 10\n";
        flog << "Time : " << this->pl.time_string(pl.run_time(), 1) << '\n';
        flog << "====================\n";
    }
}
void Manager::PRINT(string Order)
{
    vector<string> printer;
    if (Order == "ARTIST")
    {
        if (this->ab.Root() == NULL)
        {
            ErrorPrint(600);
            return;
        }
        printer = this->ab.print(); // Get the data of artistBST through print function
    }
    else if (Order == "TITLE")
    {
        if (this->tb.Root() == NULL)
        {
            ErrorPrint(600);
            return;
        }
        printer = this->tb.print();
    }
    else if (Order == "LIST")
    {
        if (this->pl.empty())
        {
            ErrorPrint(600);
            return;
        }
        printer = this->pl.print(); // Get data from PlayList through print function
    }
    else // invalid command
    {
        ErrorPrint(1000); // Output error messages
        return;
    }
    this->flog << "========PRINT========\n";
    if (Order == "ARTIST")
        this->flog << "ArtistBST\n";
    else if (Order == "TITLE")
        this->flog << "TitleBST\n";
    for (unsigned int i = 0; i < printer.size(); i++)
    {
        this->flog << printer[i] << '\n'; // Output the obtained data
    }
    // The part that outputs the information in the PlayList
    if (Order == "LIST")
    {
        this->flog << "Count : " << this->pl.Count() << " / 10\n";
        this->flog << "Time : " << this->pl.time_string(pl.run_time(), 1) << '\n';
    }
    this->flog << "====================\n";
}

void Manager::DELETE(string &Order, string &Data)
{
    string A, T, dummy;
    ss.clear();
    ss.str(Data);
    // ARTIST
    if (Order == "ARTIST")
    {
        // extracting artist
        getline(this->ss, A, '|');
        getline(this->ss, T);
        if (A.empty() || T != "" || Data.find('|') != string::npos)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->ab.search(A);
        if (this->ab.Target() != NULL)
        {
            // Delete all corresponding songs from BST and PL
            this->ab.Delete(A);
            this->tb.Delete("", A);
            this->pl.delete_node(A);
        }
        else
        {
            ErrorPrint(700); // Output error messages
            return;
        }
    }
    // Title
    else if (Order == "TITLE")
    {
        // extracting title
        getline(this->ss, T, '|');
        getline(this->ss, A);
        if (T.empty() || A != "" || Data.find('|') != string::npos)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        this->tb.search(T);
        if (this->tb.Target() != NULL)
        {
            // Delete all corresponding songs from BST and PL
            this->ab.Delete("", T);
            this->tb.Delete(T);
            this->pl.delete_node("", T);
        }
        else
        {
            ErrorPrint(700); // Output error messages
            return;
        }
    }
    // List
    else if (Order == "LIST") // Delete song data from PlayList only
    {
        // extracting artist, Title information
        getline(this->ss, A, '|');
        getline(this->ss, T, '|');
        getline(this->ss, dummy);
        if (A.empty() || T.empty() || dummy != "" ||
            count(Data.begin(), Data.end(), '|') != 1)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        if (this->pl.exist(A, T)) // Delete a song if it's on the PlayList,
        {
            this->pl.delete_node(A, T);
        }
        else
        {
            ErrorPrint(700); // Output error messages
            return;
        }
    }
    // Song
    else if (Order == "SONG")
    {
        // extracting artist, Title information
        getline(this->ss, A, '|');
        getline(this->ss, T, '|');
        getline(this->ss, dummy);
        if (A.empty() || T.empty() || dummy != "" ||
            count(Data.begin(), Data.end(), '|') != 1)
        {
            ErrorPrint(1000); // Output error messages
            return;
        }
        // Delete all corresponding songs from BST and PL
        this->ab.search(A);
        if (this->ab.Target() != NULL && this->ab.Target()->search(T)) // Explore with ArtistBST, because each BST contains the same information
        {
            this->ab.Delete(A, T);
            this->tb.Delete(T, A);
            this->pl.delete_node(A, T);
        }
        else
        {
            ErrorPrint(700); // Output error messages
            return;
        }
    }
    else // invalid command
    {
        ErrorPrint(1000); // Output error messages
        return;
    }
    SuccessPrint("DELETE");
}
void Manager::EXIT() // Output Success and close the input and output streams.
{
    SuccessPrint("EXIT");
    this->fcmd.close();
    this->flog.close();
}
int Manager::time_int(const string &time)
{
    size_t colon = time.find(':');
    if (colon == string::npos || colon == 0 || colon != time.rfind(':') ||
        time.size() - colon - 1 != 2)
        return -1;

    string minutes_text = time.substr(0, colon);
    string seconds_text = time.substr(colon + 1);
    if (!all_of(minutes_text.begin(), minutes_text.end(),
                [](unsigned char ch) { return isdigit(ch) != 0; }) ||
        !all_of(seconds_text.begin(), seconds_text.end(),
                [](unsigned char ch) { return isdigit(ch) != 0; }))
        return -1;

    try
    {
        int minutes = stoi(minutes_text);
        int seconds = stoi(seconds_text);
        if (seconds >= 60 ||
            minutes > (numeric_limits<int>::max() - seconds) / 60)
            return -1;
        return minutes * 60 + seconds;
    }
    catch (...)
    {
        return -1;
    }
}
void Manager::SuccessPrint(string Order) // Function that outputs Success
{
    if (this->flog.is_open())
    {
        this->flog << "========" << Order << "========\n";
        this->flog << "Success\n====================\n";
    }
}
void Manager::ErrorPrint(int ErrorValue) // Function that outputs Error
{
    if (this->flog.is_open())
    {
        this->flog << "========ERROR========\n";
        this->flog << ErrorValue << "\n======================\n";
    }
}
