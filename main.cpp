#include "graph.h"
#include "qlearning.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include "dynamicRegister.h"
#include <random>
#include <fstream>

void printToFile(results& input) {
    std::ofstream reward;
    std::ofstream cost;
    std::ofstream steps;
    reward.open("reward.txt");
    cost.open("cost.txt");
    steps.open("steps.txt");

    for (const auto& r : input.reward)
        reward << r << std::endl;
    for (const auto& c : input.cost)
        cost << c << std::endl;
    for (const auto& s : input.steps)
        steps << s << std::endl;

    reward.close();
    cost.close();
    steps.close();
}

void testGraph1() {
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
    
    //Then qlearning.
    Qlearning qTest(&map);

    std::vector<float> rewards = {8, 10, 6, 4, 4, 6, 2, 8};

    results test = qTest.run(rewards, 9, 0.1, 0.9, 15, 40, 500, 100);

    printToFile(test);
}

void testGraph2() {
    //Make the graph first.
    Graph map;
    for (int i = 0; i < 20; i++) {
        map.addNode(new Node);
    }
    std::vector<Node*> nodes = map.getNodes();

    float factor = 1.5;

    map.addConnection(nodes[0], nodes[1], -3 * factor);
    map.addConnection(nodes[1], nodes[2], -3 * factor);
    map.addConnection(nodes[2], nodes[3], -2 * factor);
    map.addConnection(nodes[3], nodes[4], -5 * factor);
    map.addConnection(nodes[4], nodes[5], -4 * factor);
    map.addConnection(nodes[5], nodes[6], -2 * factor);
    map.addConnection(nodes[6], nodes[7], -1 * factor);
    map.addConnection(nodes[7], nodes[8], -1 * factor);
    map.addConnection(nodes[4], nodes[9], -2 * factor);
    map.addConnection(nodes[9], nodes[10], -1 * factor);
    map.addConnection(nodes[10], nodes[14], -4 * factor);
    map.addConnection(nodes[2], nodes[11], -2 * factor);
    map.addConnection(nodes[11], nodes[12], -1 * factor);
    map.addConnection(nodes[12], nodes[13], -2 * factor);
    map.addConnection(nodes[13], nodes[14], -5 * factor);
    map.addConnection(nodes[14], nodes[15], -2 * factor);
    map.addConnection(nodes[15], nodes[16], -1 * factor);
    map.addConnection(nodes[11], nodes[17], -3 * factor);
    map.addConnection(nodes[17], nodes[13], -3 * factor);
    map.addConnection(nodes[13], nodes[18], -5 * factor);
    map.addConnection(nodes[18], nodes[19], -1 * factor);
    map.addConnection(nodes[19], nodes[10], -2 * factor);
    
    //Then qlearning.
    Qlearning qTest(&map);

    std::vector<float> rewards = {3,10,1,0,7,5,0,0,15,10,5,7,3,1,7,15,0,8,4,12};

    results test = qTest.run(rewards, 13, 0.1, 0.9, 20, 70, 1000, 100);
    
    printToFile(test);
}

int main() {

    testGraph2();

    return 0;
}