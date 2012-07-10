#include "flow.h"

Flow::Flow()
{
}

Flow::Flow(int flow_id, int sd, int dd, string srf, string pct, vector<string> tc, bool mf)
{
    this->id = flow_id;
    this->source_domain = sd;
    this->destination_domain = dd;
    this->start_rate_function = srf;
    this->path_calc_type = pct;
    this->tag_cloud = tc;
    this->malware_flag = mf;
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
    vector<string>::iterator tc_iter;
    cout << "---Short info about This Flow " << endl;
    cout << "Flow id                       = " << this->id << endl;
    cout << "Flow source domain id         = " << this->source_domain << endl;
    cout << "Flow destination domain id    = " << this->destination_domain << endl;
    cout << "Flow start rate function name = " << this->start_rate_function <<  endl;
    cout << "Flow path calc type           = " << this->path_calc_type << endl;
    cout << "Flow has following tags: ";
    for(tc_iter = this->tag_cloud.begin(); tc_iter != this->tag_cloud.end(); ++tc_iter)
    {
        cout << (*tc_iter) << " ";
    }
    cout << endl;
    cout << "Flow malware flag             = " << this->malware_flag << endl;
}


vector<string> Flow::get_flow_tags()
{
    return this->tag_cloud;
}
