#include "flow.h"


Flow::Flow()
{
}

Flow::Flow(int flow_id, int sd, int dd, string srf, string pct, vector<int> fp, set<string> tc, bool mf, int mid)
{
    this->id = flow_id;
    this->source_domain = sd;
    this->destination_domain = dd;
    this->start_rate_function = srf;
    this->path_calc_type = pct;
    this->flow_path = fp;
    this->tag_cloud = tc;
    this->malware_flag = mf;
    this->malware_id = mid;
}

int Flow::change_state(int d_id, float f_r, float f_l, float f_d)
{
    Flow_state_struct fss;
    fss.rate = f_r;
    fss.delay = f_d;
    fss.loss = f_l;

    flow_state[d_id] = fss;

    return 0;
}


void Flow::get_flow_info()
{
    set<string>::iterator tc_iter;
    vector<int>::iterator fp_iter;
    cout << "---Short info about This Flow " << endl;
    cout << "Flow id                       = " << this->id << endl;
    cout << "Flow source domain id         = " << this->source_domain << endl;
    cout << "Flow destination domain id    = " << this->destination_domain << endl;
    cout << "Flow start rate function name = " << this->start_rate_function <<  endl;
    cout << "Flow path calc type           = " << this->path_calc_type << endl;
    cout << "Flow has following tags       : ";
    for(tc_iter = this->tag_cloud.begin(); tc_iter != this->tag_cloud.end(); ++tc_iter)
    {
        cout << (*tc_iter) << " ";
    }
    cout << endl;
    cout << "Flow has following flow path  : ";
    for(fp_iter = this->flow_path.begin(); fp_iter != this->flow_path.end(); ++fp_iter)
    {
        cout << (*fp_iter) << " ";
    }
    cout << endl;
    cout << "Flow malware flag             = " << this->malware_flag << endl;
    cout << "Flow malware id               = " << this->malware_id << endl;
}


set<string> Flow::get_flow_tags()
{
    return this->tag_cloud;
}

vector<int> Flow::get_flow_path()
{
    return this->flow_path;
}

/**
 * @brief Flow::get_domain_type_for_this_flow
 * @param domain_id
 * @return 0 - if start domain
 *         1 - if transit domain
 *         2 - if end domain
 *        -1 - if NOT in this flow path domain
 */
int Flow::get_domain_type_for_this_flow(int domain_id)
{
    if(domain_id == flow_path.front())
        return 0;
    else if(domain_id == flow_path.back())
        return 2;
    else
    {
        vector<int>::iterator flow_path_iter;
        for(flow_path_iter = flow_path.begin(); flow_path_iter != flow_path.end(); ++flow_path_iter) // check if this domain is transit domain
        {
            if(domain_id == (*flow_path_iter))
                return 1;
        }
        return -1; // this domain is NOT in flow path domain
    }

}

int Flow::get_prev_domain_in_flow_path(int domain_id)
{
    vector<int>::iterator flow_path_iter;
    int prev_domain_id = -1;
    for(flow_path_iter = flow_path.begin(); flow_path_iter != flow_path.end(); ++flow_path_iter)
    {
        if((*flow_path_iter) == domain_id)
            return prev_domain_id;
        else
            prev_domain_id = (*flow_path_iter);
    }
    return prev_domain_id;
}

int Flow::get_flow_malware_id()
{
    return this->malware_id;
}

bool Flow::is_malware()
{
    return this->malware_flag;
}



float Flow::start_rate_generator()
{
    // FOR TEST PURPUSE ONLY! There const start rate generator
    return 1000.0;
}
