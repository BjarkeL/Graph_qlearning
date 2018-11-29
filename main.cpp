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

    map.addConnection(nodes[0], nodes[1], -1);
    map.addConnection(nodes[1], nodes[2], -1);
    map.addConnection(nodes[2], nodes[3], -1);
    map.addConnection(nodes[0], nodes[4], -1);
    map.addConnection(nodes[4], nodes[5], -1);
    map.addConnection(nodes[4], nodes[6], -1);
    map.addConnection(nodes[6], nodes[7], -1);
    map.addConnection(nodes[7], nodes[8], -1);
    map.addConnection(nodes[6], nodes[9], -1);

    //Then qlearning.
    Qlearning qTest(&map);

    qTest.run();



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

    return 0;
}