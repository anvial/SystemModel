#ifndef GCN_H
#define GCN_H

#include <iostream>
#include <vector>
#include <map>

#include "domain.h"
#include "flow.h"
#include "malware.h"

using namespace std;

class GCN
{
    map <pair<int,int>,float> connecivity_matrix; // value of an element means - bandwidth of this link (id1 ---> id2)
    vector <Domain *> domain_vector;
    vector <Flow *> flow_vector;
    vector <Malware *> malware_vector;

    float time;

public:
    GCN();
    int add_domain(int domain_id, int thn, map<int,float> sv, map<int,int> iv, float tr);
    int add_link(int s_domain_id, int d_domain_id, float bandwidth);
    int add_flow(int flow_id, int sd, int dd, string srf, string pct, vector<string> tc, bool mf);
    int add_malware(int malware_id, int is);

    void show_links();
    void show_domains();
    void show_flows();
    void show_malware();

    void show_all();


    int tick(); // make one tick of modeling system
};

#endif // GCN_H
