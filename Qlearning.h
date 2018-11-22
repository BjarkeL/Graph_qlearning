#ifndef QLEARNING_H
#define QLEARNING_H

//Includes:
#include <vector>
#include <fstream>

//Defines:
#define ALPHA 0.2 //Step size / Learning rate
#define GAMMA 0.75 //Discount

struct action;

class Graph;

class State {
public:
    State();
    State(int stateNum);
    ~State();

    void addAction(State* next, float reward);
    action getAction(int a);
    int nOfActions();
    float getQval(int a);
    void setQval(int a, float q);
    void setVisited();
    void clearVisited();
    bool isVisited();
    int getStateNum();
    void setFullSindex(int si, std::vector<int>* fullState);
    void updateAllStates(); //Deprecated. It is really only necessary to update the single action with new states rather than all.
    void updateSingleState(int a);
    bool allVisited();

private:
    std::vector<action> actions;
    int stateNumber;
    int fullStateIndex;
    std::vector<int>* graphState;
};

struct action {
    State* next;
    float reward;
    std::vector<std::pair<std::vector<int>, float>> q_vector;
};

class Graph {
public:
    Graph();
    ~Graph();

    void addState(State* s);
    void addConnection(State* s1, State* s2, float reward);
    bool allVisited();
    void clearVisited();
    std::vector<State*> getAllStates();
    void updateState(int fullSindex);
    std::vector<int> getGraphState();

private:
    std::vector<State*> states;
    std::vector<int> graphState;
    int fullStateIndex = 0;
};

class Qlearning {
public:
    Qlearning();
    ~Qlearning();

    void run();
    void printQvals(std::ofstream& f, Graph& g);

    int getAction(State* s);
    int getMaxAction(State* s);
    State* getNextState(State* s, int a);
    float getReward(State* s, int a);
    void calcQval(State* s, int a);

private:
    bool randomOn = true;
    std::vector<float> delta;
};

#endif