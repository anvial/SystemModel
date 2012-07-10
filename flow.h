#ifndef FLOW_H
#define FLOW_H

#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class Flow
{
    int id;
    int source_domain;
    int destination_domain;
    string start_rate_function; // begin flow rate function name
    string path_calc_type;
    bool malware_flag;
    vector<string> tag_cloud; // tag cloud of this flow
    vector<int> flow_path; // sequence of domain ids, that flow needs to go
    int flow_path_size;
    float * begin_point_flow_rate; // pointer to begin flow rate function

    struct Flow_state_struct // flow state is calc in every domain it come throught
    {
        float rate;
        float loss;
        float delay;
    };

    map <int,Flow_state_struct> flow_state; // map flow parametrs on domain

public:
    Flow();
    Flow(int flow_id, int sd, int dd, string srf, string pct, vector<string> tc, bool mf);
    int change_state(int d_id, float f_r, float f_l, float f_d);
    int change_path(vector<int> new_flow_path); // TO DO
    vector<string> get_flow_tags();

    void get_flow_info();
};

#endif // FLOW_H