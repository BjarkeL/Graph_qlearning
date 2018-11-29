#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "dynamicRegister.h"

struct edge;

class Node {
public:
    Node();
    ~Node();
private:
    int nodeIndex;
    std::vector<edge> edges;
    bool empty;
};

struct edge {
    Node* next;
};

class Graph {
public:
    Graph();
    ~Graph();

    void addNode(Node* n1);
    void addConnection(Node* n1, Node* n2, float cost);
private:
    std::vector<Node*> nodes;
};

#endif;