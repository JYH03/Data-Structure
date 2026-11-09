#include "Graph.h"
// Pure virtual function, for inheritance by ListGraph and MatrixGraph
Graph::Graph(bool type, int size)
{
	m_Type = type;
	m_Size = size;
}

Graph::~Graph()
{
}

bool Graph::getType() { return m_Type; }
int Graph::getSize() { return m_Size; }