#include <iostream>
#include <vector>
#include <tuple>
#include "interface.h"
#include "Graph.h"
#include "create_graph.h"
#include "dijkstra.h"

int main(void){
    string filename = "test.txt";
    Graph G = *constructGraphFromFile(filename);
    int *closest = findNClosest();
    return 0;
}