#include "domain_fsm.h"

Domain_FSM::Domain_FSM()
{
}

pair<string, int> Domain_FSM::find_rule(set<string> flow_tags)
{
//    map< set<string> , pair<string, int> >::iterator transition_table_iter;
    set<string> diff_set;

    for(auto transition_table_iter = this->transition_table.begin(); transition_table_iter != this->transition_table.end(); ++transition_table_iter)
    {
        diff_set.erase(diff_set.begin(),diff_set.end());
        set_difference((*transition_table_iter).first.begin(),(*transition_table_iter).first.end(),
                       flow_tags.begin(), flow_tags.end(), inserter(diff_set,diff_set.begin()));

        if (diff_set.size() == 0) // this is needed rule
        {
            return (*transition_table_iter).second;
        }
    }


    return pair<string,int>("NONE",0); // there is NO such rule
}
