#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include "Graph.h"

bool compare(std::pair<double, int> a, std::pair<double, int> b);

std::vector<int> findNClosest(Vertex, Graph, int);

std::vector<int> dijkstraWithGoal(Vertex, Vertex, Graph);

std::vector<int> buildPath(Vertex, myGraph, int*);

std::vector<int> mergePaths(std::vector<int>, std::vector<int>);