#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>



// Struct representing a vertex


struct Vertex {
    int id;
    int type;
};

struct Node {
    int vertexId;
    float weigth;
    Node* next;
};



bool operator==(const Vertex& lhs, const Vertex& rhs);

// Struct representing a graph
struct Graph {
    int numVertices;
    // Vector of vertices
    std::vector<Vertex> vertices;

    // Vector of edges of each vertex
    std::vector<Node*> edges; // first node is the first adjacent vertex

    // Function to add a new vertex between two existing vertices with a weighted edge
    void addVertexBetween(Vertex existingVertex1, Vertex existingVertex2, Vertex newVertex, float proportion);

    // Function to delete a vertex and its associated edges
    void deleteVertexBetween(Vertex vertex);

    // Function to add a new edge to the graph
    void addEdge(Vertex from, Vertex to, float weight);

    // Function to add a new vertex to the graph
    void addVertex(int id, int type);

};
// Dijkstra's algorithm to find the n vertices of type "1" closest to the initial vertex
std::vector<Vertex> dijkstra(Vertex initialVertex, int n, Graph myGraph);