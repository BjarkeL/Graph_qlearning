#ifndef QLEARNING_H
#define QLEARNING_H

#include "graph.h"
#include "dynamicRegister.h"
#include <vector>

#define DEFAULTQ 0

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

private:
    Graph* map;
    std::vector<std::pair<DynReg, float>>* qValues;
};

#endif;