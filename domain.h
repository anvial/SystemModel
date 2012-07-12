#ifndef DOMAIN_H
#define DOMAIN_H

#include <map>
#include <iostream>

#include "domain_fsm.h"
#include "flow.h"

using namespace std;

class Domain
{
    int id;
    int total_hosts_number;
    int active_hosts_number; // N(t)
    map<int,float> security_vector; // SV(t)
    map<int,int> infection_vector; // IV(t)
    float total_resoures;
    float load_resoures; // Res(t)
    Domain_FSM* d_fsm; // FPS

    bool domain_overload_state;
    map<int,float> res_between_flow_distribution; // sum of all == load_resourse ... this struct is need to know how much resources gave to concrete flow

    float infectivity_koef;

public:
    Domain();
    Domain(int domain_id, int thn, map<int,float> sv, map<int,int> iv, float tr);
    int initialize();
    int calc_new_inf_number(int malware_id, float t);
    int change_flow(Flow* flow);

    float add_load(int flow_id, float added_load); // return overload value
    float sub_load(int flow_id, float subtracted_load);

    float get_flow_res(int flow_id); // get resource used by flow with ID = flow_id
    float get_load_res();
    int get_inf_hosts(int malware_id);

    int change_inf_hosts_number(int malware_id, float transfer_rate, float copy_size, int infection_speed);


    void get_domain_info();

};

#endif // DOMAIN_H
