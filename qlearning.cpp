#include "qlearning.h"
#include <random>

Qlearning::Qlearning(Graph* g) {
    qValues = new std::vector<std::pair<DynReg, float>>[g->getNofEdges()];
    map = g;
}

Qlearning::~Qlearning() {}

Action Qlearning::getAction(State* s) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> epsilon(1, 10);
    std::uniform_int_distribution<> explore(0, s->nOfEdges() - 1);

    if (epsilon(gen) == 5)
        return explore(gen);
    else
        return getMaxAction(s);
}

Action Qlearning::getMaxAction(State* s) {

}

State* Qlearning::getNextState(State* s, Action a) {

}

float Qlearning::getReward(State* s, Action a) {

}

void Qlearning::calcQval(State* s, Action a) {

}

float Qlearning::getQval(State* s, Action a) {
    if (qValues[s->getEdgeNum(a)].size() == 0)
        return DEFAULTQ;
    for (auto& q : qValues[s->getEdgeNum(a)]) {
        if (q.first == map->getState())
            return q.second;
    }
}