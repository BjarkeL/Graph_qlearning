#include "qlearning.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <fstream>

Qlearning::Qlearning(Graph* g) {
    qValues = new std::vector<std::pair<DynReg, float>>[g->getNofEdges()];
    map = g;
}

Qlearning::~Qlearning() {}

Action Qlearning::getAction(State* s) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> epsilon(1, 50);
    std::uniform_int_distribution<> explore(0, s->nOfEdges() - 1);

    if (epsilon(gen) == 5 && randomOn)
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
    State* nextState = getNextState(s, a);
    float nodeContent = map->getContent(nextState->getNodeIndex());
    float actionCost = s->getCost(a);
    episodeCost += actionCost;
    foundReward += nodeContent;
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
    DynReg stateToUpdate;
    for (int i = 0; i < map->getNofNodes(); i++) {
        stateToUpdate.add(map->getState().read(i));
    }

    //Get the reward for taking the action.
    float reward = getReward(s, a);
    //Get the resulting state of taking the action.
    State* nextState = getNextState(s, a);
    //Get the max action from the next state.
    Action maxNextAction = getMaxAction(nextState);
    //Get the max next q-value.
    float maxNextQ = getQval(nextState, maxNextAction);

    float qVal = currentQ + ALPHA * ( reward + GAMMA * maxNextQ - currentQ );

    //Find or add the state for the q-value to be updated.
    setQval(s, a, qVal, stateToUpdate);
}

float Qlearning::getQval(State* s, Action a) {
    if (!qValues[s->getEdgeNum(a)].empty()) {
        for (auto& q : qValues[s->getEdgeNum(a)]) {
            if (map->getState() == q.first)
                return q.second;
        }
    }
    return DEFAULTQ;
}

void Qlearning::setQval(State* s, Action a, float qVal, DynReg state) {
    bool notFound = true;
    if(!qValues[s->getEdgeNum(a)].empty()) {
        for (auto& q : qValues[s->getEdgeNum(a)]) {
            if (state == q.first) {
                q.second = qVal;
                notFound = false;
                break;
            }
        }
    }
    if (qValues[s->getEdgeNum(a)].empty() || notFound)
        qValues[s->getEdgeNum(a)].push_back(std::make_pair(state, qVal));
}

void Qlearning::run() {
    int totalRuns = 500;

    std::ofstream avgReward;
    avgReward.open("avgReward.txt");
    std::ofstream avgCost;
    avgCost.open("avgCost.txt");

    //Create a duplicate of the state wished to enter from.
    State* entryState = map->addEntryNode(map->getNodes()[9]);

    for (int i = 1; i <= totalRuns; i++) {
        terminated = false;
        steps = 0;
        foundReward = 0;
        episodeCost = 0;
        if (i == totalRuns - 10)
            randomOn = false;
        //Set an initial state.
        State* currentState = entryState;

        //Run until an episode terminates.
        while (!terminated) {
            Action nextAction = getAction(currentState);

            calcQval(currentState, nextAction);

            steps++;
            currentState = getNextState(currentState, nextAction);

            //std::cout << currentState->getNodeIndex() << " -> ";
        }
        //std::cout << std::endl;
        map->refill();
        avgReward << foundReward/steps << std::endl;
        avgCost << episodeCost/steps << std::endl;
    }
    avgReward.close();
    avgCost.close();
    //print();
}

void Qlearning::print() {
    int nQvals = 0;
    //std::cout << qValues[0].size() << std::endl;
    for (int i = 0; i < map->getNofEdges(); i++) {
        nQvals += qValues[i].size();
        //std::cout << "Edge " << i + 1 << std::endl;
        //for (auto& q : qValues[i]) {
        //    std::cout << std::setprecision(2) << q.second << ", ";
        //}
        //std::cout << std::endl;
    }
    std::cout << nQvals << std::endl;
}