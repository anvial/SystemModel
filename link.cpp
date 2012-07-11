#include "link.h"

Link::Link()
{
}

Link::Link(int link_id, int sdid, int ddid, float b)
{
    this->id = link_id;
    this->s_domain_id = sdid;
    this->d_domain_id = ddid;
    this->bandwidth = b;
}

pair<int, int> Link::get_domain_pair()
{
    pair<int,int> domain_pair;

    domain_pair.first = this->s_domain_id;
    domain_pair.second = this->d_domain_id;

    return domain_pair;
}

float Link::get_bandwidth()
{
    return this->bandwidth;
}

int Link::get_link_id()
{
    return this->id;
}

void Link::get_link_info()
{
    cout << "---Short info about This Link " << endl;
    cout << "Link id                    = " << this->id << endl;
    cout << "Link source domain id      = " << this->s_domain_id << endl;
    cout << "Link destination domain id = " << this->d_domain_id << endl;
    cout << "Link bandwidth value       = " << this->bandwidth << endl;
}
