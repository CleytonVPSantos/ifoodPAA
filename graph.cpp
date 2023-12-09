#include "graph.h"

using namespace std;

void Graph::deleteTemporalVertices() {
    // Iterate through each vertex in the graph
    for(int i = numVertices-1; i>=0; i--){
        // Iterate through each edge of the current vertex
        if(vertices[i].type != 0){
            edges.erase(edges.begin()+i);
        }
        else{
            for(unsigned int j = edges[i].size()-1; j >= 0; j--){
                // Check if the connected vertex is not of type 0 (not temporal)
                if(vertices[edges[i][j].vertexId].type != 0){
                    // If so, delete the edgeNode and update the vector of edges
                    edges[i].erase(edges[i].begin()+j);
                }
                else
                    break;
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

    edges[id1].push_back(Node{id2, weight}); // add the node "to" to the end of the list
    edges[id2].push_back(Node{id1, weight}); // add the node "from" to the end of the list
}

void Graph::addVertex(int id, int type) {
    // Create a new vertex with the given ID and type
    Vertex newVertex = {id, type};

    // Add the new vertex to the vertices vector
    vertices.push_back(newVertex);

    // Add empty vector of edges for the new vertex
    edges.push_back(vector<Node>());
    
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

    vector<Node> adjList = edges[idVertex1];
    for(unsigned int i = 0; i < adjList.size(); i++){
        if(adjList[i].vertexId == idVertex2){
            weight = adjList[i].weight;
            break;
        }
    }

    return weight;
}

void Graph::addTemporalVertices(vector<tuple<int, int, double, int>>* address) {
    // Sort the matrix by some criteria (not provided in the code)
    sortAddressList(address);

    // Vector to store the number of vertices in the same edge group
    vector<int> groups;
    groups.push_back(0);
    unsigned int numVerticesSameEdge = 0;
    bool isSameEdge = true;
    unsigned int i = 0;

    // Determine the beginning and end of each edge group
    while (i < (*address).size()) {
        isSameEdge = true;
        while (isSameEdge && numVerticesSameEdge < (*address).size()) {
            if (get<0>((*address)[i]) != get<0>((*address)[i + 1]) ||
                get<1>((*address)[i]) != get<1>((*address)[i + 1])) {
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
        double weight = findEdgeWeight(get<0>((*address)[j]), get<1>((*address)[j]));

        // Add a new vertex and edge for the first vertex in the edge group
        addVertex(numVertices, get<3>((*address)[j]));
        addEdge(vertices[get<0>((*address)[j])], vertices[numVertices - 1], weight * get<2>((*address)[j]));

        // Depending on the number of vertices in the edge group, add additional vertices and edges
        int groupSize = groups[i + 1] - groups[i];
        if (groupSize == 1) {
            addEdge(vertices[numVertices - 1], vertices[get<1>((*address)[j])], weight * (1 - get<2>((*address)[j])));
        } else if (groupSize == 2) {
            addVertex(numVertices, get<3>((*address)[j + 1]));
            addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (get<2>((*address)[j + 1]) - get<2>((*address)[j])));
            addEdge(vertices[get<1>((*address)[j + 1])], vertices[numVertices - 1], weight * (1 - get<2>((*address)[j + 1])));
        } else {
            for (int k = 1; k < groupSize - 1; k++) {
                addVertex(numVertices, get<3>((*address)[j + k]));
                addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (get<2>((*address)[j + k]) - get<2>((*address)[j + k - 1])));
            }
            addVertex(numVertices, get<3>((*address)[j + groupSize - 1]));
            addEdge(vertices[numVertices - 1], vertices[numVertices - 2], weight * (get<2>((*address)[j + groupSize - 1]) - get<2>((*address)[j + groupSize - 2])));
            addEdge(vertices[numVertices - 1], vertices[get<1>((*address)[j + groupSize - 1])], weight * (1 - get<2>((*address)[j + groupSize - 1])));
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

//     cout << "\nGraph:" << endl;
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
//                 cout << i << " --(" << myGraph.edges[i][j].weight << ")-- " << myGraph.edges[i][j].vertexId << endl;
//         }
//     }
//     // for (int i = 0; i < myGraph.numVertices; i++) {
//     //     Node* edgeNode = myGraph.edges[i];
//     //     while(edgeNode != NULL){
//     //         cout << i << " --(" << edgeNode->weight << ")-- " << edgeNode->vertexId << endl;
//     //         edgeNode = edgeNode->next;
//     //     }
//     // }

//     vector<tuple<int, int, double, int>> address = {{0, 1, 0.1, 1}, {0,1,0.4,1}, {0,1,0.8,1}, {2,1,0.2,1}};
//     myGraph.addTemporalVertices(&address);
    

//     cout << "\nGraph with tempo:" << endl;
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
//                 cout << i << " --(" << myGraph.edges[i][j].weight << ")-- " << myGraph.edges[i][j].vertexId << endl;
//         }
//     }
//     myGraph.deleteTemporalVertices();


//     cout << "\nGraph After:" << endl;
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
//                 cout << i << " --(" << myGraph.edges[i][j].weight << ")-- " << myGraph.edges[i][j].vertexId << endl;
//         }
//     }

//     return 0;
// }
