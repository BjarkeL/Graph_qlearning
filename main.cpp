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
#define PRINTEPISODES 10
#define DISPLAYEPISODES 1000
#define EXPLOREEPISODES 2000
#define TOTALEPISODES PRINTEPISODES + DISPLAYEPISODES + EXPLOREEPISODES
#define RANDOMOFF TOTALEPISODES - EXPLOREEPISODES
#define AVERAGES 10

void printToFile(std::array<std::array<float, TOTALEPISODES>, RUNS> r, std::array<std::array<float, TOTALEPISODES>, RUNS> c, std::array<std::array<float, TOTALEPISODES>, RUNS> s, std::array<std::vector<std::vector<int>>, RUNS> R) {
    int runIndex = 1;
    std::ofstream reward;
    std::ofstream cost;
    std::ofstream steps;
    std::ofstream routes;
    reward.open("reward.txt");
    cost.open("cost.txt");
    steps.open("steps.txt");
    routes.open("routes.txt");

    for (int j = 0; j < TOTALEPISODES; j++) {
        for (int i = 0; i < RUNS; i++)
            reward << r[i][j] << ",";    
        reward << std::endl;
    }
    for (int j = 0; j < TOTALEPISODES; j++) {
        for (int i = 0; i < RUNS; i++)
            cost << c[i][j] << ",";    
        cost << std::endl;
    }
    for (int j = 0; j < TOTALEPISODES; j++) {
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

void run(Qlearning &q, std::vector<float> rewards, int startNode, float alpha, float gamma, int greed, int maxSteps, float maxReward, int totalSteps, int randomOff) {
    std::array<std::array<float, TOTALEPISODES>, RUNS> rew;
    std::array<std::array<float, TOTALEPISODES>, RUNS> cost;
    std::array<std::array<float, TOTALEPISODES>, RUNS> steps;
    std::array<std::vector<std::vector<int>>, RUNS> routes;
    results testResult;

    for (int i = 0; i < RUNS; i++) {
        for (int j = 0; j < TOTALEPISODES; j++) {
            rew[i][j] = 0;
            cost[i][j] = 0;
            steps[i][j] = 0;
        }
    }

    for (int i = 0; i < RUNS; i++) {
        for (int j = 0; j < AVERAGES; j++) {

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

            for (int k = 0; k < TOTALEPISODES; k++) {
                rew[i][k] += testResult.reward[k]/AVERAGES;
                cost[i][k] += testResult.cost[k]/AVERAGES;
                steps[i][k] += testResult.steps[k]/AVERAGES;
            }
        }
        routes[i] = testResult.routes;

        greed += 5;
        //alpha += 0.2;
        //gamma -= 0.2;
    }
        printToFile(rew, cost, steps, routes);
}

void testGraph() {
    Graph map;
    
    std::ifstream file1("roomWeights.txt");
    
    std::vector<float> roomSizes;
    std::string line;
    float totalSize = 0;
    int test = 0;
    int numberOfNodes = 0;
    while (!file1.eof()) {
        std::getline(file1, line);
        if (line.size()) {
            numberOfNodes++;
            roomSizes.push_back(std::stof(line));
            totalSize += std::stof(line);
        }
    }

    for (int i = 0; i < numberOfNodes; i++) {
        map.addNode(new Node);
    }

    file1.close();
    std::ifstream file2("connections.txt");
    
    std::vector<std::array<float, 3>> connections;
    float n1, n2, n3;
    while (!file2.eof()) {
        int prevI = 0;
        std::getline(file2, line);
        if (line.size()) {
            for(int i = 0; i < line.size(); i++) {
                if (line[i] == ' ') {
                    if (!prevI) {
                        n1 = std::stoi(line.substr(0, i));
                        prevI = i;
                    } else {
                        n2 = std::stoi(line.substr(prevI+1, i));
                        n3 = std::stof(line.substr(i+1, line.size()-1));
                        connections.push_back({n1, n2, n3});
                        break;
                    }
                }
            }
        }
    }
    for (auto& c : connections) {
        std::cout << c[0] << ", " << c[1] << ", " << c[2] << std::endl;
    }
    for (auto& r : roomSizes) {
        std::cout << r << std::endl;
    }

    map.buildGraph(connections);

    int startNode = 9;
    float alpha = 0.1;
    float gamma = 0.9;
    int maxSteps = 25;
    float maxReward = totalSize*0.75;
    int totalSteps = TOTALEPISODES;
    int randomOff = RANDOMOFF;
    int greed = 0;

    Qlearning q(&map);

    run(q, roomSizes, startNode, alpha, gamma, greed, maxSteps, maxReward, totalSteps, randomOff);
}

int main() {

    std::ofstream specs;
    specs.open("specs.txt");
    specs << EXPLOREEPISODES << "," << DISPLAYEPISODES << "," << PRINTEPISODES;
    specs.close();

    testGraph();

    return 0;
}