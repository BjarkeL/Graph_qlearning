#include "qlearning.h"
#include <random>
#include <iostream>
#include <iomanip>

Qlearning::Qlearning(Graph* g) {
    qValues = new std::vector<std::pair<DynReg, float>>[g->getNofEdges()];
    map = g;
}

Qlearning::~Qlearning() {}

Action Qlearning::getAction(State* s) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> epsilon(1, 100);
    std::uniform_int_distribution<> explore(0, s->nOfEdges() - 1);
    Action max = getMaxAction(s);

    if (epsilon(gen) <= greedP && randomOn && s->nOfEdges() > 1){
        while(true){
            Action sel = explore(gen);
            if(sel != max)
                return sel;
        }
    }
    else
        return max;
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
    float nodeContent = map->getContent(nextState->getNodeIndex()); // antal marbles
    float actionCost = s->getCost(a);
    episodeCost += actionCost;
    foundReward += nodeContent;
    if (steps >= maxSteps - 1) {
        terminated = true;
        return nodeContent + actionCost;
    }
    if (foundReward >= maxReward && terminateAtReward) {
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

    float qVal = currentQ + alpha * ( reward + gamma * maxNextQ - currentQ );

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

void Qlearning::randomizeRewards(std::vector<float> rewards, float deviation) {
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < rewards.size(); i++) {
        std::normal_distribution<float> R(rewards[i], deviation);
        map->getNodes()[i]->addContent(abs((int)R(gen)));
    }
}
void Qlearning::randomizeRewards2(std::vector<float> rewards) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> R(0, 1);
    
    float totalMarbles = 0;
    for (auto& s : rewards)
        totalMarbles += s;

    std::vector<float> cdf;
    float temp = 0;
    for (auto& s : rewards) {
        temp += s;
        cdf.push_back(temp/totalMarbles);
    }
    //std::cout << std::endl << temp << ", " << totalMarbles << std::endl;
    //for (auto& r : cdf)
    //    std::cout << r << ", ";

    float* rew = new float[rewards.size()];
    for(int i = 0; i < rewards.size(); i++)
        rew[i] = 0;

    for (int i = 1; i < totalMarbles; i++) {
        float select = R(gen);
        int n = 0;
        for (auto& r : cdf) {
            if (select < r) {
                rew[n]++;
                break;
            }
            n++;
        }
    }
    for (int i = 0; i < rewards.size(); i++) {
        map->getNodes()[i]->addContent(rew[i]);
        //std::cout << rew[i] << std::endl;
    }
    //std::cout << std::endl;
}

results Qlearning::run(std::vector<float> rewards, int startNode, float a, float g, int percentG, int maxS, float maxR, int totalR, int rOff) {
    randomOn = true;
    int totalRuns = totalR;
    alpha = a;
    gamma = g;
    maxSteps = maxS;
    maxReward = maxR;
    greedP = percentG;
    terminateAtReward = true;

    results result;

    //Create a duplicate of the state wished to enter from.
    State* entryState = map->addEntryNode(map->getNodes()[startNode]);

    for (int i = 1; i <= totalRuns; i++) {
        terminated = false;
        steps = 0;
        foundReward = 0;
        episodeCost = 0;
        route.clear();
        if (i == totalRuns - rOff + 1)
            randomOn = false;
        if (i > totalRuns - 10)
            terminateAtReward = false;
        //randomizeRewards(rewards, 1);
        randomizeRewards2(rewards);
        //std::cout << "Run " << i << std::endl;
        //for (int j = 0; j < map->getNofNodes(); j++)
        //    std::cout << "Node " << j << ", " << map->getNodes()[j]->getContent() << "\n";
        //Set an initial state.
        State* currentState = entryState;
        //Run until an episode terminates.
        while (!terminated) {
            Action nextAction = getAction(currentState);
            calcQval(currentState, nextAction);
            steps++;
            currentState = getNextState(currentState, nextAction);
            if (!terminateAtReward)
                route.push_back(currentState->getNodeIndex());
        }
        if (!route.empty())
            result.routes.push_back(route);
        map->refill();
        result.cost.push_back(episodeCost);
        result.reward.push_back(foundReward);
        result.steps.push_back(steps);
        //if (i == totalRuns - 10)
        //    std::cout << steps << std::endl;
    }
    print();
    for (int i = 0; i < map->getNofEdges(); i++) {
        qValues[i].clear();
    }
    return result;
}

void Qlearning::print() {
    int nQvals = 0;
    //std::cout << qValues[0].size() << std::endl;
    for (int i = 0; i < map->getNofEdges(); i++) {
        nQvals += qValues[i].size();
        //std::cout << "Edge " << i + 1 << std::endl;
        //for (auto& q : qValues[i]) {
        //    std::cout << std::setprecision(2) << q.second << ", ";
        //    for (int i = 0; i < map->getNofNodes(); i++)
        //        std::cout << q.first.read(i);
        //}
        //std::cout << std::endl;
    }
    std::cout << nQvals << std::endl;
}