#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "dynamicRegister.h"
#include <array>

#define FULL 1
#define EMPTY 0

struct edge;

class Node {
public:
    Node();
    ~Node();

    void setIndex(int index);
    void addEdge(int edgeNum, Node* n, float c);
    int nOfEdges();
    int getEdgeNum(int edgeIndex);
    Node* getNext(int edgeIndex);
    float getCost(int edgeIndex);
    int getNodeIndex();
    void addContent(float c);
    float getContent();

private:
    int nodeIndex;
    std::vector<edge> edges;
    float content = 0;
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
    int getNofNodes();
    std::vector<Node*> getNodes();  //Should perhaps interact with nodes solely through Graph.
    DynReg getState();
    void setState(int index);   //Probably not needed.
    void clearState(int index); //Probably not needed.
    float getContent(int index);
    void refill();
    Node* addEntryNode(Node* entryPoint);
    void buildGraph(std::vector<std::array<float, 3>> allNodes);

private:
    int nOfNodes = 0;
    int nOfEdges = 0;
    std::vector<Node*> nodes;
    DynReg state;
    Node* entry;
};

#endif