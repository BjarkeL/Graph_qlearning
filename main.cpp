#include "graph.h"
#include "qlearning.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include "dynamicRegister.h"
#include <random>
#include <fstream>
#include <string>

#define RUNS 4
#define TOTALSTEPS 5000

void printToFile(std::array<std::vector<float>, RUNS> r, std::array<std::vector<float>, RUNS> c, std::array<std::vector<float>, RUNS> s, std::array<std::vector<std::vector<int>>, RUNS> R) {
    int runIndex = 1;
    std::ofstream reward;
    std::ofstream cost;
    std::ofstream steps;
    std::ofstream routes;
    reward.open("reward.txt");
    cost.open("cost.txt");
    steps.open("steps.txt");
    routes.open("routes.txt");

    for (int j = 0; j < TOTALSTEPS; j++) {
        for (int i = 0; i < RUNS; i++)
            reward << r[i][j] << ",";    
        reward << std::endl;
    }
    for (int j = 0; j < TOTALSTEPS; j++) {
        for (int i = 0; i < RUNS; i++)
            cost << c[i][j] << ",";    
        cost << std::endl;
    }
    for (int j = 0; j < TOTALSTEPS; j++) {
        for (int i = 0; i < RUNS; i++)
            steps << s[i][j] << ",";    
        steps << std::endl;
    }
    for (int i = 0; i < RUNS; i++) {
        routes << "Run " << runIndex++ << ":" << std::endl << std::endl;
        for (auto& route : R[i]) {
            for (auto& step : route) {
                routes << step << ",";
            }
            routes << std::endl << std::endl;
        }
    }

    reward.close();
    cost.close();
    steps.close();
    routes.close();
}

void run(Qlearning &q, std::vector<float> rewards, int startNode, float alpha, float gamma, int greed, int maxSteps, int maxReward, int totalSteps, int randomOff) {
    std::array<std::vector<float>, RUNS> rew;
    std::array<std::vector<float>, RUNS> cost;
    std::array<std::vector<float>, RUNS> steps;
    std::array<std::vector<std::vector<int>>, RUNS> routes;
    results testResult;

    for (int i = 0; i < RUNS; i++) {
        testResult = q.run(
            rewards,
            startNode,
            alpha,
            gamma,
            greed,
            maxSteps,
            maxReward,
            totalSteps,
            randomOff);

        for (auto& r : testResult.reward)
            rew[i].push_back(r);
        for (auto& c : testResult.cost)
            cost[i].push_back(c);
        for (auto& s : testResult.steps)
            steps[i].push_back(s);
        routes[i] = testResult.routes;

        greed += 5;
    }
        printToFile(rew, cost, steps, routes);
}

/*
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

    printToFile(test, "1");
}
*/

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

    std::vector<float> rewards = {7,10,1,0,7,5,0,0,15,10,5,7,3,1,7,15,0,8,4,10};

    int startNode = 13;
    float alpha = 0.1;
    float gamma = 0.9;
    int maxSteps = 25;
    int maxReward = 85;
    int totalSteps = TOTALSTEPS;
    int randomOff = 1000;
    int greed = 1;

    std::array<std::vector<float>, RUNS> rew;
    std::array<std::vector<float>, RUNS> cost;
    std::array<std::vector<float>, RUNS> steps;
    std::array<std::vector<std::vector<int>>, RUNS> routes;
    results testResult;

    for (int i = 0; i < RUNS; i++) {
        testResult = qTest.run(
            rewards,
            startNode,
            alpha,
            gamma,
            greed,
            maxSteps,
            maxReward,
            totalSteps,
            randomOff);

        for (auto& r : testResult.reward)
            rew[i].push_back(r);
        for (auto& c : testResult.cost)
            cost[i].push_back(c);
        for (auto& s : testResult.steps)
            steps[i].push_back(s);
        routes[i] = testResult.routes;

        greed += 5;
    }
        printToFile(rew, cost, steps, routes);
}

void testGraph3() {
    Graph map;
    for (int i = 0; i < 16; i++) {
        map.addNode(new Node);
    }

    std::ifstream file1("roomWeights.txt");
    
    std::vector<float> roomSizes;
    std::string line;
    float totalSize = 0;
    int test = 0;
    while (!file1.eof()) {
        std::getline(file1, line);
        if (line.size()) {
            roomSizes.push_back((float)std::stoi(line));
            totalSize += std::stoi(line);
        }
    }

    file1.close();
    std::ifstream file2("connections.txt");
    
    std::vector<std::array<int, 3>> connections;
    int n1, n2;
    while (!file2.eof()) {
        std::getline(file2, line);
        if (line.size()) {
            for(int i = 0; i < line.size(); i++) {
                if (line[i] == ' ') {
                    n1 = std::stoi(line.substr(0, i));
                    n2 = std::stoi(line.substr(i+1, line.size()-1));
                    connections.push_back({n1, n2, -2});
                    break;
                }
            }
        }
    }

    map.buildGraph(connections);

    int startNode = 0;
    float alpha = 0.1;
    float gamma = 0.9;
    int maxSteps = 20;
    int maxReward = totalSize*0.75;
    int totalSteps = TOTALSTEPS;
    int randomOff = 1000;
    int greed = 1;

    Qlearning q(&map);

    run(q, roomSizes, startNode, alpha, gamma, greed, maxSteps, maxReward, totalSteps, randomOff);
}

int main() {

    testGraph3();

    return 0;
}