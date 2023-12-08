#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include "sort.h"



// Struct representing a vertex


struct Vertex {
    int id;
    int type;

    Vertex(){
        id = 0;
        type = 0;
    }
    Vertex(int _id, int _type){
        id = _id;
        type = _type;
    }
};

struct Node {
    int vertexId;
    double weight;
    Node* next;
};

// Struct representing a graph
struct Graph {
    int numVertices = 0;
    int numTemporalVertices = 0;
    // Vector of vertices
    std::vector<Vertex> vertices;

    // Vector of edges of each vertex
    std::vector<Node*> edges; // first node is the first adjacent vertex

    // Function to delete a vertex and its associated edges
    void deleteTemporalVertices();

    // Function to add a new edge to the graph
    void addEdge(Vertex from, Vertex to, double weight);

    // Function to add a new vertex to the graph
    void addVertex(int id, int type);

    void addTemporalVertices(std::vector<std::tuple<int, int, double, int>>* address);

};

#endif