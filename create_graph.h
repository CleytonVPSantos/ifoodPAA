#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include "Graph.h"
using namespace std;

struct Graph* newGraph(int numVertices);

struct Graph* constructGraphFromFile(const string& strFileName);

struct Graph* constructGraphFromUserInput();

vector<tuple<int, int, float, int>> constructAddressFromFile(const string& strFileName);

vector<tuple<int, int, float, int>> constructAddressFromUserInput();
