#include "gcn.h"
#include <QDebug>
GCN::GCN()
{
    std::cout << "Created Global Network" << std::endl;
}

int GCN::add_domain(int domain_id, int thn, map<int, float> sv, map<int, int> iv, float tr)
{
    Domain* domain = new Domain(domain_id,thn,sv,iv,tr);
    this->domain_map.insert(pair<int, Domain*>(domain_id,domain));
    return 0;
}

int GCN::add_link(int link_id, int sdid, int ddid, float b)
{
    Link* link = new Link(link_id,sdid,ddid,b);
    this->link_map.insert(pair<int, Link*>(link_id,link));
    // add link to tick link state map
    tick_link_state_map.insert(pair<int,float>(link_id,0.0f));
    return 0;
}

int GCN::add_flow(int flow_id, int sd, int dd, string srf, string pct,vector<int> fp, vector<string> tc, bool mf)
{
    Flow* flow = new Flow(flow_id,sd,dd,srf,pct,fp,tc,mf);
    this->flow_map.insert(pair<int, Flow*>(flow_id,flow));
    return 0;
}

int GCN::add_malware(int malware_id, int is)
{
    Malware* malware = new Malware(malware_id,is);
    this->malware_map.insert(pair<int, Malware*>(malware_id,malware));
    return 0;
}

//int GCN::flow_streaming(int flow_id)
//{
//    Flow* flow;
//    vector<int> flow_path;
//    vector<int>::iterator flow_path_iter;

//    flow = flow_map[flow_id];
//    flow_path = flow->get_flow_path();


//    for(flow_path_iter = flow_path.begin(); flow_path_iter != flow_path.end(); ++flow_path_iter)
//    {
//        Domain* domain;

//        domain = domain_map[(*flow_path_iter)];
//        if((*flow_path_iter) == flow_path.front()) // first iteration
//        {
//            float start_rate = flow->start_rate_generator();
//            float overload = domain->add_load(start_rate);
//            if(overload > 0.0f)
//            {
//                flow->change_state((*flow_path_iter),start_rate - overload, overload, 0.0f);
//            }
//            else
//            {
//                flow->change_state((*flow_path_iter),start_rate,0.0f,0.0f);
//            }
//        }
//        else if((*flow_path_iter) == flow_path.back()) // last iteration
//        {
//            // TO DO;
//        }

//    }

//    return 0;
//}

// domains than flows
// THIS THE MOST FUCKING COMPLEX FUNCTION IN THIS MODEL
int GCN::flows_throught_domains_streaming()
{
    map<int, Domain*>::iterator domain_map_iter;

    for(domain_map_iter = domain_map.begin(); domain_map_iter != domain_map.end(); ++domain_map_iter)
    {
        Domain* domain;
        int domain_id = domain_map_iter->first;
        domain = domain_map_iter->second;

        map<int, Flow*>::iterator flow_map_iter;
        int domain_for_flow_type;
        for(flow_map_iter = flow_map.begin(); flow_map_iter != flow_map.end(); ++flow_map_iter)
        {
            Flow* flow;
            int flow_id = flow_map_iter->first;
            flow = flow_map_iter->second;
            domain_for_flow_type = flow->get_domain_type_for_this_flow(domain_id);

            if(domain_for_flow_type == 0) // start domain for this flow
            {
                float start_rate = flow->start_rate_generator();
                float overload = domain->add_load(flow_id, start_rate); // change domain load... overload may be NULL
                flow->change_state(domain_id,start_rate - overload,overload,0.0f); // change flow state at this domain
            }
            else if (domain_for_flow_type == 1) // transit domain for this flow
            {
                int prev_domain_id = flow->get_prev_domain_in_flow_path(domain_id);
                Link* link = this->get_link(prev_domain_id,domain_id);
                Domain* prev_domain = domain_map[prev_domain_id];
                float flow_rate_in_prev_domain = prev_domain->get_flow_res(flow_id); // see how muck ress this flow took in prev domain

                float transfer_rate; // next stage to calc transfer rate for this flow
                float link_bandwidth = link->get_bandwidth(); // general link bandwidth
                int link_id = link->get_link_id(); // link id
                float tick_link_bandwidth = tick_link_state_map[link_id]; // current tick link bandwidth
                if(flow_rate_in_prev_domain > (link_bandwidth - tick_link_bandwidth))
                    transfer_rate = link_bandwidth - tick_link_bandwidth; // we cant transfer more then link allowed
                else
                    transfer_rate = flow_rate_in_prev_domain;

                //change domains (curr and prev) load
                prev_domain->sub_load(flow_id,transfer_rate);
                domain->add_load(flow_id,transfer_rate);

                //change tick link state
                tick_link_state_map[link_id] += transfer_rate;
            }
            else if (domain_for_flow_type == 2) // last domain for this flow
            {

                int prev_domain_id = flow->get_prev_domain_in_flow_path(domain_id);
                Link* link = this->get_link(prev_domain_id,domain_id);

                Domain* prev_domain = domain_map[prev_domain_id];
                float flow_rate_in_prev_domain = prev_domain->get_flow_res(flow_id); // see how muck ress this flow took in prev domain

                float transfer_rate; // next stage to calc transfer rate for this flow
                float link_bandwidth = link->get_bandwidth(); // general link bandwidth
                int link_id = link->get_link_id(); // link id
                float tick_link_bandwidth = tick_link_state_map[link_id]; // current tick link bandwidth
                if(flow_rate_in_prev_domain > (link_bandwidth - tick_link_bandwidth))
                    transfer_rate = link_bandwidth - tick_link_bandwidth; // we cant transfer more then link allowed
                else
                    transfer_rate = flow_rate_in_prev_domain;

                //change domains (curr and prev) load
                prev_domain->sub_load(flow_id,transfer_rate); // NO ADD LOAD - becouse of consume

                //change tick link state
                tick_link_state_map[link_id] += transfer_rate;
            }
        }
    }

    return 0;
}

Link *GCN::get_link(int s_domain_id, int d_domain_id)
{
    map<int, Link*>::iterator link_map_iter;

    for(link_map_iter = link_map.begin(); link_map_iter != link_map.end(); ++link_map_iter)
    {
        Link* link;
        pair<int,int> domain_pair;
        link = link_map_iter->second;

        domain_pair = link->get_domain_pair();
        if((domain_pair.first == s_domain_id) &&
           (domain_pair.second == d_domain_id))
            return link;
    }
    return NULL;
}

float GCN::get_link_bandwidth(int s_domain_id, int d_domain_id)
{
    map<int, Link*>::iterator link_map_iter;

    for(link_map_iter = link_map.begin(); link_map_iter != link_map.end(); ++link_map_iter)
    {
        Link* link;
        pair<int,int> domain_pair;
        link = link_map_iter->second;

        domain_pair = link->get_domain_pair();
        if((domain_pair.first == s_domain_id) &&
           (domain_pair.second == d_domain_id))
            return link->get_bandwidth();
    }
    return 0.0f;

}

int GCN::get_link_id(int s_domain_id, int d_domain_id)
{
    map<int, Link*>::iterator link_map_iter;

    for(link_map_iter = link_map.begin(); link_map_iter != link_map.end(); ++link_map_iter)
    {
        Link* link;
        pair<int,int> domain_pair;
        link = link_map_iter->second;

        domain_pair = link->get_domain_pair();
        if((domain_pair.first == s_domain_id) &&
           (domain_pair.second == d_domain_id))
            return link->get_link_id();
    }
    return -1;
}

void GCN::log_domain_load_res(float time, int domain_id)
{
    Domain* domain;
    domain = domain_map[domain_id];
    cout << "TIME: " << time << " LOAD_RES = " << domain->get_load_res() << endl;
}



void GCN::show_links()
{
    map<int, Link*>::iterator link_map_iter;
    Link* link;
    cout << endl << "---Short info about links in Network " << endl;
    for(link_map_iter = link_map.begin(); link_map_iter != link_map.end(); ++link_map_iter)
    {
        link = link_map_iter->second;
        link->get_link_info();
    }
    cout << endl;
}

void GCN::show_domains()
{
    map<int, Domain*>::iterator domain_map_iter;
    Domain* domain;
    cout << endl << "---Short info about domains in Network " << endl;
    for(domain_map_iter = domain_map.begin(); domain_map_iter != domain_map.end(); ++domain_map_iter)
    {
        domain = domain_map_iter->second;
        domain->get_domain_info();
    }
    cout << endl;
}

void GCN::show_flows()
{
    map<int, Flow*>::iterator flow_map_iter;
    Flow* flow;
    cout << endl << "---Short info about flows in Network " << endl;
    for(flow_map_iter = flow_map.begin(); flow_map_iter != flow_map.end(); ++flow_map_iter)
    {
        flow = flow_map_iter->second;
        flow->get_flow_info();
    }
    cout << endl;
}

void GCN::show_malware()
{
    map<int, Malware*>::iterator malware_map_iter;
    Malware* malware;
    cout << endl << "---Short info about malwares in Network " << endl;
    for(malware_map_iter = malware_map.begin(); malware_map_iter != malware_map.end(); ++malware_map_iter)
    {
        malware = malware_map_iter->second;
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

int GCN::tick()
{
    // makw NULL all tick link states
    map<int,float>::iterator tick_link_state_map_iter;
    for(tick_link_state_map_iter = tick_link_state_map.begin(); tick_link_state_map_iter != tick_link_state_map.end(); ++tick_link_state_map_iter)
    {
        tick_link_state_map_iter->second = 0.0f;
    }

    this->flows_throught_domains_streaming();

    return 0;
}
