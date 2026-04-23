#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

class Graph {
  int V;
  vector<vector<int>> adj;

public:
  Graph(int V) : V(V), adj(V) {}
  void addEdge(int v, int w) { adj[v].push_back(w); }
  void parallelBFS(int start) 
  {
    vector<bool> visited(V, false);
    vector<int> frontier, next_frontier;

    frontier.push_back(start);
    visited[start] = true;

    while (!frontier.empty()) 
    {
      next_frontier.clear();

    // Process current level in parallel
    #pragma omp parallel
    {
      vector<int> local_next;
      #pragma omp for nowait
      for (int i = 0; i < frontier.size(); i++)
      {
        int v = frontier[i];
        #pragma omp critical
        cout << v << " ";
        for (int n : adj[v]) 
        {
          if (!visited[n]) 
          {
            bool wasVisited;
            // Atomic check + set
            #pragma omp critical
            {
              wasVisited = visited[n];
              if (!visited[n])
                visited[n] = true;
            }
            if (!wasVisited)
              local_next.push_back(n);
          }
        }
      }
      // Merge local results safely
      #pragma omp critical
      next_frontier.insert(next_frontier.end(), local_next.begin(),
                             local_next.end());
    }
    frontier = next_frontier;
    }
  }

  void parallelDFS(int start) {
    vector<bool> visited(V, false);

#pragma omp parallel
    {
#pragma omp single
      dfsTask(start, visited);
    }
  }

  void dfsTask(int v, vector<bool> &visited) {
    bool alreadyVisited;

#pragma omp critical
    {
      alreadyVisited = visited[v];
      if (!visited[v]) {
        visited[v] = true;
        cout << v << " ";
      }
    }

    if (alreadyVisited)
      return;

    for (int n : adj[v]) {
#pragma omp task
      dfsTask(n, visited);
    }
  }
};

int main() {
  Graph g(7);

  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 3);
  g.addEdge(1, 4);
  g.addEdge(2, 5);
  g.addEdge(2, 6);

  cout << "Parallel BFS: ";
  g.parallelBFS(0);
  cout << endl;

  cout << "Parallel DFS: ";
  g.parallelDFS(0);
  cout << endl;

  return 0;

  /*
      0 -------->1
      |         / \
      |        /   \
      |       /     \
      v       v       v
      2 ----> 3       4
      |      |
      |      |
      v      v
      5      6
  */
}