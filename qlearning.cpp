#include "qlearning.h"
#include <random>
#include <iostream>

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
    std::random_device rd;
    std::mt19937 gen(rd());

    float maxQ = -std::numeric_limits<float>::max();
    std::vector<float> listOfTies;
    int maxQindex = 0;
    for (int i = 0; i < s->nOfEdges(); i++) {
        float _qVal = getQval(s, i);
        if (_qVal > maxQ) {
            maxQ = _qVal;
            listOfTies.clear(); //Clear the list of ties if a new value beats current max.
            maxQindex = i;
        } else if (_qVal == maxQ) {
            listOfTies.push_back(i);
        }
    }
    //If the list of tied is not empty the last max has not been beaten and tied with another.
    if (!listOfTies.empty()) {
        listOfTies.push_back(maxQindex);
        std::uniform_int_distribution<> breakTie(0, listOfTies.size()-1);
        return listOfTies[breakTie(gen)]; //Return one of the tied actions.
    } else
    return maxQindex;
}

State* Qlearning::getNextState(State* s, Action a) {
    return s->getNext(a);
}

float Qlearning::getReward(State* s, Action a) {
    float nodeContent = map->getContent(s->getNodeIndex());
    float actionCost = s->getCost(a);
    if (steps >= MAXSTEPS - 1) {
        terminated = true;
        return nodeContent + actionCost - 10;
    }
    if (foundReward >= MAXREWARD) {
        terminated = true;
        return nodeContent + actionCost + 10;
    }
    return nodeContent + actionCost;
}

void Qlearning::calcQval(State* s, Action a) {
    //Get the current q-value of the state.
    float currentQ = getQval(s, a);
    //Get the state to be updated before it empties.
    DynReg stateToUpdate = map->getState();
    //Get the reward for taking the action.
    float reward = getReward(s, a);
    //Get the resulting state of taking the action.
    State* nextState = getNextState(s, a);
    //Get the max action from the next state.
    Action maxNextAction = getMaxAction(nextState); //Must be without updating the state.
    //Get the max next q-value.
    float maxNextQ = getQval(nextState, maxNextAction);

    float qVal = currentQ + ALPHA * ( reward + GAMMA * maxNextQ - currentQ );

    //Find or add the state for the q-value to be updated.
    if (qValues[s->getEdgeNum(a)].empty())
        qValues[s->getEdgeNum(a)].push_back(std::make_pair(stateToUpdate, qVal));
    else {
        for (auto& q : qValues[s->getEdgeNum(a)]) {
            if (stateToUpdate == q.first) {
                q.second = qVal;
                break;
            }
        }
    }
}

float Qlearning::getQval(State* s, Action a) {
    if (qValues[s->getEdgeNum(a)].empty())
        return DEFAULTQ;
    for (auto& q : qValues[s->getEdgeNum(a)]) {
        if (map->getState() == q.first)
            return q.second;
    }
}

void Qlearning::setQval(State* s, Action a, float qVal) {
    if (qValues[s->getEdgeNum(a)].size() == 0)
        qValues[s->getEdgeNum(a)].push_back(std::make_pair(map->getState(), qVal));
    else {
        for (auto& q : qValues[s->getEdgeNum(a)]) {
            if (map->getState() == q.first) {
                q.second = qVal;
                break;
            }
        }
    }
}

void Qlearning::run() {
    int totalRuns = 200;

    for (int i = 1; i <= totalRuns; i++) {
        terminated = false;
        steps = 1;
        //Set an initial state: (Could easily make it random).
        State* currentState = map->getNodes()[0];

        //Run until an episode terminates.
        while (!terminated) {
            Action nextAction = getAction(currentState);

            calcQval(currentState, nextAction);

            currentState = getNextState(currentState, nextAction);

            steps++;
        }
        map->refill();
    }
}