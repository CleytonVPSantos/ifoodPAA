#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include "graph.h"
using namespace std;

struct Graph* newGraph(int numVertices);

struct Graph* constructGraphFromFile(const string& strFileName);

struct Graph* constructGraphFromUserInput();

vector<tuple<int, int, double, int>> constructAddressFromFile(const string& strFileName);

vector<tuple<int, int, double, int>> constructAddressFromUserInput();

vector<tuple<int, int, double, int>> addDCsFromFile(const string& strFileName);