#include "ListGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

using namespace std;

// Constructor
ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
    // kw_graph: stores vertex connection information (array of vector<int>)
    this->kw_graph = new vector<int>[size];
    // m_List: stores weight information (array of map<int, int>)
    this->m_List = new map<int, int>[size];
}

// Destructor
ListGraph::~ListGraph()
{
    // Release allocated memory for kw_graph and m_List
    delete[] kw_graph;
    delete[] m_List;
}

// Return undirected edge data
void ListGraph::getAdjacentEdges(int vertex, map<int, int> *m)
{
    getAdjacentEdgesDirect(vertex, m);     // First get directed edge data
    for (int row = 0; row < m_Size; row++) // For all vertices
    {
        if (row == vertex) // Skip self-loops
            continue;
        // Find incoming edges and insert them
        for (auto it : kw_graph[row])
        {
            if (it == vertex)
            {
                auto w = m_List[row].find(vertex);
                int weight = w->second;
                auto check = m->find(row);
                if (check != m->end() && check->second > weight) // Weight is determined as minimum when undirected
                {
                    check->second = weight;
                }
                else //"If there is no edge information, input data"
                    m->insert({row, weight});
            }
        }
    }
}
// Return directed edge data
void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int> *m)
{
    if (vertex < 0 || vertex >= m_Size)
        return;
    *m = m_List[vertex]; // Copy all outgoing edge info from given vertex
}

// Insert edge
void ListGraph::insertEdge(int from, int to, int weight)
{
    // Save information about vertices the current vertex points to
    kw_graph[from].push_back(to);
    // Store destination as key and weight as value
    m_List[from][to] = weight;
}

// Print graph
bool ListGraph::printGraph(ofstream *fout)
{
    if (m_Size < 0) // Return false if graph has no vertices
        return false;
    for (int i = 0; i < m_Size; i++)
    {
        // Output
        *fout << "[" << i << "]";
        if (m_List[i].size() != 0)
        {
            // Traverse m_List and print
            for (auto it : m_List[i])
            {
                int dest = it.first;
                int weight = it.second;
                *fout << " -> (" << dest << "," << weight << ")";
            }
        }
        else
            *fout << " -> ";
        *fout
            << endl;
    }
    return true;
}

// Input data
void ListGraph::setGraph(int pos, string data)
{
    stringstream ss(data);
    int to, weight;
    // Read two integers (destination, weight) separated by spaces and insert
    while (ss >> to >> weight)
    {
        insertEdge(pos, to, weight);
    }
}