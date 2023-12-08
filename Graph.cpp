#include "Graph.h"
// Function to add a new vertex between two existing vertices with a weighted edge

// Function to delete a vertex and its associated edges
void Graph::deleteTemporalVertices() {
    for(int i = 0; i < numVertices; i++){
        Node* edgeNode = edges[i];
        Node* tempNode = NULL;
        while(edgeNode != NULL){
            if(vertices[edgeNode->vertexId].type != 0){
                if(edgeNode == edges[i]){
                    edges[i] = edgeNode->next;
                }
                else{
                    Node* edgeNode2 = edges[i];
                    while(edgeNode2->next != edgeNode){
                        edgeNode2 = edgeNode2->next;
                    }
                    edgeNode2->next = edgeNode->next;
                }
                tempNode = edgeNode->next;
                delete edgeNode;
                edgeNode = tempNode;
            }
            else{
                edgeNode = edgeNode->next;
            }
        }
    }
    int i = numVertices - 1;
    while(vertices[i].type != 0){
        vertices.erase(vertices.begin()+i);
        i--;
        numTemporalVertices--;
        numVertices--;
    }
}

void Graph::addEdge(Vertex from, Vertex to, double weight) {
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
    if(type!=0){
        numTemporalVertices++;
    }
}

void Graph::addTemporalVertices(std::vector<std::tuple<int, int, double, int>> address) {
    // sorted matriz by
    sortAddressList(&address);
    std::vector<int> groups;
    groups.push_back(0);
    unsigned int numVerticesSameEdge = 0;
    bool isSameEdge = true;
    unsigned int i = 0;
    while(i < address.size()){
        isSameEdge = true;
        while(isSameEdge && numVerticesSameEdge < address.size()){
            if(std::get<0>(address[i]) != std::get<0>(address[i+1])){
                isSameEdge = false;
            }
            if(std::get<1>(address[i]) != std::get<1>(address[i+1])){
                isSameEdge = false;
            }
            numVerticesSameEdge++;
            i++;
        }
        groups.push_back(numVerticesSameEdge);
    }

    double weight = 0;
    for(unsigned int i = 0; i < groups.size() - 1; i++){
        int j = groups[i];
        Node* edgeNode = edges[std::get<0>(address[j])];
        while(edgeNode != NULL){
            if(edgeNode->vertexId == std::get<1>(address[j])){
                weight = edgeNode->weight;
                break;
            }
            edgeNode = edgeNode->next;
        }
        addVertex(numVertices, std::get<3>(address[j]));
        addEdge(vertices[std::get<0>(address[j])], vertices[numVertices-1], weight*std::get<2>(address[j]));

        if(groups[i+1]-groups[i]<2){
            addEdge(vertices[numVertices-1], vertices[std::get<1>(address[j])], weight*(1-std::get<2>(address[j])));
        }
        else if(groups[i+1]-groups[i]<3){
            addVertex(numVertices, std::get<3>(address[j+1]));
            addEdge(vertices[numVertices-1], vertices[numVertices-2], weight*(std::get<2>(address[j+1])-std::get<2>(address[j])));
            addEdge(vertices[std::get<1>(address[j+1])], vertices[numVertices-1], weight*(1-std::get<2>(address[j+1])));
        }
        else{
            for(int k = 1; k < groups[i+1]-groups[i]-1; k++){
                addVertex(numVertices, std::get<3>(address[j+k]));
                addEdge(vertices[numVertices-1], vertices[numVertices-2], weight*(std::get<2>(address[j+k])-std::get<2>(address[j+k-1])));
            }
            addVertex(numVertices, std::get<3>(address[j+groups[i+1]-groups[i]-1]));
            addEdge(vertices[numVertices-1], vertices[numVertices-2], weight*(std::get<2>(address[j+groups[i+1]-groups[i]-1])-std::get<2>(address[j+groups[i+1]-groups[i]-2])));
            addEdge(vertices[numVertices-1], vertices[std::get<1>(address[j+groups[i+1]-groups[i]-1])], weight*(1-std::get<2>(address[j+groups[i+1]-groups[i]-1])));
        }
    } 
}

// int main() {
//     // Create a graph
//     Graph myGraph;

//     // Add some vertices
//     myGraph.addVertex(0, 0);
//     myGraph.addVertex(1, 0);
//     myGraph.addVertex(2, 0);

//     // Add weighted edges
//     myGraph.addEdge(myGraph.vertices[0], myGraph.vertices[1], 10);
//     myGraph.addEdge(myGraph.vertices[1], myGraph.vertices[2], 5);
//     myGraph.addEdge(myGraph.vertices[0], myGraph.vertices[2], 2);

//     std::vector<std::tuple<int, int, float, int>> address = {{0, 1, 0.1, 1}, {0,1,0.4,1}, {0,1,0.8,1}, {2,1,0.2,1}};
//     myGraph.addTemporalVertices(address);
    

//     std::cout << "\nGraph:" << std::endl;
//     for (int i = 0; i < myGraph.numVertices; i++) {
//         Node* edgeNode = myGraph.edges[i];
//         while(edgeNode != NULL){
//             std::cout << i << " --(" << edgeNode->weight << ")-- " << edgeNode->vertexId << std::endl;
//             edgeNode = edgeNode->next;
//         }
//     }

//     myGraph.deleteTemporalVertices();


//     std::cout << "\nGraph After:" << std::endl;
//     for (int i = 0; i < myGraph.numVertices; i++) {
//         Node* edgeNode = myGraph.edges[i];
//         while(edgeNode != NULL){
//             std::cout << i << " --(" << edgeNode->weight << ")-- " << edgeNode->vertexId << std::endl;
//             edgeNode = edgeNode->next;
//         }
//     }

//     return 0;
// }
