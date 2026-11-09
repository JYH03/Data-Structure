#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Constructor
MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
    // Allocate memory for 2D array
    this->m_Mat = new int *[size];
    for (int i = 0; i < size; i++)
    {
        this->m_Mat[i] = new int[size];
        for (int j = 0; j < size; j++)
        {
            this->m_Mat[i][j] = 0; // Initialize with 0
        }
    }
}

// Destructor
MatrixGraph::~MatrixGraph()
{
    // Release memory for 2D array
    if (m_Mat)
    {
        for (int i = 0; i < m_Size; i++)
        {
            if (m_Mat[i])
                delete[] m_Mat[i];
        }
        delete[] m_Mat;
    }
}

// Function to input data
void MatrixGraph::setGraph(int pos, string data)
{
    stringstream ss(data);
    int weight;
    int col = 0;

    // Extract integers one by one separated by spaces
    while (ss >> weight && col < m_Size)
    {
        insertEdge(pos, col, weight); // Insert data
        col++;
    }
}

// Function to return undirected edge information
void MatrixGraph::getAdjacentEdges(int vertex, map<int, int> *m)
{
    if (vertex < 0 || vertex >= m_Size)    // If vertex is out of size range
        return;                            // Return
    for (int col = 0; col < m_Size; col++) // Repeat for all possible edges
    {
        if (col == vertex)
            continue;
        if (m_Mat[vertex][col] != 0 && m_Mat[col][vertex] != 0) // Weight is determined as minimum when undirected
            m->insert({col, min(m_Mat[vertex][col], m_Mat[col][vertex])});
        else if (m_Mat[vertex][col] != 0) // When there are only outgoing edges
            m->insert({col, m_Mat[vertex][col]});
        else if (m_Mat[col][vertex] != 0) ////When there are only incoming edges
            m->insert({col, m_Mat[col][vertex]});
    }
}

// Function to return directed edge information
void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int> *m)
{
    if (vertex < 0 || vertex >= m_Size)    // If vertex is out of size range
        return;                            // Return
    for (int col = 0; col < m_Size; col++) // Repeat for all possible edges
    {
        if (m_Mat[vertex][col] != 0) // Only outgoing edges
        {
            m->insert({col, m_Mat[vertex][col]}); // Insert data into given map
        }
    }
}

void MatrixGraph::insertEdge(int from, int to, int weight)
{
    if (from >= 0 && from < m_Size && to >= 0 && to < m_Size) // Edge info must be >=0 and < size
        m_Mat[from][to] = weight;                             // Save edge info
}

bool MatrixGraph::printGraph(ofstream *fout)
{
    if (m_Size < 0) // Return false if graph has no vertices
        return false;
    // Print first line
    *fout << "\t";
    for (int i = 0; i < m_Size; i++)
    {
        *fout << "[" << i << "]" << "\t";
    }
    *fout << endl;

    // Print matrix contents
    for (int i = 0; i < m_Size; i++)
    {
        *fout << "[" << i << "]" << "\t"; // Row number
        for (int j = 0; j < m_Size; j++)
        {
            *fout << m_Mat[i][j] << "\t";
        }
        *fout << endl;
    }
    return true; // Return success
}