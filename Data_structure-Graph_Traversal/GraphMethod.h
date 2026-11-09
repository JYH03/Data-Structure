#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"
#include <fstream>

using namespace std;

bool BFS(Graph *graph, char option, int vertex, ofstream &fout);
bool DFS(Graph *graph, char option, int vertex, ofstream &fout);
bool Kruskal(Graph *graph, ofstream &fout);
bool Dijkstra(Graph *graph, char option, int vertex, ofstream &fout);
bool Bellmanford(Graph *graph, char option, int s_vertex, int e_vertex, ofstream &fout);
bool FLOYD(Graph *graph, char option, ofstream &fout);
bool Centrality(Graph *graph, ofstream &fout);

#endif