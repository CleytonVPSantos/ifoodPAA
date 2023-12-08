#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include "Graph.h"

std::vector<int> dijkstra(Vertex initialVertex, Graph myGraph, int k);

std::vector<Vertex> dijkstra2(Vertex initialVertex, Graph myGraph, Vertex finalVertex);

std::vector<int> buildPath(Vertex finalVertex, int parent[]);

std::vector<Vertex> dijkstra3(Vertex cliente, Graph myGraph, int k, int j);