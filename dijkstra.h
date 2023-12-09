#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include "graph.h"

bool compare(std::pair<double, int>, std::pair<double, int> );

using dijkstra_type = std::tuple<std::vector<int>, std::vector<int>, std::vector<double>, std::vector<int>>;

std::vector<int> findNClosest(Vertex, Graph, int);

dijkstra_type dijkstra(Vertex, Vertex, Graph, int, int);

std::vector<std::pair<std::vector<int>, int>> pathsOfNClosest(Vertex, Graph, int);

std::vector<int> mergePaths(std::vector<int>, std::vector<int>);

std::vector<int> simpleDelivery(Vertex, Vertex, Vertex, Graph);

std::vector<int> buildPath(Vertex, Graph, std::vector<int>);

std::vector<int> mergePaths(std::vector<int>, std::vector<int>);

std::vector<std::tuple<int, int, std::vector<int>>> optmizedDelivery(Vertex, Graph, int);

std::vector<std::pair<std::vector<int>, double>> pathToEachCenter(Vertex, Graph);

std::vector<int> invertVector(std::vector<int>);