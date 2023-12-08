#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include "Graph.h"

const int INF = std::numeric_limits<int>::max();

std::vector<int> dijkstra(Vertex initialVertex, Graph myGraph, int k)
{
    int n = myGraph.numTemporalVertices;
    std::vector<int> resposta (k);
    int count = 0;
    std::priority_queue<std::pair<int, int> > heap;
    bool visited[n];
    int distance[n];  
    for(int i = 0; i < n; i++){
        visited[i] = 0;
        distance[n] = INF;
    }
    distance[initialVertex.id] = 0;
    heap.push(std::make_pair(distance[initialVertex.id], initialVertex.id));

    while(!heap.empty() && count < k){
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
    return resposta;
}



//#######################################################################



std::vector<Vertex> dijkstra2(Vertex initialVertex, Graph myGraph, Vertex finalVertex)
{
    int n = myGraph.numTemporalVertices;
    int count = 0;
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

    while(!heap.empty()){
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
        if(v1.id == finalVertex.id)
            break;
    }
    std::vector<Vertex> ret (1);
    return ret;
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

