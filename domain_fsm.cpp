#include "domain_fsm.h"

Domain_FSM::Domain_FSM()
{
}

int Domain_FSM::add_transition(int s_id, int d_id, string cond)
{
    pair<int,int> state_pair;
    state_pair.first = s_id;
    state_pair.second = d_id;

    this->transition_table.insert(pair<pair<int,int>,string>(state_pair,cond));

    return 0;
}
