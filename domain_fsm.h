#ifndef DOMAIN_FSM_H
#define DOMAIN_FSM_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;

class Domain_FSM
{
    int start_state_id;
    int curr_state_id;
    map< set<string> , pair<string, int> > transition_table;

public:
    Domain_FSM();
    pair<string, int> find_rule(set<string> flow_tags);

};

#endif // DOMAIN_FSM_H
