#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <algorithm>

using namespace std;

Manager::Manager() // Constructor, open log.txt and initialize
{
	graph = nullptr;
	fout.open("log.txt", ios::out | ios::trunc);
	load = 0;
}

Manager::~Manager() // Destructor, delete graph and close log.txt if open
{
	if (load && graph)
		delete graph;
	if (fout.is_open())
		fout.close();
}

void Manager::run(const char *command_txt)
{
	ifstream fin;
	fin.open(command_txt, ios_base::in);
	if (!fin)
	{
		return;
	}
	string line, Order;
	while (getline(fin, line)) // Receive command
	{
		if (line.empty())
			continue;
		// Remove Windows-style newline character (\r)
		if (line.back() == '\r')
			line.pop_back();

		stringstream ss(line);
		ss >> Order;

		if (Order == "LOAD")
		{
			string filename, extra;
			if (!(ss >> filename) || (ss >> extra))
			{ // Error if arguments are missing or excessive
				printErrorCode(100);
			}
			else
			{
				LOAD(filename.c_str());
			}
		}
		else if (Order == "PRINT")
		{
			string extra;
			if (ss >> extra)
				printErrorCode(200); // Error if arguments are excessive
			else
				PRINT();
		}
		else if (Order == "BFS")
		{
			char option;
			int vertex;
			string extra;
			// Error if arguments are missing or excessive
			if (!(ss >> option >> vertex) || (ss >> extra))
			{
				printErrorCode(300);
			}
			else
			{
				mBFS(option, vertex);
			}
		}
		else if (Order == "DFS")
		{
			char option;
			int vertex;
			string extra;
			// Error if arguments are missing or excessive
			if (!(ss >> option >> vertex) || (ss >> extra))
			{
				printErrorCode(400);
			}
			else
			{
				mDFS(option, vertex);
			}
		}
		else if (Order == "KRUSKAL")
		{
			string extra;
			// Error if arguments are excessive
			if (ss >> extra)
				printErrorCode(500);
			else
				mKRUSKAL();
		}
		else if (Order == "DIJKSTRA")
		{
			char option;
			int vertex;
			string extra;
			// Error if arguments are missing or excessive
			if (!(ss >> option >> vertex) || (ss >> extra))
			{
				printErrorCode(600);
			}
			else
			{
				mDIJKSTRA(option, vertex);
			}
		}
		else if (Order == "BELLMANFORD")
		{
			char option;
			int st, des;
			string extra;
			// Error if arguments are missing or excessive
			if (!(ss >> option >> st >> des) || (ss >> extra))
			{
				printErrorCode(700);
			}
			else
			{
				mBELLMANFORD(option, st, des);
			}
		}
		else if (Order == "FLOYD")
		{
			char option;
			string extra;
			// Error if arguments are missing or excessive
			if (!(ss >> option) || (ss >> extra))
			{
				printErrorCode(800);
			}
			else
			{
				mFLOYD(option);
			}
		}
		else if (Order == "CENTRALITY")
		{
			string extra;
			// Error if arguments are excessive
			if (ss >> extra)
				printErrorCode(900);
			else
				mCentrality();
		}
		else if (Order == "EXIT")
		{
			fout << "========EXIT========" << endl;
			fout << "Success" << endl;
			fout << "====================" << endl;
			fin.close();
			return;
		}
	}
}

bool Manager::LOAD(const char *filename)
{
	// If a graph is already loaded, release it
	if (load && graph)
	{
		delete graph;
		graph = nullptr;
		load = 0;
	}
	// Open file with given filename
	ifstream data(filename);
	if (!data)
	{
		printErrorCode(100);
		return false;
	}
	string line;
	// Read first line, error if file is empty
	if (!getline(data, line) || line.empty())
	{
		printErrorCode(100);
		return false;
	}
	if (line.back() == '\r')
		line.pop_back();
	string type = line; // Graph type

	// Read second line, error if file is empty
	if (!getline(data, line) || line.empty())
	{
		printErrorCode(100);
		return false;
	}
	int size = stoi(line); // Graph size

	if (type == "L") // When List type information is given
	{
		// Create List graph (type 0)
		this->graph = new ListGraph(0, size);
		int vertex = 0;
		// Input graph information
		while (getline(data, line))
		{
			stringstream ss(line);
			int val;
			vector<int> numbers;
			// Read numbers
			while (ss >> val)
				numbers.push_back(val);
			if (numbers.size() == 1) // When it's a vertex part
			{
				vertex = numbers[0];
			}
			else
			{
				graph->setGraph(vertex, line);
			}
		}
	}
	else if (type == "M") // When Matrix type information is given
	{
		// Create Matrix graph (type 0)
		this->graph = new MatrixGraph(1, size);
		int row = 0;
		// Input data
		while (getline(data, line) && row < size)
		{
			graph->setGraph(row, line);
			row++;
		}
	}
	else // Error if data format is invalid
	{
		printErrorCode(100);
		return false;
	}
	// Mark as loaded and print Success
	load = 1;
	printSuccessCode("LOAD");
	return true;
}

bool Manager::PRINT()
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(200);
		return false;
	}
	// Print graph
	fout << "========PRINT========" << endl;
	graph->printGraph(&fout);
	fout << "=====================" << endl
		 << endl;
	return true;
}

bool Manager::mBFS(char option, int vertex)
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(300);
		return false;
	}
	if (!BFS(graph, option, vertex, fout)) // Execute BFS
	{
		printErrorCode(300);
		return false;
	}
	return true;
}

bool Manager::mDFS(char option, int vertex)
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(400);
		return false;
	}
	if (!DFS(graph, option, vertex, fout)) // Execute DFS
	{
		printErrorCode(400);
		return false;
	}
	return true;
}

bool Manager::mKRUSKAL()
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(500);
		return false;
	}
	if (!Kruskal(graph, fout)) // Execute Kruskal
	{
		printErrorCode(500);
		return false;
	}
	return true;
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(600);
		return false;
	}
	if (!Dijkstra(graph, option, vertex, fout)) // Execute Dijkstra
	{
		printErrorCode(600);
		return false;
	}
	return true;
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(700);
		return false;
	}
	if (!Bellmanford(graph, option, s_vertex, e_vertex, fout)) // Execute Bellmanford
	{
		printErrorCode(700);
		return false;
	}
	return true;
}

bool Manager::mFLOYD(char option)
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(800);
		return false;
	}
	if (!FLOYD(graph, option, fout)) // Execute Floyd
	{
		printErrorCode(800);
		return false;
	}
	return true;
}

bool Manager::mCentrality()
{
	if (!graph) // Error if graph does not exist
	{
		printErrorCode(900);
		return false;
	}
	if (!Centrality(graph, fout)) // Perform undirected Floyd search and calculate closeness centrality
	{
		printErrorCode(900);
		return false;
	}
	return true;
}

void Manager::printSuccessCode(string code) // print Success
{
	fout << "========" << code << "========" << endl;
	fout << "Success" << endl;
	fout << "=======================" << endl
		 << endl;
}

void Manager::printErrorCode(int n) // print Error
{
	fout << "========ERROR========" << endl;
	fout << n << endl;
	fout << "=====================" << endl
		 << endl;
}