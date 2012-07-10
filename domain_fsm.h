#ifndef DOMAIN_FSM_H
#define DOMAIN_FSM_H

#include <vector>
#include <map>
#include <string>
#include <iostream>

using namespace std;

class Domain_FSM
{
    int start_state_id;
    int curr_state_id;
    map<pair<int,int>,string> transition_table; // curr state id to new state id     if condition "string" is true

public:
    Domain_FSM();
    int add_transition(int s_id, int d_id, string cond);
};

#endif // DOMAIN_FSM_H
