#include "Graph.h"
// Function to add a new vertex between two existing vertices with a weighted edge

bool operator==(const Vertex& lhs, const Vertex& rhs) {
    return lhs.id == rhs.id;
}

void Graph::addVertexBetween(Vertex existingVertex1, Vertex existingVertex2, Vertex newVertex, float proportion) {
    int id1 = existingVertex1.id;
    int id2 = existingVertex2.id;
    int newId = newVertex.id;
    int weight = 0;
    Node* edgeNode = edges[id1];
    while(edgeNode->next != NULL){
        if(edgeNode->vertexId == id2){
            weight = edgeNode->weigth;
        }
        edgeNode = edgeNode->next;
    }

    vertices.push_back(newVertex);

    Node* nextNode = edges[id1]->next;
    edges[id1]->next = new Node{newId, weight*proportion, nextNode};
    Node* nextNode2 = edges[id2]->next;
    edges[id2]->next = new Node{newId, weight*(1-proportion), nextNode2};

    edges.push_back(new Node{id1, weight*proportion, new Node{id2, weight*(1-proportion), NULL}});
    numVertices++;
}

// Function to delete a vertex and its associated edges
void Graph::deleteVertexBetween(Vertex vertex) {
    int id = vertex.id;
    Node* edgeNode = edges[id];
    while(edgeNode->next != NULL){
        int id2 = edgeNode->vertexId;
        Node* edgeNode2 = edges[id2];
        while(edgeNode2->next != NULL){
            if(edgeNode2->next->vertexId == id){
                edgeNode2->next = edgeNode2->next->next;
            }
            edgeNode2 = edgeNode2->next;
        }
        edgeNode = edgeNode->next;
    }
    edges.erase(edges.begin()+id);
    numVertices--;
}

void Graph::addEdge(Vertex from, Vertex to, float weight) {
    int id1 = from.id;
    int id2 = to.id;

    Node* edgeNode = edges[id1];
    if(edgeNode == NULL){
        edges[id1] = new Node{id2, weight, NULL};
        
    }
    else{
        while(edgeNode->next != NULL){
            edgeNode = edgeNode->next;
        }
        edgeNode->next = new Node{id2, weight, NULL};
    }
    Node* edgeNode2 = edges[id2];
    if(edgeNode2 == NULL){
        edges[id2] = new Node{id1, weight, NULL};
    }
    else{
        while(edgeNode2->next != NULL){
            edgeNode2 = edgeNode2->next;
        }
        edgeNode2->next = new Node{id1, weight, NULL};
    }
}

void Graph::addVertex(int id, int type) {
    Vertex newVertex = {id, type};
    vertices.push_back(newVertex);
    edges.push_back(NULL);
    
    numVertices++;
}

int main() {
    // Create a graph
    Graph myGraph;
    std::cout << "Graph created" << std::endl;
    // Add some vertices
    myGraph.addVertex(0, 0);
    myGraph.addVertex(1, 0);
    myGraph.addVertex(2, 0);

    std::cout << "Vertices added" << std::endl;

    // Add weighted edges
    myGraph.addEdge(myGraph.vertices[0], myGraph.vertices[1], 8);
    myGraph.addEdge(myGraph.vertices[1], myGraph.vertices[2], 5);
    myGraph.addEdge(myGraph.vertices[0], myGraph.vertices[2], 2);

    std::cout << "Edges added" << std::endl;

    // print the graph
    //add vertex between 0 and 1 with proportion 0.25
    myGraph.addVertexBetween(myGraph.vertices[0], myGraph.vertices[1], {3, 1}, 0.25);



    std::cout << "\nGraph:" << std::endl;
    for (int i = 0; i < myGraph.numVertices; i++) {
        Node* edgeNode = myGraph.edges[i];
        while(edgeNode != NULL){
            std::cout << i << " --(" << edgeNode->weigth << ")-- " << edgeNode->vertexId << std::endl;
            edgeNode = edgeNode->next;
        }
    }

    return 0;
}
