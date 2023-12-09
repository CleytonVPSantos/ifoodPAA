#include "graph.h"

void Graph::deleteTemporalVertices() {
    // Iterate through each vertex in the graph
    for(int i = 0; i < numVertices; i++){
        // Iterate through each edge of the current vertex
        for(unsigned int j = 0; j < edges[i].size(); j++){
            // Check if the connected vertex is not of type 0 (not temporal)
            if(vertices[edges[i][j].vertexId].type != 0){
                // If so, delete the edgeNode and update the vector of edges
                edges[i].erase(edges[i].begin()+j);
                j--;
            }
        }
    }

    // Cleanup: Remove any vertices at the end of the vertices vector with type 0
    int i = numVertices-1;
    while(vertices[i].type != 0){
        vertices.erase(vertices.begin()+i);
        i--;
        numTemporalVertices--;
        numVertices--;
    }
}

void Graph::addEdge(Vertex from, Vertex to, double weight) {
    // Extract the IDs of the 'from' and 'to' vertices
    int id1 = from.id;
    int id2 = to.id;

    edges[id1].push_back(Node{id2, weight}); // add the new node to the end of the list
    edges[id2].push_back(Node{id1, weight}); // add the new node to the end of the list
}

void Graph::addVertex(int id, int type) {
    // Create a new vertex with the given ID and type
    Vertex newVertex = {id, type};

    // Add the new vertex to the vertices vector
    vertices.push_back(newVertex);

    // Add empty vector of edges for the new vertex
    edges.push_back(std::vector<Node>());
    
    // Increment the total number of vertices
    numVertices++;

    // If the vertex is of a non-zero type, increment the number of temporal vertices
    if(type != 0){
        numTemporalVertices++;
    }
}

double Graph::findEdgeWeight(int idVertex1, int idVertex2){
    // Initialize weight to a default value (it will remain unchanged if the edge is not found)
    double weight = 0.0;

    std::vector<Node> adjList = edges[idVertex1];
    for(unsigned int i = 0; i < adjList.size(); i++){
        if(adjList[i].vertexId == idVertex2){
            weight = adjList[i].weight;
            break;
        }
    }

    return weight;
}

void Graph::addTemporalVertices(std::vector<std::tuple<int, int, double, int>>* address) {
    // Sort the matrix by some criteria (not provided in the code)
    sortAddressList(address);

    // Vector to store the number of vertices in the same edge group
    std::vector<int> groups;
    groups.push_back(0);
    unsigned int numVerticesSameEdge = 0;
    bool isSameEdge = true;
    unsigned int i = 0;

    // Determine the beginning and end of each edge group
    while (i < (*address).size()) {
        isSameEdge = true;
        while (isSameEdge && numVerticesSameEdge < (*address).size()) {
            if (std::get<0>((*address)[i]) != std::get<0>((*address)[i + 1]) ||
                std::get<1>((*address)[i]) != std::get<1>((*address)[i + 1])) {
                isSameEdge = false;
            }
            numVerticesSameEdge++;
            i++;
        }
        groups.push_back(numVerticesSameEdge);
    }

    // Process each edge group and add vertices and edges accordingly
    for (unsigned int i = 0; i < groups.size() - 1; i++) {
        int j = groups[i];
        double weight = findEdgeWeight(std::get<0>((*address)[j]), std::get<1>((*address)[j]));

        // Add a new vertex and edge based on the current edge group
        addVertex(numVertices, std::get<3>((*address)[j]));
        addEdge(vertices[std::get<0>((*address)[j])], vertices[numVertices - 1], weight * std::get<2>((*address)[j]));

        // Depending on the number of vertices in the edge group, add additional vertices and edges
        int groupSize = groups[i + 1] - groups[i];
        if (groupSize == 1) {
            addEdge(vertices[numVertices - 1], vertices[std::get<1>((*address)[j])], weight * (1 - std::get<2>((*address)[j])));
        } else if (groupSize == 2) {
            addVertex(numVertices, std::get<3>((*address)[j + 1]));
            addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (std::get<2>((*address)[j + 1]) - std::get<2>((*address)[j])));
            addEdge(vertices[std::get<1>((*address)[j + 1])], vertices[numVertices - 1], weight * (1 - std::get<2>((*address)[j + 1])));
        } else {
            for (int k = 1; k < groupSize - 1; k++) {
                addVertex(numVertices, std::get<3>((*address)[j + k]));
                addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (std::get<2>((*address)[j + k]) - std::get<2>((*address)[j + k - 1])));
            }
            addVertex(numVertices, std::get<3>((*address)[j + groupSize - 1]));
            addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (std::get<2>((*address)[j + groupSize - 1]) - std::get<2>((*address)[j + groupSize - 2])));
            addEdge(vertices[numVertices - 1], vertices[std::get<1>((*address)[j + groupSize - 1])], weight * (1 - std::get<2>((*address)[j + groupSize - 1])));
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

//     std::cout << "\nGraph:" << std::endl;
//     for (int i = 0; i < myGraph.numVertices; i++) {
//         for(int j = 0; j < myGraph.edges[i].size(); j++){
//             // check if edge was already printed
//             bool alreadyPrinted = false;
//             for(int k = 0; k < j; k++){
//                 if(myGraph.edges[i][j].vertexId == myGraph.edges[i][k].vertexId){
//                     alreadyPrinted = true;
//                 }
//             }
//             if(!alreadyPrinted)
//                 std::cout << i << " --(" << myGraph.edges[i][j].weight << ")-- " << myGraph.edges[i][j].vertexId << std::endl;
//         }
//     }
//     // for (int i = 0; i < myGraph.numVertices; i++) {
//     //     Node* edgeNode = myGraph.edges[i];
//     //     while(edgeNode != NULL){
//     //         std::cout << i << " --(" << edgeNode->weight << ")-- " << edgeNode->vertexId << std::endl;
//     //         edgeNode = edgeNode->next;
//     //     }
//     // }

//     std::vector<std::tuple<int, int, double, int>> address = {{0, 1, 0.1, 1}, {0,1,0.4,1}, {0,1,0.8,1}, {2,1,0.2,1}};
//     myGraph.addTemporalVertices(&address);
    

//     std::cout << "\nGraph with tempo:" << std::endl;
//     for (int i = 0; i < myGraph.numVertices; i++) {
//         for(int j = 0; j < myGraph.edges[i].size(); j++){
//             // check if edge was already printed
//             bool alreadyPrinted = false;
//             for(int k = 0; k < j; k++){
//                 if(myGraph.edges[i][j].vertexId == myGraph.edges[i][k].vertexId){
//                     alreadyPrinted = true;
//                 }
//             }
//             if(!alreadyPrinted)
//                 std::cout << i << " --(" << myGraph.edges[i][j].weight << ")-- " << myGraph.edges[i][j].vertexId << std::endl;
//         }
//     }
//     myGraph.deleteTemporalVertices();


//     std::cout << "\nGraph After:" << std::endl;
//     for (int i = 0; i < myGraph.numVertices; i++) {
//         for(int j = 0; j < myGraph.edges[i].size(); j++){
//             // check if edge was already printed
//             bool alreadyPrinted = false;
//             for(int k = 0; k < j; k++){
//                 if(myGraph.edges[i][j].vertexId == myGraph.edges[i][k].vertexId){
//                     alreadyPrinted = true;
//                 }
//             }
//             if(!alreadyPrinted)
//                 std::cout << i << " --(" << myGraph.edges[i][j].weight << ")-- " << myGraph.edges[i][j].vertexId << std::endl;
//         }
//     }

//     return 0;
// }
