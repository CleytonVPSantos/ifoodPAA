#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include "dijkstra.h"

using dijkstra_type = std::tuple<std::vector<int>, std::vector<int>, std::vector<double>, std::vector<int>>;



bool compare(std::pair<double, int> a, std::pair<double, int> b){
    if (a.first == -1) return true;
    if (b.first == -1) return false;
    if (a.first == b.first) return a.second > b.second;
    return a.first > b.first;
}



dijkstra_type dijkstra(Vertex initialVertex, Vertex finalVertex, Graph myGraph, int n, int operation){
    int numVertices = myGraph.numVertices; // number of vertex (including temporary)
    std::vector<int> deliveryman (n); // closest deliverymen
    int count = 0; // counts deliverymen

    // Dijkstra initialization
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>,
                        decltype(&compare) > heap(compare);
    std::vector<int> parent (numVertices);
    bool visited[numVertices];
    std::vector<double> distance (numVertices);
    std::vector<int> centers;
    double cost;
    for(int i = 0; i < numVertices; i++){
        visited[i] = false;
        distance[i] = -1;
        parent[i] = -1;
    }
    parent[initialVertex.id] = initialVertex.id;
    distance[initialVertex.id] = 0;
    heap.push(std::make_pair(distance[initialVertex.id], initialVertex.id));

    // the magic is done here
    while(!heap.empty() && count < n){
        Vertex v1 = myGraph.vertices[heap.top().second]; 
        heap.pop();
        if (visited[v1.id]) continue;
        if(distance[v1.id] == -1) { break; }
        std::vector<Node> edges = myGraph.edges[v1.id];
        for (auto node: edges) {
            Vertex v2 = myGraph.vertices[node.vertexId];
            if(!visited[v2.id]) {
                cost = node.weight;
                if((distance[v1.id] + cost < distance[v2.id]) || distance[v2.id] == -1){
                    distance[v2.id] = distance[v1.id] + cost;
                    parent[v2.id] = v1.id; 
                    heap.push(std::make_pair(distance[v2.id], v2.id));
                }
            }
        }
        visited[v1.id] = true;

        // especial actions (depending on the operation - fifth argument)
        if(operation == 0){
            // checks if the vertex is a deliveryman
            if(v1.type == 1){
                deliveryman[count] = v1.id;
                count++;
            }
        }
        else if(operation == 1){
            // checks if we are in the final vertex
            if(v1.id == finalVertex.id)
                break;
        }
        else if(operation == 2){
            // checks if it is a distribution center
           if(myGraph.vertices[v1.id].type == 4)
                centers.push_back(v1.id); 
        }
    }   
    return std::make_tuple(deliveryman, parent, distance, centers);
}



// receives a vector and returns the vector backwards
std::vector<int> invertVector(std::vector<int> myVector){
    int n = myVector.size();
    std::vector<int> inverted (n);
    for(int i = 1; i <= n; i++){
        inverted[i-1] = myVector[n-i];
    }
    return inverted;
}



// receives a vertex, a graph and a vector of parents and returns the dijkstra's path to the given vertex
std::vector<int> buildPath(Vertex finalVertex, Graph myGraph, std::vector<int> parent){ 
    int i = finalVertex.id;
    std::vector<int> reversePath;
    reversePath.push_back(i);
    i = parent[i];
    while(parent[i] != i)
    {
        if(myGraph.vertices[i].type == 0) // ignores temporary vertex (deliverymen, DCs, stores)
            reversePath.push_back(i);
        i = parent[i];
    }
    reversePath.push_back(i);

    return invertVector(reversePath);
}



// receives two vectors and merge them 
std::vector<int> mergePaths(std::vector<int> first, std::vector<int> second){
    for(int i = 0; i < second.size(); i++){
        first.push_back(second[i]);
    }
    return first;
}



// solver Problem 1 - finds the n closest deliverymen to a given vertex (a store) 
std::vector<int> findNClosest(Vertex initialVertex, Graph myGraph, int n)
{
    return std::get<0> (dijkstra(initialVertex, initialVertex, myGraph, n, 0));
}



// solves Problem 2 - finds the shortest paths deliveryman-store, store-costumer and merges them
std::vector<int> simpleDelivery(Vertex deliveryman, Vertex store, Vertex costumer, Graph myGraph){
    std::vector<int> parentStore = buildPath(store, myGraph, std::get<1> (dijkstra(deliveryman, store, myGraph, 1, 1)));
    std::vector<int> parentCostumer = buildPath(costumer, myGraph, std::get<1> (dijkstra(store, costumer, myGraph, 1, 1)));
    parentCostumer.erase(parentCostumer.begin()); // the store's vertex is in both paths
    
    return mergePaths(parentStore, parentCostumer);
}



// finds the paths of the N closest deliverymen to a given vertex passing in a DC
// the DC will always be the second vertex in the path (this is used in Problem 3)
std::vector<std::pair<std::vector<int>, int>> pathsOfNClosest(Vertex initialVertex, Graph myGraph, int n){
    std::vector<std::pair<std::vector<int>, int>> deliverymanPath; // entregadores mais próximos
    dijkstra_type dijkstraData = dijkstra(initialVertex, initialVertex, myGraph, n, 0);
    std::vector<int> deliveryman = std::get<0> (dijkstraData);
    std::vector<int> parent = std::get<1> (dijkstraData);
    int DC; // to store the used DC
    for(int i = 0; i < deliveryman.size(); i++){
        // finds the used DC
        DC = deliveryman[i];
        while(parent[DC] != parent[parent[DC]])
            DC = parent[DC];

        deliverymanPath.push_back(std::make_pair(buildPath(myGraph.vertices[deliveryman[i]], myGraph, parent), DC));
    } 
    return deliverymanPath;
}



// finds the closest path to each DC given a vertex (deliveryman)
// returns the pair (path, path_length)
std::vector<std::pair<std::vector<int>, double>> pathToEachCenter(Vertex initialVertex, Graph myGraph){
    std::vector<std::pair<std::vector<int>, double>> paths;
    dijkstra_type dijkstraData = dijkstra(initialVertex, initialVertex, myGraph, 1, 2);
    std::vector<int> parent = std::get<1> (dijkstraData);
    std::vector<double> distance = std::get<2> (dijkstraData);
    std::vector<int> centers = std::get<3> (dijkstraData);
    int i;
    for(i = 0; i < centers.size(); i++){
        paths.push_back(std::make_pair(buildPath(myGraph.vertices[centers[i]], myGraph, parent), distance[centers[i]]));
    }
    
    return paths;
} 



// finds the n closest deliveryman (to a given costumer) (only paths passing in a DC are considered)
std::vector<std::tuple<int, int, std::vector<int>>> optmizedDelivery(Vertex costumer, Graph myGraph, int n){
    std::vector<std::tuple<int, int, std::vector<int>>> nOptimal;
    int numVertices = myGraph.numVertices;
    // DC-costumer optimal paths
    std::vector<std::pair<std::vector<int>, double>> pathsToCenters = pathToEachCenter(costumer, myGraph); 
    int numDCs = pathsToCenters.size();

    // creates a false vertex to calculate the n closest deliverymen (with paths passing in a DC)
    myGraph.addVertex(numVertices, 10);
    Vertex pseudocostumer = myGraph.vertices[numVertices];
    for(int i = 0; i < numDCs; i++)
    {
        myGraph.addEdge(myGraph.vertices[pathsToCenters[i].first[pathsToCenters[i].first.size()-1]], pseudocostumer, pathsToCenters[i].second);
    }   
 
    // closest deliverymen (paths passing in a DC) (DC-deliveryman paths)
    std::vector<std::pair<std::vector<int>, int>> nClosestPaths = pathsOfNClosest(pseudocostumer, myGraph, n); 

    // finds with DC each path uses and merges the paths costumer-DC, DC-deliveryman 
    for(int i = 0; i < n; i++){
        for(int j = 0; j < numDCs; j++){
            int size = pathsToCenters[j].first.size();
            if(pathsToCenters[j].first[size-1] == nClosestPaths[i].second)
            {
                int deliveryman = nClosestPaths[i].first[nClosestPaths[i].first.size()-1];
                int usedCenter = nClosestPaths[i].second;
                nClosestPaths[i].first.erase(nClosestPaths[i].first.begin()); // removes the false vertex from the path
                nClosestPaths[i].first.erase(nClosestPaths[i].first.begin()); // removes the DC from the path
                std::vector<int> reversedPath = mergePaths(pathsToCenters[j].first, nClosestPaths[i].first); // this is the inverted path
                nOptimal.push_back(std::make_tuple(deliveryman, usedCenter, invertVector(reversedPath)));
                break;
            }
        }
    }    
    return nOptimal;                
}

