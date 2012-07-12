#ifndef GCN_H
#define GCN_H

#include <iostream>
#include <vector>
#include <map>

#include "domain.h"
#include "flow.h"
#include "malware.h"
#include "link.h"

using namespace std;

class GCN
{
    map <int, Link *> link_map;
    map <int, Domain *> domain_map;
    map <int, Flow *> flow_map;
    map <int, Malware *> malware_map;

    map <int, float> tick_link_state_map; // to obsrve every link state per one model time tick

public:
    GCN();
    int add_domain(int domain_id, int thn, map<int,float> sv, map<int,int> iv, float tr);
    int add_link(int link_id, int sdid, int ddid, float b);
    int add_flow(int flow_id, int sd, int dd, string srf, string pct, vector<int> fp, vector<string> tc, bool mf, int mid);
    int add_malware(int malware_id, int is, float cs);

//    int flow_streaming(int flow_id);
    int flows_throught_domains_streaming(float time);
    Link* get_link(int s_domain_id, int d_domain_id);
    float get_link_bandwidth(int s_domain_id, int d_domain_id);
    int get_link_id(int s_domain_id, int d_domain_id);

    void log_domain_load_res(float time, int domain_id);
    void log_domain_inf_hosts(float time, int domain_id, int malware_id);

    void show_links();
    void show_domains();
    void show_flows();
    void show_malware();

    void show_all();


    int tick(float time); // make one tick of modeling system
};

#endif // GCN_H
