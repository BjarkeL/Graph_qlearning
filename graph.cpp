#include "graph.h"

/***** Node *****/

Node::Node() {}

Node::~Node() {}

void Node::setIndex(int index) {
    nodeIndex = index;
}

void Node::addEdge(int edgeNum, Node* n, float c) {
    edge e = {.edgeNumber = edgeNum, .next = n, .cost = c};
    edges.push_back(e);
}

int Node::nOfEdges() {
    return edges.size();
}

int Node::getEdgeNum(int edgeIndex) {
    return edges[edgeIndex].edgeNumber;
}

Node* Node::getNext(int edgeIndex) {
    return edges[edgeIndex].next;
}

float Node::getCost(int edgeIndex) {
    return edges[edgeIndex].cost;
}

int Node::getNodeIndex() {
    return nodeIndex;
}

void Node::addContent(float c) {
    content = c;
}

float Node::getContent() {
    return content;
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
    n1->addEdge(nOfEdges++, n2, cost);
    n2->addEdge(nOfEdges++, n1, cost);
}

int Graph::getNofEdges() {
    return nOfEdges;
}

int Graph::getNofNodes() {
    return nOfNodes;
}

std::vector<Node*> Graph::getNodes() {
    return nodes;
}

DynReg Graph::getState() {
    return state;
}

void Graph::setState(int index) {
    state.set(index);
}

void Graph::clearState(int index) {
    state.clear(index);
}

float Graph::getContent(int index) {
    if (state.read(index)) {
        state.clear(index);
        return nodes[index]->getContent();
    } else
        return 0;
}

void Graph::refill() {
    for (int i = 0; i < nOfNodes; i++) {
        if (!state.read(i))
            state.set(i);
    }
}

Node* Graph::addEntryNode(Node* entryPoint) {
    entry = new Node;
    entry->addEdge(entryPoint->getNodeIndex(), entryPoint, 0);
    return entry;
}

void Graph::buildGraph(std::vector<std::array<float, 3>> allNodes) {
    for (auto& a : allNodes)
        addConnection(nodes[a[0]], nodes[a[1]], a[2]);
}