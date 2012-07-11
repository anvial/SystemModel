#ifndef LINK_H
#define LINK_H

#include <iostream>

using namespace std;

class Link
{
    int id;
    int s_domain_id;
    int d_domain_id;
    float bandwidth;

public:
    Link();
    Link(int link_id, int sdid, int ddid, float b);

    pair<int,int> get_domain_pair();
    float get_bandwidth();
    int get_link_id();

    void get_link_info();
};

#endif // LINK_H
