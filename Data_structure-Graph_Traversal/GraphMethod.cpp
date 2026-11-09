#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <string>
#include <fstream>
#include <climits>
#include <iomanip>

#include "GraphMethod.h"

using namespace std;

// === Helper Structures ===
struct Edge
{
   int from, to, weight;

   // Operator for sorting
   bool operator<(const Edge &other) const
   {
      if (weight != other.weight)
         return weight < other.weight;
      if (from != other.from)
         return from < other.from;
      return to < other.to;
   }
};

struct UnionFind // Object for cycle detection
{
   vector<int> parent; // Vector storing parent node of each vertex
   UnionFind(int n)
   {
      parent.resize(n); // Resize according to number of vertices
      for (int i = 0; i < n; ++i)
         parent[i] = i; // Initial parent of each vertex is itself
   }
   int find(int i) // Check if parent nodes are the same (cycle detection)
   {
      if (parent[i] == i)                 // If parent is itself
         return i;                        // Return as top-level parent
      return parent[i] = find(parent[i]); // Recursively find parent
   }
   void unite(int i, int j) // Find parents of two vertices and make them the same (connect)
   {
      int root_i = find(i);
      int root_j = find(j);
      if (root_i != root_j)
         parent[root_i] = root_j;
   }
};

bool BFS(Graph *graph, char option, int vertex, ofstream &fout)
{
   int size = graph->getSize();      // Get graph size
   if (vertex < 0 || vertex >= size) // If vertex is out of range
      return false;                  // Return false

   vector<bool> visited(size, false); // Vector to check visited vertices
   queue<int> q;                      // Queue for BFS

   fout << "========BFS========" << endl;
   if (option == 'O') // If directed
      fout << "Directed Graph BFS" << endl;
   else // If undirected
      fout << "Undirected Graph BFS" << endl;

   fout << "Start: " << vertex << endl; // Print starting vertex

   q.push(vertex);         // Push first vertex into queue
   visited[vertex] = true; // Mark first vertex as visited

   bool first = true; // Variable for output format (->)
   while (!q.empty()) // While queue is not empty (still vertices to visit)
   {
      int current = q.front(); // Get front vertex
      q.pop();                 // Remove front vertex

      if (!first)        // If not the first vertex
         fout << " -> "; // Print arrow for format
      fout << current;   // Print visited vertex
      first = false;     // Mark that first vertex has been printed

      map<int, int> Adj; // Map to store edge info
      if (option == 'O') // If directed
         graph->getAdjacentEdgesDirect(current, &Adj);
      else if (option == 'X') // If undirected
         graph->getAdjacentEdges(current, &Adj);
      else
         return false;
      for (auto const &iter : Adj) // Traverse map
      {
         int next_vertex = iter.first; // Get next vertex
         if (!visited[next_vertex])    // If not visited
         {
            visited[next_vertex] = true; // Mark visited
            q.push(next_vertex);         // Push into queue
         }
      }
   }
   fout << endl;
   fout << "===================" << endl;
   return true;
}

bool DFS(Graph *graph, char option, int vertex, ofstream &fout)
{
   int size = graph->getSize();      // Get graph size
   if (vertex < 0 || vertex >= size) // If vertex is out of range
      return false;                  // Return false

   vector<bool> visited(size, false); // Vector to store visited info
   stack<int> s;                      // Stack for DFS

   fout << "========DFS========" << endl;
   if (option == 'O')
      fout << "Directed Graph DFS" << endl;
   else
      fout << "Undirected Graph DFS" << endl;

   fout << "Start: " << vertex << endl;

   s.push(vertex);      // Push first vertex into stack
   bool isFirst = true; // Variable for output format (->)

   while (!s.empty()) // Repeat until stack is empty (no more vertices to explore)
   {
      int current = s.top(); // Get most recent vertex
      s.pop();               // Remove it
      if (visited[current])  // Skip if already visited
      {
         continue;
      }
      visited[current] = true;
      if (!isFirst)
         fout << " -> " << current; // Print with arrow if not first
      else
      {
         fout << current; // Print first vertex
         isFirst = false; // Mark for future arrows
      }

      map<int, int> Adj; // Map to store edge info
      if (option == 'O') // If directed
         graph->getAdjacentEdgesDirect(current, &Adj);
      else if (option == 'X') // If undirected
         graph->getAdjacentEdges(current, &Adj);
      else
         return false;
      vector<int> neighbors;       // Vector to store neighbors
      for (auto const &iter : Adj) // Traverse edges
      {
         int next_vertex = iter.first; // Get vertex
         if (!visited[next_vertex])    // If not visited
         {
            neighbors.push_back(next_vertex); // Save
         }
      }

      for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) // Traverse in reverse to check smallest first
      {
         s.push(*it); // Push neighbor into stack
      }
   }
   fout << endl;
   fout << "===================" << endl;
   return true;
}

bool Kruskal(Graph *graph, ofstream &fout)
{
   int size = graph->getSize(); // Get number of vertices
   if (size == 0)               // If no vertices
      return false;             // Return false

   vector<Edge> edges;            // Vector to store edge combinations
   for (int i = 0; i < size; ++i) // Traverse all vertices
   {
      map<int, int> Adj;                // Map to store edge info
      graph->getAdjacentEdges(i, &Adj); // Get undirected graph edges
      for (auto const &iter : Adj)      // Traverse edges
      {
         int to = iter.first;                 // Connected vertex
         int weight = iter.second;            // Edge weight
         if (i < to)                          // Save only smaller->larger to avoid duplicates
            edges.push_back({i, to, weight}); // Save edge
      }
   }
   sort(edges.begin(), edges.end());            // Sort by weight
   UnionFind uf(size);                          // Cycle detection object
   int cost = 0;                                // Total cost
   int selected = 0;                            // a variable for counting the number of selected edges
   vector<vector<pair<int, int>>> result(size); // Vector for result output

   for (const auto &edge : edges) // Traverse all edges
   {
      if (uf.find(edge.from) != uf.find(edge.to)) // If parents are different
      {
         uf.unite(edge.from, edge.to); // Connect parents
         cost += edge.weight;          // Add weight
         selected++;
         // Save connected edge info
         result[edge.from].push_back(make_pair(edge.to, edge.weight));
         result[edge.to].push_back(make_pair(edge.from, edge.weight));
      }
   }
   if (size > 1 && selected < size - 1)
      return false;
   // Print result in required format
   fout << "========KRUSKAL========" << endl;
   for (int i = 0; i < size; ++i)
   {
      fout << "[" << i << "] ";
      sort(result[i].begin(), result[i].end()); // Sort ascending
      for (size_t j = 0; j < result[i].size(); ++j)
      {
         fout << result[i][j].first << "(" << result[i][j].second << ")";
         if (j < result[i].size() - 1)
            fout << " ";
      }
      fout << endl;
   }
   fout << "Cost: " << cost << endl;
   fout << "=======================" << endl;
   return true;
}

bool Dijkstra(Graph *graph, char option, int vertex, ofstream &fout)
{
   int size = graph->getSize(); // Check number of vertices
   if (vertex < 0 || vertex >= size)
      return false;

   for (int i = 0; i < size; i++) // Check all vertices
   {
      map<int, int> adj;
      graph->getAdjacentEdgesDirect(i, &adj);
      for (auto const &iter : adj)
      {
         if (iter.second < 0)
            return false; // Return false if negative weight exists
      }
   }

   vector<int> dist(size, INT_MAX);                                                    // Vector to store distances, initialized
   vector<int> parent(size, -1);                                                       // Vector to store previous vertex
   priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // Priority queue storing cost and vertex number

   dist[vertex] = 0; // Initialize starting vertex distance to 0
   pq.push(make_pair(0, vertex));

   while (!pq.empty())
   {
      // Get vertex with shortest distance
      int d = pq.top().first;
      int u = pq.top().second;
      pq.pop();

      // Skip if shorter path already exists
      if (d > dist[u])
         continue;

      // Check vertices connected to current vertex
      map<int, int> Adj;
      if (option == 'O')
         graph->getAdjacentEdgesDirect(u, &Adj);
      else if (option == 'X') // If undirected
         graph->getAdjacentEdges(u, &Adj);
      else
         return false;
      for (auto const &iter : Adj)
      {
         // Connected vertex and weight
         int v = iter.first;
         int weight = iter.second;

         if (dist[u] + weight < dist[v]) // Current distance + weight < existing path
         {
            dist[v] = dist[u] + weight;     // Update
            parent[v] = u;                  // Save parent
            pq.push(make_pair(dist[v], v)); // Push into queue for next exploration
         }
      }
   }
   // Output format
   fout << "========DIJKSTRA========" << endl;
   if (option == 'O')
      fout << "Directed Graph Dijkstra" << endl;
   else
      fout << "Undirected Graph Dijkstra" << endl;

   fout << "Start: " << vertex << endl;

   for (int i = 0; i < size; ++i)
   {
      fout << "[" << i << "] ";
      if (i == vertex)
      {
         if (dist[i] == INT_MAX) // Cannot reach
            fout << "x";
         else
            fout << vertex << " (0)";
      }
      else if (dist[i] == INT_MAX) // Destination not reachable
      {
         fout << "x";
      }
      else
      {
         vector<int> path; // Vector to store path
         int curr = i;
         while (curr != -1) // Trace parents
         {
            path.push_back(curr);
            curr = parent[curr];
         }
         reverse(path.begin(), path.end()); // Reverse order

         for (size_t k = 0; k < path.size(); k++)
         {
            fout << path[k];
            if (k != path.size() - 1)
               fout << " -> ";
         }
         fout << " (" << dist[i] << ")"; // Print final cost
      }
      fout << endl;
   }
   fout << "========================" << endl;
   return true;
}

bool Bellmanford(Graph *graph, char option, int s_vertex, int e_vertex, ofstream &fout)
{
   int size = graph->getSize(); // Number of vertices
   if (s_vertex < 0 || s_vertex >= size || e_vertex < 0 || e_vertex >= size)
      return false; // Return false if out of range

   vector<int> dist(size, INT_MAX); // Vector to store distances
   vector<int> parent(size, -1);    // Vector to store previous vertex
   dist[s_vertex] = 0;              // Set start vertex distance to 0

   // Store edge information of each vertex
   vector<Edge> allEdges;
   for (int i = 0; i < size; ++i)
   {
      map<int, int> Adj;
      if (option == 'O')
         graph->getAdjacentEdgesDirect(i, &Adj);
      else if (option == 'X') // If undirected
         graph->getAdjacentEdges(i, &Adj);
      else
         return false;
      for (auto const &iter : Adj)
      {
         int to = iter.first;
         int weight = iter.second;
         allEdges.push_back({i, to, weight});
      }
   }
   // Update shortest distance for reachable vertices
   for (int i = 0; i < size - 1; ++i)
   {
      for (const auto &edge : allEdges)
      {
         if (dist[edge.from] != INT_MAX && dist[edge.from] + edge.weight < dist[edge.to]) // Start vertex + weight < destination shortest distance
         {
            // Update
            dist[edge.to] = dist[edge.from] + edge.weight;
            parent[edge.to] = edge.from;
         }
      }
   }
   // Return false if negative cycle exists
   for (const auto &edge : allEdges)
   {
      if (dist[edge.from] != INT_MAX && dist[edge.from] + edge.weight < dist[edge.to])
      {
         return false;
      }
   }

   fout << "======BELLMANFORD======" << endl;
   if (option == 'O')
      fout << "Directed Graph Bellman-Ford" << endl;
   else
      fout << "Undirected Graph Bellman-Ford" << endl;

   if (dist[e_vertex] == INT_MAX) // If destination not reachable
   {
      fout << "x" << endl;
   }
   else
   {
      // Trace parent vertices to complete shortest path
      vector<int> path;
      int curr = e_vertex;
      while (curr != -1)
      {
         path.push_back(curr);
         curr = parent[curr];
      }
      reverse(path.begin(), path.end());
      for (size_t k = 0; k < path.size(); k++)
      {
         fout << path[k];
         if (k != path.size() - 1)
            fout << " -> ";
      }
      fout << endl;
      fout << "Cost: " << dist[e_vertex] << endl;
   }
   fout << "=======================" << endl;
   return true;
}

bool FLOYD(Graph *graph, char option, ofstream &fout)
{
   int size = graph->getSize();                                // Number of vertices
   vector<vector<int>> dist(size, vector<int>(size, INT_MAX)); // 2D vector to store distances

   for (int i = 0; i < size; ++i)
   {
      dist[i][i] = 0;
      map<int, int> Adj; // Map to store edge information
      if (option == 'O')
         graph->getAdjacentEdgesDirect(i, &Adj);
      else if (option == 'X') // If undirected
         graph->getAdjacentEdges(i, &Adj);
      else
         return false;
      // Fill 2D vector with edge information
      for (auto const &iter : Adj)
      {
         int to = iter.first;
         int weight = iter.second;
         if (weight < dist[i][to])
         {
            dist[i][to] = weight;
         }
      }
   }
   // Triple loop: if i->j is greater than i->k + k->j, update with smaller cost
   for (int k = 0; k < size; ++k)
   {
      for (int i = 0; i < size; ++i)
      {
         for (int j = 0; j < size; ++j)
         {
            if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX)
            {
               if (dist[i][j] > dist[i][k] + dist[k][j]) // i->j > i->k + k->j
               {
                  dist[i][j] = dist[i][k] + dist[k][j]; // Update
               }
            }
         }
      }
   }
   // Return false if negative cycle exists
   for (int i = 0; i < size; ++i)
   {
      if (dist[i][i] < 0)
         return false;
   }
   // Output format
   fout << "========FLOYD========" << endl;
   if (option == 'O')
      fout << "Directed Graph Floyd" << endl;
   else
      fout << "Undirected Graph Floyd" << endl;

   fout << "\t";
   for (int i = 0; i < size; i++)
      fout << "[" << i << "]" << "\t";
   fout << endl;

   for (int i = 0; i < size; ++i)
   {
      fout << "[" << i << "]" << "\t";
      for (int j = 0; j < size; ++j)
      {
         if (dist[i][j] == INT_MAX)
            fout << "x" << "\t";
         else
            fout << dist[i][j] << "\t";
      }
      fout << endl;
   }
   fout << "=====================" << endl;
   return true;
}

bool Centrality(Graph *graph, ofstream &fout)
{
   int size = graph->getSize(); // Number of vertices

   vector<vector<int>> dist(size, vector<int>(size, INT_MAX)); // 2D vector for Floyd method
   for (int i = 0; i < size; ++i)
   {
      dist[i][i] = 0;
      map<int, int> Adj;
      graph->getAdjacentEdges(i, &Adj);
      for (auto const &iter : Adj)
      {
         int to = iter.first;
         int weight = iter.second;

         dist[i][to] = weight;
      }
   }
   // Perform Floyd
   for (int k = 0; k < size; ++k)
   {
      for (int i = 0; i < size; ++i)
      {
         for (int j = 0; j < size; ++j)
         {
            if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX)
            {
               if (dist[i][j] > dist[i][k] + dist[k][j]) // i->j > i->k + k->j
               {
                  dist[i][j] = dist[i][k] + dist[k][j]; // Update
               }
            }
         }
      }
   }
   // Return false if negative cycle exists
   for (int i = 0; i < size; ++i)
   {
      if (dist[i][i] < 0)
         return false;
   }

   fout << "========CENTRALITY========" << endl;

   vector<long long> sums(size, 0);
   double M_central = -1.0;
   // Calculate sum of shortest paths to other vertices and compute centrality
   for (int i = 0; i < size; i++)
   {
      // Sum shortest paths from each vertex to others, then divide by n-1
      for (int j = 0; j < size; j++)
      {
         if (dist[i][j] != INT_MAX && i != j)
            sums[i] += dist[i][j];
      }
      double value = (sums[i] == 0) ? 0 : (double)(size - 1) / sums[i];
      if (value > M_central) // Record highest centrality
         M_central = value;
   }
   // Output part
   for (int i = 0; i < size; i++)
   {
      if (sums[i] == 0)
      {
         fout << "[" << i << "] x";
      }
      else
      {
         fout << "[" << i << "] " << (size - 1) << "/" << sums[i];
         double value = (double)(size - 1) / sums[i];
         if (M_central == value) // Check if vertex has highest centrality
         {
            fout << " <- Most Central";
         }
      }
      fout << endl;
   }
   fout << "==========================" << endl;
   return true;
}