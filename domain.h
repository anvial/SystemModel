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

    float time;


public:
    Domain();
    Domain(int domain_id, int thn, map<int,float> sv, map<int,int> iv, float tr);
    int initialize();
    int calc_new_inf_number(int malware_id, float t);
    int change_flow(Flow* flow);


    void get_domain_info();

};

#endif // DOMAIN_H
