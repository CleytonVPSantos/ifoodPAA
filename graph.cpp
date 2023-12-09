#include "graph.h"

void Graph::deleteTemporalVertices() {
    // Iterate through each vertex in the graph
    for(int i = 0; i < numVertices; i++){
        // Get the first edge node of the current vertex
        std::vector<Node> adjList = edges[i];

        // Traverse the linked list of edges for the current vertex
        for(unsigned j = 0; j < adjList.size(); j++){
            // Check if the connected vertex is not of type 0 (not temporal)
            if(vertices[adjList[j].vertexId].type != 0){
                // If so, delete the edgeNode and update the linked list
                adjList.erase(adjList.begin()+j);
                // If the edgeNode is the first in the list, update edges[i]
                // if(edgeNode == edges[i]){
                //     edges[i] = edgeNode->next;
                // }
                // // If the edgeNode is not the first, update the linked list
                // else{
                //     Node* edgeNode2 = edges[i];
                //     while(edgeNode2->next != edgeNode){
                //         edgeNode2 = edgeNode2->next;
                //     }
                //     edgeNode2->next = edgeNode->next;
                // }
                // // Update tempNode, delete the current edgeNode, and move to the next
                // tempNode = edgeNode->next;
                // delete edgeNode;
                // edgeNode = tempNode;
            }
            // If the connected vertex is of type 0, move to the next edgeNode
            // else{
            //     edgeNode = edgeNode->next;
            // }
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
    edges[id2].push_back(Node{id2, weight}); // add the new node to the end of the list
    
    // Add an edge from 'from' to 'to'
    // Node* edgeNode = edges[id1];
    // if(edgeNode == NULL){
    //     // If there are no existing edges, create a new edgeNode
    //     edges[id1] = new Node{id2, weight, NULL};
    // }
    // else{
    //     // Traverse the linked list to the end and add a new edgeNode
    //     while(edgeNode->next != NULL){
    //         edgeNode = edgeNode->next;
    //     }
    //     edgeNode->next = new Node{id2, weight, NULL};
    // }

    // // Add an edge from 'to' to 'from' (since it's an undirected graph)
    // Node* edgeNode2 = edges[id2];
    // if(edgeNode2 == NULL){
    //     // If there are no existing edges, create a new edgeNode
    //     edges[id2] = new Node{id1, weight, NULL};
    // }
    // else{
    //     // Traverse the linked list to the end and add a new edgeNode
    //     while(edgeNode2->next != NULL){
    //         edgeNode2 = edgeNode2->next;
    //     }
    //     edgeNode2->next = new Node{id1, weight, NULL};
    // }
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

    // // Traverse the linked list of edges for the first vertex
    // Node* edgeNode = edges[idVertex1];
    // while(edgeNode != NULL){
    //     // Check if the current edgeNode corresponds to the second vertex
    //     if(edgeNode->vertexId == idVertex2){
    //         // If found, update the weight and break out of the loop
    //         weight = edgeNode->weight;
    //         break;
    //     }

    //     // Move to the next edgeNode in the linked list
    //     edgeNode = edgeNode->next;
    // }

    // // Return the weight of the edge (default value if the edge is not found)
    // return weight;
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

    // Determine the number of vertices in each edge group
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

    double weight = 0;
    // Process each edge group and add vertices and edges accordingly
    for (unsigned int i = 0; i < groups.size() - 1; i++) {
        int j = groups[i];
        weight = findEdgeWeight(std::get<0>((*address)[j]), std::get<1>((*address)[j]));

        // Add a new vertex and edge based on the current edge group
        addVertex(numVertices, std::get<3>((*address)[j]));
        addEdge(vertices[std::get<0>((*address)[j])], vertices[numVertices - 1], weight * std::get<2>((*address)[j]));

        // Depending on the number of vertices in the edge group, add additional vertices and edges
        if (groups[i + 1] - groups[i] < 2) {
            addEdge(vertices[numVertices - 1], vertices[std::get<1>((*address)[j])], weight * (1 - std::get<2>((*address)[j])));
        } else if (groups[i + 1] - groups[i] < 3) {
            addVertex(numVertices, std::get<3>((*address)[j + 1]));
            addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (std::get<2>((*address)[j + 1]) - std::get<2>((*address)[j])));
            addEdge(vertices[std::get<1>((*address)[j + 1])], vertices[numVertices - 1], weight * (1 - std::get<2>((*address)[j + 1])));
        } else {
            for (int k = 1; k < groups[i + 1] - groups[i] - 1; k++) {
                addVertex(numVertices, std::get<3>((*address)[j + k]));
                addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (std::get<2>((*address)[j + k]) - std::get<2>((*address)[j + k - 1])));
            }
            addVertex(numVertices, std::get<3>((*address)[j + groups[i + 1] - groups[i] - 1]));
            addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (std::get<2>((*address)[j + groups[i + 1] - groups[i] - 1]) - std::get<2>((*address)[j + groups[i + 1] - groups[i] - 2])));
            addEdge(vertices[numVertices - 1], vertices[std::get<1>((*address)[j + groups[i + 1] - groups[i] - 1])], weight * (1 - std::get<2>((*address)[j + groups[i + 1] - groups[i] - 1])));
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
