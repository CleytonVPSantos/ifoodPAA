#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include "graph.h"

bool compare(std::pair<double, int>, std::pair<double, int> );

std::vector<int> findNClosest(Vertex, Graph, int);

std::vector<std::vector<int>> pathsOfNClosest(Vertex, Graph, int);

std::vector<int> dijkstraWithGoal(Vertex, Vertex, Graph);

std::vector<int> mergePaths(std::vector<int>, std::vector<int>);

std::vector<int> simpleDelivery(Vertex, Vertex, Vertex, Graph);

std::vector<int> buildPath(Vertex, Graph, int*);

std::vector<int> mergePaths(std::vector<int>, std::vector<int>);

std::vector<std::tuple<int, int, std::vector<int>>> optmizedDelivery(Vertex, Graph, int);

std::vector<std::pair<std::vector<int>, float>> pathToEachCenter(Vertex, Graph);

int countDistributionCenters(Graph);