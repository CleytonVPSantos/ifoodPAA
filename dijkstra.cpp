#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include "Graph.h"

const int INF = std::numeric_limits<int>::max();

std::vector<int> findNClosest(Vertex, Graph, int);
std::vector<int> dijkstraWithGoal(Vertex, Vertex, Graph);
std::vector<int> buildPath(Vertex, int*);
std::vector<int> mergePaths(std::vector<int>, std::vector<int>);

bool compare(std::pair<double, int> a, std::pair<double, int> b){
    if (a.first == -1) return true;
    if (b.first == -1) return false;
    if (a.first == b.first) return a.second > b.second;
    return a.first > b.first;
}


std::vector<int> findNClosest(Vertex initialVertex, Graph myGraph, int n)
{
    int numVertices = myGraph.numVertices; // numero de vertices no grafo (incluindo temporários)
    std::vector<int> deliveryman (n); // entregadores mais próximos
    int count = 0; // conta entregadores encontrados

    // inicialização do Dijkstra
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>,
                        decltype(&compare) > heap(compare);
    bool visited[numVertices];
    double distance[numVertices], cost;
    for(int i = 0; i < numVertices; i++){
        visited[i] = false;
        distance[i] = -1;
    }
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
                }
            }
            edges = edges->next;
        }
        visited[v1.id] = true;

        // checa se o vértice que checamos é entregador
        if(v1.type == 1){
            deliveryman[count] = v1.id;
            count++;
        }
    }   
    return deliveryman;
}



//#######################################################################



std::vector<int> dijkstraWithGoal(Vertex initialVertex, Vertex finalVertex, Graph myGraph){
    int n = myGraph.numTemporalVertices; // numero de vertices no grafo (incluindo temporários)

    // inicialização do Dijkstra
    std::priority_queue<std::pair<int, int> > heap;
    bool visited[n];
    int parent[n];
    int distance[n];  
    for(int i = 0; i < n; i++){
        visited[i] = 0;
        parent[i] = -1;
        distance[n] = INF;
    }
    parent[initialVertex.id] = initialVertex.id;
    distance[initialVertex.id] = 0;
    heap.push(std::make_pair(distance[initialVertex.id], initialVertex.id));

    // loop principal
    while(!heap.empty()){
        Vertex v1 = myGraph.vertices[heap.top().second];
        heap.pop();
        if(distance[v1.id] == INF) { break; }
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
    return buildPath(finalVertex, parent);
}


std::vector<int> buildPath(Vertex finalVertex, int parent[]){ 
    int i = finalVertex.id;
    std::vector<int> path;
    while(parent[i] != i)
    {
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


// Questao 3 primeira parte
std::vector<Vertex> dijkstra3(Vertex cliente, Graph myGraph, int k, int j){
    int cds[k]; // lista dos centros de distribução
    int n = myGraph.numTemporalVertices;
    int count = 0; // conta quantos CDs ja encontramos

    // inicialização Dijkstra
    std::priority_queue<std::pair<int, int> > heap;
    bool visited[n];
    int distance[n];
    int parent[n];  
    for(int i = 0; i < n; i++){
        visited[i] = 0;
        parent[i] = -1;
        distance[n] = INF;
    }
    parent[cliente.id] = cliente.id;
    distance[cliente.id] = 0;
    heap.push(std::make_pair(distance[cliente.id], cliente.id));

    // loop principal
    while(!heap.empty() && count < k){ // encerra quando passar por todos os CDs ou esgotar o grafo
        Vertex v1 = myGraph.vertices[heap.top().second];
        heap.pop();
        if(distance[v1.id] == INF) { break; }
        Node* fuckyeah = myGraph.edges[v1.id];
        while(fuckyeah){
            Vertex v2 = myGraph.vertices[fuckyeah->vertexId];
            if(!visited[v2.id]) {
                int cost = fuckyeah->weight;
                if(distance[v1.id] + cost < distance[v2.id]){
                    distance[v2.id] = distance[v1.id] + cost;
                    parent[v2.id] = v1.id;
                    heap.push(std::make_pair(distance[v2.id], v2.id));
                }
            }
            fuckyeah = fuckyeah->next;
        }
        visited[v1.id] = true;
        if(v1.type == 3){
            cds[count] = v1.id;
            count++;
        }
    }

    myGraph.addVertex(n, 10); // adiciona um vertice virtual
    for(int i = 0; i < k; i++)
    {
        myGraph.addEdge(myGraph.vertices[cds[i]], cliente, distance[cds[i]]);
    }

    std::vector<std::vector<int>> pathCD; // vetor com os caminhos minimos de cliente para cada CD
    for(int i = 0; i < k; i++)
    {
        pathCD.push_back(buildPath(myGraph.vertices[cds[i]], parent));
    }

    Vertex pseudo = myGraph.vertices[n];
    // encontra os j entregadores mais próximos do vértice virtual    
    count = 0;
    int resposta[j];
    while(!heap.empty()) heap.pop();
    bool new_visited[n+1];
    int new_distance[n+1];
    int new_parent[n+1];
    for(int i = 0; i < n+1; i++){
        new_visited[i] = 0;
        new_parent[i] = -1;
        new_distance[n] = INF;
    }
    distance[pseudo.id] = 0;
    heap.push(std::make_pair(distance[pseudo.id], pseudo.id));

    while(!heap.empty() && count < j){
        Vertex v1 = myGraph.vertices[heap.top().second];
        heap.pop();
        if(distance[v1.id] == INF) { break; }
        Node* fuckyeah = myGraph.edges[v1.id];
        while(fuckyeah){
            Vertex v2 = myGraph.vertices[fuckyeah->vertexId];
            if(!visited[v2.id]) {
                int cost = fuckyeah->weight;
                if(distance[v1.id] + cost < distance[v2.id]){
                    distance[v2.id] = distance[v1.id] + cost;
                    heap.push(std::make_pair(distance[v2.id], v2.id));
                }
            }
            fuckyeah = fuckyeah->next;
        }
        visited[v1.id] = true;
        if(v1.type == 1){
            resposta[count] = v1.id;
            count++;
        }
    }
    
    std::vector<std::vector<int>> pathJ; // vetor com os caminhos minimos de cliente para cada CD
    for(int i = 0; i < j; i++)
    {
        pathJ.push_back(buildPath(myGraph.vertices[resposta[i]], parent));
    }

    /*for(int i=0; i < j; i++)
    {
        int l;
        pathJ[j][1];
    }*/
    std::vector<Vertex> ret;
    return ret;

}

