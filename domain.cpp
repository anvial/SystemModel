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
