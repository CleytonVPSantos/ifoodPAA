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
    int numVertices = myGraph.numVertices; // numero de vertices no grafo (incluindo temporários)
    std::vector<int> deliveryman (n); // entregadores mais próximos
    int count = 0; // conta entregadores encontrados

    // inicialização do Dijkstra
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
        Node* edges = myGraph.edges[v1.id];
        while(edges){
            Vertex v2 = myGraph.vertices[edges->vertexId];
            if(!visited[v2.id]) {
                cost = edges->weight;
                if((distance[v1.id] + cost < distance[v2.id]) || distance[v2.id] == -1){
                    distance[v2.id] = distance[v1.id] + cost;
                    heap.push(std::make_pair(distance[v2.id], v2.id));
                    parent[v2.id] = v1.id; 
                }
            }
            edges = edges->next;
        }
        visited[v1.id] = true;

        // ações especiais que variam com a operação realizada
        if(operation == 0){
            // checa se o vértice que checamos é entregador
            if(v1.type == 1){
                deliveryman[count] = v1.id;
                count++;
            }
        }
        else if(operation == 1){
            // checa se chegamos no vértice final
            if(v1.id == finalVertex.id)
                break;
        }
        else if(operation == 2){
            // checa se é um centro de distribuição 
           if(myGraph.vertices[v1.id].type == 4)
                centers.push_back(v1.id); 
        }
    }   
    return std::make_tuple(deliveryman, parent, distance, centers);
}



std::vector<int> buildPath(Vertex finalVertex, Graph myGraph, std::vector<int> parent){ 
    int i = finalVertex.id;
    std::vector<int> reversePath;
    reversePath.push_back(i);
    i = parent[i];
    while(parent[i] != i)
    {
        if(myGraph.vertices[i].type == 0)
            reversePath.push_back(i);
        i = parent[i];
    }
    reversePath.push_back(i);
    int n = reversePath.size();
    std::vector<int> path (n);
    for(int i = 1; i <= n; i++){
        path[i-1] = reversePath[n-i];
    } 
    return path;
}



std::vector<int> mergePaths(std::vector<int> first, std::vector<int> second){
    for(int i = 0; i < second.size(); i++){
        first.push_back(second[i]);
    }
    return first;
}



std::vector<int> findNClosest(Vertex initialVertex, Graph myGraph, int n)
{
    return std::get<0> (dijkstra(initialVertex, initialVertex, myGraph, n, 0));
}



std::vector<int> simpleDelivery(Vertex deliveryman, Vertex store, Vertex costumer, Graph myGraph){
    std::vector<int> parentStore = buildPath(store, myGraph, std::get<1> (dijkstra(deliveryman, store, myGraph, 1, 1)));
    std::vector<int> parentCostumer = buildPath(costumer, myGraph, std::get<1> (dijkstra(store, costumer, myGraph, 1, 1)));
    parentCostumer.erase(parentCostumer.begin());
    
    return mergePaths(parentStore, parentCostumer);
}



std::vector<std::vector<int>> pathsOfNClosest(Vertex initialVertex, Graph myGraph, int n){

    std::vector<std::vector<int>> deliverymanPath; // entregadores mais próximos
    
    dijkstra_type dijkstraData = dijkstra(initialVertex, initialVertex, myGraph, n, 0);
    std::vector<int> deliveryman = std::get<0> (dijkstraData);
    std::vector<int> parent = std::get<1> (dijkstraData);

    for(int i = 0; i < deliveryman.size(); i++){
        deliverymanPath.push_back(buildPath(myGraph.vertices[deliveryman[i]], myGraph, parent));
    } 
    return deliverymanPath;
}



std::vector<std::pair<std::vector<int>, float>> pathToEachCenter(Vertex initialVertex, Graph myGraph){
    std::vector<std::pair<std::vector<int>, float>> paths;
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



std::vector<std::tuple<int, int, std::vector<int>>> optmizedDelivery(Vertex costumer, Graph myGraph, int n){
    std::vector<std::tuple<int, int, std::vector<int>>> nOptimal;
    int numVertices = myGraph.numVertices;
    // caminhos ótimos cliente-CD
    std::vector<std::pair<std::vector<int>, float>> pathsToCenters = pathToEachCenter(costumer, myGraph); 
    int numDCs = pathsToCenters.size();
    // cria um vertice "falso" para calcular os n entregadores mais proximos que passam por CD
    myGraph.addVertex(numVertices, 10);
    Vertex pseudocostumer = myGraph.vertices[numVertices];
    for(int i = 0; i < numDCs; i++)
    {
        myGraph.addEdge(myGraph.vertices[pathsToCenters[i].first[pathsToCenters[i].first.size()-1]], pseudocostumer, pathsToCenters[i].second);
    }   
    // entregadores mais proximos do cliente passando por um CD (caminhos CD-entregador)
    std::vector<std::vector<int>> nClosestPaths = pathsOfNClosest(pseudocostumer, myGraph, n); 

    // encontra por qual CD cada caminho para cada entregador passa e junta os caminhos cliente-CD, CD-entregador
    for(int i = 0; i < n; i++){
        for(int j = 0; j < numDCs; j++){
            if(pathsToCenters[j].first[pathsToCenters[j].first.size()-1] == nClosestPaths[i][1])
            {
                int deliveryman = nClosestPaths[i][0];
                int usedCenter = pathsToCenters[j].first[pathsToCenters[j].first.size()-1];
                nClosestPaths[i].erase(nClosestPaths[i].begin()); // remove o vértice falso do caminho
                nClosestPaths[i].erase(nClosestPaths[i].begin()); // remove o CD do caminho
                nOptimal.push_back(std::make_tuple(deliveryman, usedCenter, mergePaths(pathsToCenters[j].first, nClosestPaths[i])));
                break;
            }
        }
    }                 
    return nOptimal;                
}