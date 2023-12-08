#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include "Graph.h"

const int INF = std::numeric_limits<int>::max();


std::vector<std::vector<int>> pathsOfNClosest(Vertex initialVertex, Graph myGraph, int n){
    int numVertices = myGraph.numVertices; // numero de vertices no grafo (incluindo temporários)
    std::vector<std::vector<int>> deliveryman; // entregadores mais próximos
    int count = 0; // conta entregadores encontrados

    // inicialização do Dijkstra
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,  // min heap
                        std::greater<std::pair<float, int>> > heap;
    bool visited[numVertices];
    int parent[numVertices];
    float distance[numVertices];  
    for(int i = 0; i < numVertices; i++){
        visited[i] = false;
        parent[i] = -1;
        distance[i] = INF;
    }
    parent[initialVertex.id] = initialVertex.id;
    distance[initialVertex.id] = 0;
    heap.push(std::make_pair(distance[initialVertex.id], initialVertex.id));

    // the magic is done here
    while(!heap.empty() && count < n){
        Vertex v1 = myGraph.vertices[heap.top().second]; 
        heap.pop();
        if(visited[v1.id]) continue;
        if(distance[v1.id] == INF) break; 
        Node* edges = myGraph.edges[v1.id];
        while(edges){
            Vertex v2 = myGraph.vertices[edges->vertexId];
            if(!visited[v2.id]) {
                int cost = edges->weight;
                if(distance[v1.id] + cost < distance[v2.id]){
                    distance[v2.id] = distance[v1.id] + cost;
                    heap.push(std::make_pair(distance[v2.id], v2.id));
                }
            }
            edges = edges->next;
        }
        visited[v1.id] = true;

        // checa se o vértice que checamos é entregador
        if(v1.type == 1){
            deliveryman.push_back(buildPath(myGraph.vertices[v1.id], myGraph, parent));
            count++;
        }
    }   
    return deliveryman;
}



//#######################################################################



std::vector<int> dijkstraWithGoal(Vertex initialVertex, Vertex finalVertex, Graph myGraph){
    int numVertices = myGraph.numVertices; // numero de vertices no grafo (incluindo temporários)

    // inicialização do Dijkstra
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,  // min heap
                        std::greater<std::pair<float, int>> > heap;
    bool visited[numVertices];
    int parent[numVertices];
    float distance[numVertices];  
    for(int i = 0; i < numVertices; i++){
        visited[i] = 0;
        parent[i] = -1;
        distance[i] = INF;
    }
    parent[initialVertex.id] = initialVertex.id;
    distance[initialVertex.id] = 0;
    heap.push(std::make_pair(distance[initialVertex.id], initialVertex.id));

    // loop principal
    while(!heap.empty()){
        Vertex v1 = myGraph.vertices[heap.top().second];
        heap.pop();
        if(visited[v1.id]) continue;
        if(distance[v1.id] == INF) break; 
        Node* edges = myGraph.edges[v1.id];
        while(edges){
            Vertex v2 = myGraph.vertices[edges->vertexId];
            if(!visited[v2.id]) {
                int cost = edges->weight;
                if(distance[v1.id] + cost < distance[v2.id]){
                    distance[v2.id] = distance[v1.id] + cost;
                    parent[v2.id] = v1.id;
                    heap.push(std::make_pair(distance[v2.id], v2.id));
                }
            }
            edges = edges->next;
        }
        visited[v1.id] = true;

        // checa se chegamos no vértice final
        if(v1.id == finalVertex.id)
            break;
    }
    return buildPath(finalVertex, myGraph, parent);
}


std::vector<int> buildPath(Vertex finalVertex, Graph myGraph, int parent[]){ 
    int i = finalVertex.id;
    std::vector<int> path;
    while(parent[i] != i)
    {
        if(myGraph.vertices[i].type == 0)
            path.push_back(i);
        i = parent[i];
    }
    return path;
}


std::vector<int> mergePaths(std::vector<int> first, std::vector<int> second){
    for(int i = 0; i < second.size(); i++){
        first.push_back(second[i]);
    }
    return first;
}


std::vector<int> simpleDelivery(Vertex deliveryMan, Vertex store, Vertex costumer, Graph myGraph){
    return mergePaths(dijkstraWithGoal(deliveryMan, store, myGraph), dijkstraWithGoal(store, costumer, myGraph));
}


std::vector<std::tuple<int, int, std::vector<int>>> optmizedDelivery(Vertex costumer, Graph myGraph, int n){
    std::vector<std::tuple<int, int, std::vector<int>>> nOptimal;
    std::vector<std::pair<std::vector<int>, float>> pathsToCenters = pathToEachCenter(costumer, myGraph); 
    int numDCs = pathsToCenters.size();
    int numVertices = myGraph.numVertices;
    myGraph.addVertex(numVertices, 10);
    Vertex pseudocostumer = myGraph.vertices[numVertices];
    for(int i = 0; i < numDCs; i++)
    {
        myGraph.addEdge(myGraph.vertices[pathsToCenters[i].first[-1]], pseudocostumer, pathsToCenters[i].second);
    }   
    std::vector<std::vector<int>> nClosest = pathOfNClosest(pseudocostumer, myGraph, n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < numDCs; j++){
            if(pathsToCenters[j].first[-1] == nClosest[i][1])
            {
                std::vector<int> deliveryPath = nClosest[i];
                deliveryPath.erase(deliveryPath.begin());
                deliveryPath.erase(deliveryPath.begin());
                nOptimal.push_back(std::make_tuple(nClosest[i][0], pathsToCenters[j].first[-1], mergePaths(pathsToCenters[j].first, deliveryPath)));
                break;
            }
        }
    }                 
    return nOptimal;                
}


std::vector<std::pair<std::vector<int>, float>> pathToEachCenter(Vertex initialVertex, Graph myGraph){
    std::vector<std::pair<std::vector<int>, float>> paths;

    int numVertices = myGraph.numVertices; // numero de vertices no grafo (incluindo temporários)

    // inicialização do Dijkstra
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,  // min heap
                        std::greater<std::pair<float, int>> > heap;
    bool visited[numVertices];
    int parent[numVertices];
    float distance[numVertices];  
    for(int i = 0; i < numVertices; i++){
        visited[i] = 0;
        parent[i] = -1;
        distance[numVertices] = INF;
    }
    parent[initialVertex.id] = initialVertex.id;
    distance[initialVertex.id] = 0;
    heap.push(std::make_pair(distance[initialVertex.id], initialVertex.id));

    // loop principal
    while(!heap.empty()){
        Vertex v1 = myGraph.vertices[heap.top().second];
        heap.pop();
        if(visited[v1.id]) continue;
        if(distance[v1.id] == INF) break; 
        Node* edges = myGraph.edges[v1.id];
        while(edges){
            Vertex v2 = myGraph.vertices[edges->vertexId];
            if(!visited[v2.id]) {
                int cost = edges->weight;
                if(distance[v1.id] + cost < distance[v2.id]){
                    distance[v2.id] = distance[v1.id] + cost;
                    parent[v2.id] = v1.id;
                    heap.push(std::make_pair(distance[v2.id], v2.id));
                }
            }
            edges = edges->next;
        }
        visited[v1.id] = true;
        if(myGraph.vertices[v1.id].type == 4)
            paths.push_back(std::make_pair(buildPath(myGraph.vertices[v1.id], myGraph, parent), distance[v1.id]));
    }
    return paths;
} 


int countDistributionCenters(Graph myGraph){
    int numDCs = 0;
    int numVertices = myGraph.numVertices;
    for(int i = 0; i < numVertices; i++){
        if(myGraph.vertices[i].type == 4)
            numDCs++;
    }
    return numDCs;
}
