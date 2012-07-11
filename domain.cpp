#include "domain.h"



Domain::Domain()
{
}

Domain::Domain(int domain_id, int thn, map<int, float> sv, map<int, int> iv, float tr)
{
    this->id = domain_id;
    this->total_hosts_number = thn;
    this->active_hosts_number = thn;
    this->security_vector = sv;
    this->infection_vector = iv;
    this->total_resoures = tr;
    this->load_resoures = 0.0;

    this->domain_overload_state = false;
}


void Domain::get_domain_info()
{
    map<int,float>::iterator sv_itr;
    map<int,int>::iterator iv_itr;
    cout << "---Short info about This Domain " << endl;
    cout << "Domain id                   = " << this->id << endl;
    cout << "Domain total hosts number   = " << this->total_hosts_number << endl;
    cout << "Domain active hosts number  = " << this->active_hosts_number << endl;
    cout << "Domain Security Vector Info = " << endl;
    for (sv_itr = this->security_vector.begin(); sv_itr != this->security_vector.end(); ++sv_itr)
    {
        cout << "   malware id = " << sv_itr->first << " protection  = " << sv_itr->second << endl;
    }
    for (iv_itr = this->infection_vector.begin(); iv_itr != this->infection_vector.end(); ++iv_itr)
    {
        cout << "   malware id = " << iv_itr->first << " infected hosts = " << iv_itr->second << endl;
    }
    cout << "Domain total Resources      = " << this->total_resoures << endl;
}

/**
 * @brief Domain::add_load
 * @param added_load
 * @return overload value
 */
float Domain::add_load(int flow_id, float added_load)
{
    float overload_value = 0.0f;

    this->load_resoures += added_load;
    if(this->load_resoures > this->total_resoures)
    {
        this->domain_overload_state = true;
        overload_value = this->total_resoures - this->load_resoures;
        this->load_resoures = this->total_resoures;
    }

    res_between_flow_distribution[flow_id] += added_load - overload_value;
    return overload_value;
}

float Domain::sub_load(int flow_id, float subtracted_load)
{
    float minusload_value = 0.0f;

    this->load_resoures -= subtracted_load;
    if(this->load_resoures < 0.0f)
    {
        minusload_value = this->load_resoures * (-1.0f); // to get module of minus value
        this->load_resoures = 0.0f;
    }

    if(subtracted_load > 0.0f)
        this->domain_overload_state = false; // because some resource of domain - NOW free;

    res_between_flow_distribution[flow_id] -= subtracted_load - minusload_value;
    return minusload_value;
}

float Domain::get_flow_res(int flow_id)
{
    return res_between_flow_distribution[flow_id];
}

float Domain::get_load_res()
{
    return this->load_resoures;
}
