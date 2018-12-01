#ifndef QLEARNING_H
#define QLEARNING_H

#include "graph.h"
#include "dynamicRegister.h"
#include <vector>

#define DEFAULTQ 0
#define MAXSTEPS 20
#define MAXREWARD 43
#define ALPHA 0.1
#define GAMMA 0.9

typedef Node State;
typedef int Action;

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

    void run();

    void print();

private:
    Graph* map;
    std::vector<std::pair<DynReg, float>>* qValues;
    bool terminated;
    int steps;
    float foundReward = 0;
    float episodeCost = 0;
    bool randomOn = true;
};

#endif