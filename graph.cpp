#include "graph.h"

/***** Node *****/

Node::Node() {}

Node::~Node() {}

void Node::setIndex(int index) {
    nodeIndex = index;
}

void Node::addEdge(int* edgeNum, Node* n, float c) {
    edge e = {.edgeNumber = *edgeNum++, .next = n, .cost = c};
    edges.push_back(e);
}

int Node::nOfEdges() {
    return edges.size();
}

int Node::getEdgeNum(int edgeIndex) {
    return edges[edgeIndex].edgeNumber;
}

/***** Graph *****/

Graph::Graph() {}

Graph::~Graph() {}

void Graph::addNode(Node* n) {
    nodes.push_back(n);
    state.add(FULL);
    n->setIndex(nOfNodes++);
}

void Graph::addConnection(Node* n1, Node* n2, float cost) {
    n1->addEdge(&nOfEdges, n2, cost);
    n2->addEdge(&nOfEdges, n1, cost);
}

int Graph::getNofEdges() {
    return nOfEdges;
}

DynReg Graph::getState() {
    return state;
}