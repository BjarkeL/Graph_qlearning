#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "dynamicRegister.h"

#define FULL 1
#define EMPTY 0

struct edge;

class Node {
public:
    Node();
    ~Node();

    void setIndex(int index);
    void addEdge(int* edgeNum, Node* n, float c);
    int nOfEdges();
    int getEdgeNum(int edgeIndex);

private:
    int nodeIndex;
    std::vector<edge> edges;
    bool empty;
};

struct edge {
    int edgeNumber;
    Node* next;
    float cost;
};

class Graph {
public:
    Graph();
    ~Graph();

    void addNode(Node* n);
    void addConnection(Node* n1, Node* n2, float cost);
    int getNofEdges();
    std::vector<Node*> getNodes();  //Should perhaps interact with nodes solely through Graph.
    DynReg getState();

private:
    int nOfNodes = 0;
    int nOfEdges = 0;
    std::vector<Node*> nodes;
    DynReg state;
};

#endif;