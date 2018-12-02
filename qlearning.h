#ifndef QLEARNING_H
#define QLEARNING_H

#include "graph.h"
#include "dynamicRegister.h"
#include <vector>

#define DEFAULTQ 0
#define MAXSTEPS 25
#define MAXREWARD 70
#define ALPHA 0.1
#define GAMMA 0.9

typedef Node State;
typedef int Action;

struct results {
    std::vector<float> reward;
    std::vector<float> cost;
    std::vector<float> steps;
};

class Qlearning {
public:
    Qlearning(Graph* g);
    ~Qlearning();

    Action getAction(State* s);
    Action getMaxAction(State* s);
    State* getNextState(State* s, Action a);
    float getReward(State* s, Action a);
    void calcQval(State* s, Action a);
    float getQval(State* s, Action a);
    void setQval(State* s, Action a, float qVal, DynReg state);
    void randomizeRewards(std::vector<float> rewards, float deviation);

    results run(std::vector<float> rewards, int startNode, float a, float g, int maxS, int maxR, int totalR, int rOff);

    void print();

private:
    Graph* map;
    std::vector<std::pair<DynReg, float>>* qValues;
    bool terminated;
    int steps;
    float foundReward = 0;
    float episodeCost = 0;
    bool randomOn = true;
    float alpha = ALPHA;
    float gamma = GAMMA;
    int maxSteps = MAXSTEPS;
    int maxReward = MAXREWARD;
};

#endif