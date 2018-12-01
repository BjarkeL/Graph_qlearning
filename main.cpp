#include "graph.h"
#include "qlearning.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include "dynamicRegister.h"

int main() {

    //Make the graph first.
    Graph map;
    for (int i = 0; i < 10; i++) {
        map.addNode(new Node);
    }
    std::vector<Node*> nodes = map.getNodes();

    map.addConnection(nodes[0], nodes[8], -2);
    map.addConnection(nodes[1], nodes[8], -2);
    map.addConnection(nodes[2], nodes[8], -2);
    map.addConnection(nodes[3], nodes[8], -3);
    map.addConnection(nodes[9], nodes[8], -10);
    map.addConnection(nodes[5], nodes[9], -2);
    map.addConnection(nodes[6], nodes[9], -8);
    map.addConnection(nodes[5], nodes[4], -1);
    map.addConnection(nodes[5], nodes[7], -1);

    nodes[0]->addContent(8);
    nodes[1]->addContent(10);
    nodes[2]->addContent(6);
    nodes[3]->addContent(4);
    nodes[4]->addContent(4);
    nodes[5]->addContent(6);
    nodes[6]->addContent(2);
    nodes[7]->addContent(8);
/*
    map.addConnection(nodes[0], nodes[1], -1);
    map.addConnection(nodes[1], nodes[2], -1);
    map.addConnection(nodes[2], nodes[3], -1);
    map.addConnection(nodes[0], nodes[4], -1);
    map.addConnection(nodes[4], nodes[5], -1);
    map.addConnection(nodes[4], nodes[6], -1);
    map.addConnection(nodes[6], nodes[7], -1);
    map.addConnection(nodes[7], nodes[8], -1);
    map.addConnection(nodes[6], nodes[9], -1);

    //Add content into a few nodes:
    nodes[9]->addContent(7);
    nodes[8]->addContent(7);
*/
    //Then qlearning.
    Qlearning qTest(&map);

    qTest.run();


/*
    DynReg test;
    DynReg test2;

    for (int i = 0; i < 15; i++) {
        test.add(1);
    }
    for (int i = 0; i < 15; i++) {
        test2.add(1);
    }

    std::cout << "Size of register: " << test.getSize() << std::endl;

    test2.clear(14);

    for (int i = 0; i < 15; i++) {
        std::cout << "Line: " << std::setw(2) << i << ": a * " << test.read(i) << ", b * " << test2.read(i) << std::endl;
    }

    std::cout << "a equal b? " << bool(test == test2) << std::endl;
*/
    return 0;
}