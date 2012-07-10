#include "gcn.h"

GCN::GCN()
{
    std::cout << "Created Global Network" << std::endl;
}

int GCN::add_domain(int domain_id, int thn, map<int, float> sv, map<int, int> iv, float tr)
{
    Domain* domain = new Domain(domain_id,thn,sv,iv,tr);
    this->domain_vector.push_back(domain);
//    domain->get_domain_info();
    return 0;
}

int GCN::add_link(int s_domain_id, int d_domain_id, float bandwidth)
{
    this->connecivity_matrix.insert(pair<pair<int,int>,float>(pair<int,int>(s_domain_id,d_domain_id),
                                                              bandwidth));
    return 0;
}

int GCN::add_flow(int flow_id, int sd, int dd, string srf, string pct, vector<string> tc, bool mf)
{
    Flow* flow = new Flow(flow_id,sd,dd,srf,pct,tc,mf);
    this->flow_vector.push_back(flow);
//    flow->get_flow_info();
    return 0;
}

int GCN::add_malware(int malware_id, int is)
{
    Malware* malware = new Malware(malware_id,is);
    this->malware_vector.push_back(malware);
//    malware->get_malware_info();
    return 0;
}



void GCN::show_links()
{
    pair<int,int> domain_pair;
    float bandwidth;
    map<pair<int,int>,float>::iterator cm_iter;
    cout << endl <<"---Short info about connectivity " << endl;
    for(cm_iter = this->connecivity_matrix.begin(); cm_iter != this->connecivity_matrix.end(); ++cm_iter)
    {
        domain_pair = cm_iter->first;
        bandwidth = cm_iter->second;
        cout << "Domain id = " << domain_pair.first << " connect Domain id = " << domain_pair.second << " with bandwidth = " << bandwidth << endl;
    }
    cout << endl;
}

void GCN::show_domains()
{
    vector<Domain*>::iterator domain_vector_iter;
    Domain* domain;
    cout << endl << "---Short info about domains in Network " << endl;
    for(domain_vector_iter = domain_vector.begin(); domain_vector_iter != domain_vector.end(); ++domain_vector_iter)
    {
        domain = (*domain_vector_iter);
        domain->get_domain_info();
    }
    cout << endl;
}

void GCN::show_flows()
{
    vector<Flow*>::iterator flow_vector_iter;
    Flow* flow;
    cout << endl << "---Short info about flows in Network " << endl;
    for(flow_vector_iter = flow_vector.begin(); flow_vector_iter != flow_vector.end(); ++flow_vector_iter)
    {
        flow = (*flow_vector_iter);
        flow->get_flow_info();
    }
    cout << endl;
}

void GCN::show_malware()
{
    vector<Malware*>::iterator malware_vector_iter;
    Malware* malware;
    cout << endl << "---Short info about malwares in Network " << endl;
    for(malware_vector_iter = malware_vector.begin(); malware_vector_iter != malware_vector.end(); ++malware_vector_iter)
    {
        malware = (*malware_vector_iter);
        malware->get_malware_info();
    }
    cout << endl;
}

void GCN::show_all()
{
    cout << endl << "--- ALL INFO ABOUT NETWORK MODEL ---" << endl;
    this->show_domains();
    this->show_links();
    this->show_flows();
    this->show_malware();
}
