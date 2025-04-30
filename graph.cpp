#include "graph.h"
#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

// constructor, empty graph
// directionalEdges defaults to true
Graph::Graph(bool directionalEdges) {
  this->directionalEdges = directionalEdges;
}

// destructor
Graph::~Graph() {
  // Clean up all vertices and edges
  adjacencyList.clear();
}

// @return total number of vertices
int Graph::verticesSize() const { return adjacencyList.size(); }

// @return total number of edges
int Graph::edgesSize() const {
  int edges = 0;
  for (const auto &vertex : adjacencyList) {
    edges += vertex.second.size();
  }
  return edges;
}

// @return number of edges from given vertex, -1 if vertex not found
int Graph::vertexDegree(const string &label) const {
  auto it = adjacencyList.find(label);
  if (it == adjacencyList.end()) {
    return -1; // Vertex not found
  }
  return it->second.size();
}

// @return true if vertex added, false if it already is in the graph
bool Graph::add(const string &label) {
  if (contains(label)) {
    return false; // Vertex already exists
  }
  adjacencyList[label] = {};
  return true;
}

/** return true if vertex already in graph */
bool Graph::contains(const string &label) const {
  return adjacencyList.find(label) != adjacencyList.end();
}

// @return string representing edges and weights, "" if vertex not found
// A-3->B, A-5->C should return B(3),C(5)
string Graph::getEdgesAsString(const string &label) const {
  auto it = adjacencyList.find(label);
  if (it == adjacencyList.end()) {
    return ""; // Vertex not found
  }
  stringstream result;
  for (const auto &neighbor : it->second) {
    result << neighbor.first << "(" << neighbor.second << "),";
  }
  string edges = result.str();
  if (!edges.empty()) {
    edges.pop_back(); // Remove the last comma
  }
  return edges;
}

// @return true if successfully connected
bool Graph::connect(const string &from, const string &to, int weight) {
  if (from == to) {
    return false; // No self-loops allowed
  }
  if (contains(from) && contains(to)) {
    // Check if the edge already exists (directed graph check)
    if (adjacencyList[from].find(to) != adjacencyList[from].end()) {
      return false; // Edge already exists
    }
    // For directed graph, we only add a single direction
    adjacencyList[from][to] = weight;
    if (!directionalEdges) {
      // For undirected graph, also add the reverse direction with the same
      // weight
      adjacencyList[to][from] = weight;
    }
    return true;
  }
  return false;
}

// Removes the edge between two vertices, also removes the reverse edge for
// undirected graphs.
bool Graph::disconnect(const string &from, const string &to) {
  auto it = adjacencyList.find(from);
  if (it == adjacencyList.end() || it->second.find(to) == it->second.end()) {
    return false; // Edge not found
  }
  it->second.erase(to); // Remove the edge
  if (!directionalEdges) {
    // For undirected graph, remove the reverse edge as well
    adjacencyList[to].erase(from);
  }
  return true;
}

// depth-first traversal starting from given startLabel
void Graph::dfs(const string &startLabel, void visit(const string &label)) {
  set<string> visited; // Track visited vertices
  function<void(const string &)> dfsVisit = [&](const string &vertex) {
    visited.insert(vertex); // Mark vertex as visited
    visit(vertex);          // Visit the vertex
    for (const auto &neighbor : adjacencyList[vertex]) {
      if (visited.find(neighbor.first) == visited.end()) {
        dfsVisit(neighbor.first); // Recurse to unvisited neighbors
      }
    }
  };

  if (contains(startLabel)) {
    dfsVisit(startLabel); // Start DFS traversal from startLabel
  }
}

// breadth-first traversal starting from startLabel
void Graph::bfs(const string &startLabel, void visit(const string &label)) {
  set<string> visited;
  queue<string> q;

  if (contains(startLabel)) {
    q.push(startLabel);
    visited.insert(startLabel);

    while (!q.empty()) {
      string current = q.front();
      q.pop();
      visit(current); // Visit the current vertex

      for (const auto &neighbor : adjacencyList[current]) {
        if (visited.find(neighbor.first) == visited.end()) {
          visited.insert(neighbor.first);
          q.push(neighbor.first);
        }
      }
    }
  }
}

// store the weights in a map
// store the previous label in a map
pair<map<string, int>, map<string, string>>
Graph::dijkstra(const string &startLabel) const {
  map<string, int> weights;
  map<string, string> previous;
  priority_queue<pair<int, string>, vector<pair<int, string>>,
                 greater<pair<int, string>>>
      pq;

  // Check if the startLabel exists in the adjacency list
  if (adjacencyList.find(startLabel) == adjacencyList.end()) {
    // Return empty maps if the start vertex is not in the graph
    return make_pair(weights, previous);
  }

  // Initialize all distances to infinity and previous vertices to empty
  for (const auto &vertex : adjacencyList) {
    weights[vertex.first] = INT_MAX; // Set all vertices to infinity
    previous[vertex.first] = "";     // No predecessor initially
  }

  // Set the distance to the start vertex to 0
  weights[startLabel] = 0;
  pq.push({0, startLabel}); // Push the start vertex with distance 0

  // Process the priority queue until it's empty
  while (!pq.empty()) {
    string current = pq.top().second;
    int dist = pq.top().first;
    pq.pop();

    // Skip if the distance is not the current shortest distance
    if (dist > weights[current]) {
      continue;
    }

    // Check if current vertex exists in adjacency list
    auto it = adjacencyList.find(current);
    if (it != adjacencyList.end()) {
      // Explore all the neighbors of the current vertex
      for (const auto &neighbor : it->second) {
        int weight = neighbor.second; // Get the edge weight
        int newDist = dist + weight;  // Calculate the new distance

        // If the new distance is smaller, update it
        if (newDist < weights[neighbor.first]) {
          weights[neighbor.first] = newDist;  // Update the shortest distance
          previous[neighbor.first] = current; // Update the previous vertex
          pq.push({newDist,
                   neighbor.first}); // Push the updated vertex to the queue
        }
      }
    } else {
      // Handle the case where the current vertex is not in the adjacency list
      cerr << "Vertex " << current << " not found in adjacency list." << endl;
      continue;
    }
  }

  // Remove unreachable vertices (those with distance INT_MAX) and start vertex
  // from the previous map
  for (auto it = weights.begin(); it != weights.end();) {
    if (it->second == INT_MAX || it->first == startLabel) {
      it = weights.erase(it); // Erase unreachable or start vertex
    } else {
      ++it;
    }
  }

  // Cleanup the `previous` map:
  // Remove entries from `previous` map where corresponding weights are
  // unreachable or the start vertex
  for (auto it = previous.begin(); it != previous.end();) {
    if (weights.find(it->first) == weights.end() ||
        weights[it->first] == INT_MAX || it->first == startLabel) {
      it = previous.erase(it); // Erase unreachable or start vertex
    } else {
      ++it;
    }
  }

  weights.erase(startLabel);
  previous.erase(startLabel);
  return make_pair(weights, previous); // Return the distance and previous maps
}

// minimum spanning tree using Prim's algorithm
int Graph::mstPrim(const string &startLabel,
                   void visit(const string &from, const string &to,
                              int weight)) const {
  if (adjacencyList.find(startLabel) == adjacencyList.end()) {
    return -1; // If the start vertex doesn't exist
  }

  map<string, int> minEdge;     // Minimum edge for each vertex
  map<string, string> previous; // Store the predecessor vertex for each vertex
  set<string> visited;          // Keep track of visited vertices
  priority_queue<pair<int, string>, vector<pair<int, string>>,
                 greater<pair<int, string>>>
      pq;
  int mstWeight = 0; // Total weight of the MST

  // Initialize all vertices with infinity distance (except the start vertex)
  for (const auto &vertex : adjacencyList) {
    minEdge[vertex.first] = INT_MAX;
  }

  // Start from the given vertex
  minEdge[startLabel] = 0;
  pq.push({0, startLabel});

  while (!pq.empty()) {
    string current = pq.top().second;
    int weight = pq.top().first;
    pq.pop();

    // Skip if the current vertex has already been visited
    if (visited.find(current) != visited.end()) {
      continue;
    }

    visited.insert(current); // Mark the vertex as visited
    mstWeight += weight;     // Add the weight of the current edge to MST weight

    // Call the visit function for the MST edge, except for the start vertex
    if (current != startLabel) {
      visit(previous[current], current, weight);
    }

    // Explore all the neighbors of the current vertex
    for (const auto &neighbor : adjacencyList.at(current)) {
      if (visited.find(neighbor.first) == visited.end() &&
          neighbor.second < minEdge[neighbor.first]) {
        minEdge[neighbor.first] =
            neighbor.second; // Update the minimum edge weight
        pq.push({neighbor.second,
                 neighbor.first}); // Add the neighbor to the priority queue
        previous[neighbor.first] = current; // Record the predecessor vertex
      }
    }
  }

  return mstWeight;
}

// minimum spanning tree using Kruskal's algorithm
int Graph::mstKruskal(void visit(const string &from, const string &to,
                                 int weight)) const {
  // Using union-find to avoid cycles
  vector<tuple<int, string, string>> edges; // <weight, from, to>

  for (const auto &vertex : adjacencyList) {
    for (const auto &neighbor : vertex.second) {
      edges.push_back({neighbor.second, vertex.first, neighbor.first});
    }
  }

  sort(edges.begin(), edges.end());

  int mstWeight = 0;
  map<string, string> parent;

  for (const auto &vertex : adjacencyList) {
    parent[vertex.first] = vertex.first;
  }

  function<string(const string &)> find = [&](const string &v) {
    if (parent[v] != v) {
      parent[v] = find(parent[v]);
    }
    return parent[v];
  };

  for (const auto &edge : edges) {
    int weight;
    string from;
    string to;
    tie(weight, from, to) = edge;

    if (find(from) != find(to)) {
      parent[find(from)] = to;
      mstWeight += weight;
      visit(from, to, weight);
    }
  }

  return mstWeight;
}

// read a text file and create the graph
bool Graph::readFile(const string &filename) {
  ifstream myfile(filename);
  if (!myfile.is_open()) {
    cerr << "Failed to open " << filename << endl;
    return false;
  }
  int edges = 0;
  int weight = 0;
  string fromVertex;
  string toVertex;
  myfile >> edges;
  for (int i = 0; i < edges; ++i) {
    myfile >> fromVertex >> toVertex >> weight;
    // Add vertices to the graph
    add(fromVertex);
    add(toVertex);
    connect(fromVertex, toVertex, weight);
  }
  myfile.close();
  return true;
}