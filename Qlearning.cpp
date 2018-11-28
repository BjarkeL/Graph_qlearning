#include "Qlearning.h"
#include <random>
#include <fstream>
#include <iostream>
#include <time.h>

/*** States ***/

State::State() {}

State::State(int stateNum, int stateS) {
    stateNumber = stateNum;
    stateSize = stateS;
}

State::~State() {}

void State::addAction(State* next, float reward) {
    action a = {.next = next, .reward = reward};
    actions.push_back(a);
}

action State::getAction(int a) {
    return actions[a];
}

int State::nOfActions() {
    return actions.size();
}

float State::getQval(int a) {
    for (const auto& q_vals : actions[a].q_vector) {
        if (q_vals.first == *graphState)
            return q_vals.second;
    }
    return 0;
}

void State::setQval(int a, float q) {
    for (auto& q_vals : actions[a].q_vector) {
        if (q_vals.first == *graphState)
            q_vals.second = q;
    }
}

void State::setVisited() {
    (*graphState)[fullStateIndex] = 1;
}

void State::clearVisited() {
    (*graphState)[fullStateIndex] = 0;
}

bool State::isVisited() {
    return (*graphState)[fullStateIndex];
}

int State::getStateNum() {
    return stateNumber;
}

void State::setFullSindex(int si, std::vector<int>* fullState) {
    fullStateIndex = si;
    graphState = fullState;
}

void State::updateAllStates() {
    bool present = false;
    for (auto& a : actions) {
        if (a.q_vector.empty())
            present = false;
        else {
            for (const auto& q : a.q_vector) {
                if (*graphState == q.first)
                    present = true;
            }
        }
        if (!present)
            a.q_vector.push_back(std::make_pair(*graphState, 0));
    }
}

void State::updateSingleState(int a) {
    bool present = false;
    if (actions[a].q_vector.empty())
        present = false;
    else {
        for (const auto& q : actions[a].q_vector) {
            if (*graphState == q.first)
                present = true;
        }
    }
    if (!present)
        actions[a].q_vector.push_back(std::make_pair(*graphState, 0));
}

bool State::allVisited() {
    for (const auto& s : *graphState) {
        if (!s)
            return false;
    }
    return true;
}

int State::getStateSize() {
    return stateSize;
}

bool State::getEmptyState() {
    return empty;
}

void State::setEmptyState(bool e) {
    empty = e;
}

/*** Graph ***/

Graph::Graph() {}

Graph::~Graph() {}

void Graph::addState(State* s) {
    states.push_back(s);
    graphState.push_back(0);
    s->setFullSindex(fullStateIndex++, &graphState);
}

void Graph::addConnection(State* s1, State* s2, float reward) {
    s1->addAction(s2, reward);
    s2->addAction(s1, reward);
}

bool Graph::allVisited() {
    for (const auto& s : graphState) {
        if (!s)
            return false;
    }
    return true;
}

void Graph::clearVisited() {
    for (int i = 0; i < graphState.size(); i++) {
        graphState[i] = 0;
    }
}

std::vector<State*> Graph::getAllStates() {
    return states;
}

void Graph::updateState(int fullSindex) {
    graphState[fullSindex] = 1;
}

std::vector<int> Graph::getGraphState() {
    return graphState;
}

/*** Qlearning ***/

Qlearning::Qlearning() {}

Qlearning::~Qlearning() {}

void Qlearning::run() {

    //State sizes:
    std::vector<int> sSize = {0, 0, 12, 0, 0, 15, 0, 0, 10, 10};

    //Set up the graph:
    Graph allStates;
    for (int i = 0; i < 10; i++) {
        allStates.addState(new State(i, sSize[i]));
    }

    std::vector<State*> allStates_v = allStates.getAllStates();
    allStates.addConnection(allStates_v[0], allStates_v[1], -2);
    allStates.addConnection(allStates_v[1], allStates_v[2], -2);
    allStates.addConnection(allStates_v[2], allStates_v[3], -2);
    allStates.addConnection(allStates_v[0], allStates_v[4], -2);
    allStates.addConnection(allStates_v[4], allStates_v[5], -2);
    allStates.addConnection(allStates_v[4], allStates_v[6], -2);
    allStates.addConnection(allStates_v[6], allStates_v[7], -2);
    allStates.addConnection(allStates_v[7], allStates_v[8], -2);
    allStates.addConnection(allStates_v[6], allStates_v[9], -2);

    std::ofstream details;
    details.open("details.txt");
    std::ofstream stats;
    stats.open("stats.txt");
    std::ofstream q_delta;
    q_delta.open("q_delta.txt");
    clock_t t1, t2;
    t1 = clock();
    int last_run_l = 0;
    int total_runs = 1000;

    for (int i = 0; i < total_runs; i++) {
        terminated = false;
        stepsTaken = 0;
        ballsFound = 0;
        totalReward = 0;

        if (i == total_runs - 10)
            randomOn = false;
        if (i == 0 || i > total_runs - 10)
            details << "Run: " << i+1 << "\n";
        //Set initial state:
        State* current_state = allStates_v[0];

        int run_length = 0;

        //Clear empty states:
        for (const auto& s : allStates_v)
            s->setEmptyState(false);

        //For each episode:
        while(!terminated) {
            //if (run_length > 50)
            //    break;

            //Set the current state to visited.
            current_state->setVisited();

            //Write the current state number to file.
            if (i == 0 || i > total_runs - 10)
                details << current_state->getStateNum() << " -> ";
            if (i == total_runs - 1)
                last_run_l++;

            run_length++;
            stepsTaken++;

            //Get the next action to do.
            int next_a = getAction(current_state);

            //Update the states in the following action to include the current state.
            current_state->updateSingleState(next_a);

            //Calculate the q-value for being in the current state and taken the given action.
            calcQval(current_state, next_a);


            //Set the current state to be the next state.
            current_state = getNextState(current_state, next_a);
        }
        stats << totalReward / run_length << std::endl;
        float qd_sum = 0;
        for (auto& qd : delta) {
            qd_sum += abs(qd);
        }
        delta.clear();
        q_delta << qd_sum / run_length << std::endl;

        if (i == 0 || i > total_runs - 10)
            details << "\n";
        //Clear visited:
        allStates.clearVisited();


        std::cout << ballsFound << std::endl;
    }
    t2 = clock();
    printQvals(details, allStates);
    details << "Length of the last run: " << last_run_l << "\n";
    details.close();
    stats.close();
    q_delta.close();
    std::cout << "Running time: " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << " s." << std::endl;
}

void Qlearning::printQvals(std::ofstream& f, Graph& g) {
    int total_q_vals = 0;
    f << "\n";
    for (const auto& s : g.getAllStates()) {
        f << "State: " << s->getStateNum() << "\n";
        for (int i = 0; i < s->nOfActions(); i++) {
            f << s->getAction(i).q_vector.size() << "\n";
            total_q_vals += s->getAction(i).q_vector.size();
        }
    }
    f << "\nTotal q values: " << total_q_vals << "\n\n";
    //Write out all q-values to file.
    //for (const auto& s : g.getAllStates()) {
    //    f << "State: " << s->getStateNum() << "\n";
    //    for (int i = 0; i < s->nOfActions(); i++) {
    //        f << "\nAction: " << i << "\n";
    //        for (const auto& a : s->getAction(i).q_vector) {
    //            f << "State: ";
    //            for (int j = 0; j < a.first.size(); j++) {
    //                f << a.first[j];
    //            }
    //            f << " q: " << a.second << "\n";
    //        }
    //    }
    //    f << "\n";
    //}
}

int Qlearning::getAction(State* s) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> epsilon(1, 100);
    std::uniform_int_distribution<> explore(0, s->nOfActions()-1);

    if (epsilon(gen) == 5 && randomOn)
        return explore(gen);
    else
        return getMaxAction(s);
}

int Qlearning::getMaxAction(State* s) {
    std::random_device rd;
    std::mt19937 gen(rd());

    float maxQ = -std::numeric_limits<float>::max();
    std::vector<float> listOfTies;
    int maxQindex = 0;
    for (int i = 0; i < s->nOfActions(); i++) {
        if (s->getQval(i) > maxQ) {
            maxQ = s->getQval(i);
            listOfTies.clear(); //Clear the list of ties if a new value beats current max.
            maxQindex = i;
        } else if (s->getQval(i) == maxQ) {
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

State* Qlearning::getNextState(State* s, int a) {
    return s->getAction(a).next;
}

float Qlearning::getReward(State* s, int a, int mean) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> balls(mean, 3);
    float temp = balls(gen);

    if (!s->getEmptyState()) {
        s->setEmptyState(true);
        ballsFound += temp;
        if (ballsFound >= 30) {
            terminated = true;
            return temp + 10 + s->getAction(a).reward;
        }
        return temp + s->getAction(a).reward;
    }
    
    if (stepsTaken > 10) {
        terminated = true;
        //return -10;
    }
    
    return s->getAction(a).reward;
}

void Qlearning::calcQval(State* s, int a) {
    //get the current q-value:
    float current_q = s->getQval(a);
    //Get the reward for taking the action:
    float reward = getReward(s, a, s->getStateSize());
    //Get the resulting state of being in the current state and taking the action:
    State* next_s = getNextState(s, a);

    //Set next state as visited to update which state it picks from.
    bool reset = false;
    if (!next_s->isVisited()) {
        reset = true;
        next_s->setVisited();
    }


    //Get the max q-value from the next state:
    int maxAction = getMaxAction(next_s);
    float max_next_q = next_s->getQval(maxAction);

    if (reset)
        next_s->clearVisited();


    //Calculate the new q-value and insert it into the state/action pair:
    float q_val = current_q + ALPHA * ( reward + GAMMA * max_next_q - current_q );
    delta.push_back(current_q - q_val);
    s->setQval(a, q_val);

    totalReward += reward;
}