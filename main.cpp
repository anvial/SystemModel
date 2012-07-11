#include <iostream>
#include <QtXml/QtXml>
#include "gcn.h"
#include <string>

using namespace std;

const float time_step = 0.01f;
const float time_fin = 0.01f;

float time_curr;

struct domain_data_struct
{
    int id;
    int total_hosts_number;
    int active_hosts_number; // N(t)
    map<int,float> security_vector; // SV(t)
    map<int,int> infection_vector; // IV(t)
    float total_resoures;
    float load_resoures; // Res(t)
    Domain_FSM *d_fsm; // FPS
};

struct flow_data_struct
{
    int id;
    int source_domain;
    int destination_domain;
    string start_rate_function;
    string path_calc_type;
    vector<int> flow_path;
    vector<string> tag_cloud;
    bool malware_flag;
};

struct malware_data_struct
{
    int id;
    int infection_speed;
};

struct link_data_struct
{
    int id;
    int s_domain_id;
    int d_domain_id;
    float bandwidth;
};

map<int,float> traverse_sv_node(const QDomNode& node)
{
    map<int,float> sv_map;
    QDomNode svNode = node.firstChild();
    while(!svNode.isNull())
    {
        if(svNode.isElement())
        {
            QDomElement svElement = svNode.toElement();
            if(!svElement.isNull())
            {
                if(svElement.tagName() == "SV_elem")
                {
                    sv_map.insert(pair<int,float>(svElement.attribute("malware_id","").toInt(),
                                                  svElement.text().toFloat()));
                }
            }
        }
    svNode = svNode.nextSibling();
    }
    return sv_map;
}

map<int,int> traverse_iv_node(const QDomNode& node)
{
    map<int,int> iv_map;
    QDomNode ivNode = node.firstChild();
    while(!ivNode.isNull())
    {
        if(ivNode.isElement())
        {
            QDomElement ivElement = ivNode.toElement();
            if(!ivElement.isNull())
            {
                if(ivElement.tagName() == "IV_elem")
                {
                    iv_map.insert(pair<int,int>(ivElement.attribute("malware_id","").toInt(),
                                                ivElement.text().toInt()));
                }
            }
        }
    ivNode = ivNode.nextSibling();
    }
    return iv_map;
}

domain_data_struct traverse_domain_node(const QDomNode& node)
{
    domain_data_struct dds;
    QDomNode domainNode = node.firstChild();
    while(!domainNode.isNull())
    {
      if(domainNode.isElement())
      {
          QDomElement domainElement = domainNode.toElement();
          if(!domainElement.isNull())
          {
              if(domainElement.tagName() == "hosts_number")
              {
                  dds.active_hosts_number = domainElement.text().toInt();
                  dds.total_hosts_number = domainElement.text().toInt();
              }
              else if(domainElement.tagName() == "res")
              {
                  dds.total_resoures = domainElement.text().toFloat();
              }
              else if(domainElement.tagName() == "SV")
              {
                  dds.security_vector = traverse_sv_node(domainNode);
              }
              else if(domainElement.tagName() == "IV")
              {
                  dds.infection_vector = traverse_iv_node(domainNode);
              }

          }
      }
      domainNode = domainNode.nextSibling();
    }
    return dds;
}

malware_data_struct traverse_malware_node(const QDomNode& node)
{
    malware_data_struct mds;
    QDomNode malwareNode = node.firstChild();
    while(!malwareNode.isNull())
    {
        if(malwareNode.isElement())
        {
            QDomElement malwareElement = malwareNode.toElement();
            if(!malwareElement.isNull())
            {
                if(malwareElement.tagName() == "infection_speed")
                {
                    mds.infection_speed = malwareElement.text().toInt();
                }
            }
        }
        malwareNode = malwareNode.nextSibling();
    }
    return mds;
}

vector<string> traverse_tag_cloud_node(const QDomNode& node)
{
    vector<string> tc;
    QDomNode tagCloudNode = node.firstChild();
    while(!tagCloudNode.isNull())
    {
        if(tagCloudNode.isElement())
        {
            QDomElement tagCloudElement = tagCloudNode.toElement();
            if(!tagCloudElement.isNull())
            {
                if(tagCloudElement.tagName() == "tag")
                {
                    tc.push_back(tagCloudElement.text().toStdString());
                }
            }
        }
        tagCloudNode = tagCloudNode.nextSibling();
    }
    cout << endl;
    return tc;
}

vector<int> traverse_flow_path_node(const QDomNode& node)
{
    vector<int> fp;
    QDomNode flowPathNode = node.firstChild();
    while(!flowPathNode.isNull())
    {
        if(flowPathNode.isElement())
        {
            QDomElement flowPathElement = flowPathNode.toElement();
            if(!flowPathElement.isNull())
            {
                if(flowPathElement.tagName() == "domain_in_path")
                {
                    fp.push_back(flowPathElement.text().toInt());
                }
            }
        }
        flowPathNode = flowPathNode.nextSibling();
    }
    cout << endl;
    return fp;
}

flow_data_struct traverse_flow_node(const QDomNode& node)
{
    flow_data_struct fds;
    QDomNode flowNode = node.firstChild();
    while(!flowNode.isNull())
    {
        if(flowNode.isElement())
        {
            QDomElement flowElement = flowNode.toElement();
            if(!flowElement.isNull())
            {
                if(flowElement.tagName() == "source_domain")
                {
                    fds.source_domain = flowElement.text().toInt();
                }
                else if(flowElement.tagName() == "destination_domain")
                {
                    fds.destination_domain = flowElement.text().toInt();
                }
                else if(flowElement.tagName() == "start_rate_function")
                {
                    fds.start_rate_function = flowElement.text().toStdString();
                }
                else if(flowElement.tagName() == "flow_path")
                {
                    fds.path_calc_type = flowElement.attribute("type","").toStdString();
                    if (fds.path_calc_type == "manual")
                    {
                        fds.flow_path = traverse_flow_path_node(flowNode);
                    }
                }
                else if(flowElement.tagName() == "tag_cloud")
                {
                    fds.tag_cloud = traverse_tag_cloud_node(flowNode);
                }
                else if(flowElement.tagName() == "malware_flag")
                {
                    if (flowElement.text() == "true")
                        fds.malware_flag = true;
                    else
                        fds.malware_flag = false;
                }
            }
        }
        flowNode = flowNode.nextSibling();
    }
    return fds;
}


void traverseNode(const QDomNode& node, GCN* network)
{
   QDomNode domNode = node.firstChild();
   while(!domNode.isNull())
   {
       if(domNode.isElement())
       {
           QDomElement domElement = domNode.toElement();
           if(!domElement.isNull())
           {

               if(domElement.tagName() == "domain")
               {
                   domain_data_struct dds;
                   dds = traverse_domain_node(domNode);
                   dds.id = domElement.attribute("id", "").toInt();
                   network->add_domain(dds.id,dds.total_hosts_number,dds.security_vector,
                                       dds.infection_vector,dds.total_resoures);
               }
               else if(domElement.tagName() == "link")
               {
                   link_data_struct lds;
                   lds.id = domElement.attribute("id","").toInt();
                   lds.s_domain_id = domElement.attribute("s_domain_id","").toInt();
                   lds.d_domain_id = domElement.attribute("d_domain_id","").toInt();
                   lds.bandwidth = domElement.text().toFloat();
                   // there is no speccial function traverse_link_node  - because i'm lazy =)

                   network->add_link(lds.id,lds.s_domain_id,lds.d_domain_id,lds.bandwidth);
               }
               else if(domElement.tagName() == "malware")
               {
                   malware_data_struct mds;
                   mds = traverse_malware_node(domNode);
                   mds.id = domElement.attribute("id","").toInt();
                   network->add_malware(mds.id,mds.infection_speed);
               }
               else if(domElement.tagName() == "flow")
               {
                   flow_data_struct fds;
                   fds = traverse_flow_node(domNode);
                   fds.id = domElement.attribute("id","").toInt();
                   network->add_flow(fds.id,fds.source_domain,fds.destination_domain,
                                     fds.start_rate_function,fds.path_calc_type,fds.flow_path,fds.tag_cloud,fds.malware_flag);
               }
           }           
       }
       traverseNode(domNode,network);
       domNode = domNode.nextSibling();
   }
}

void Read_Config(GCN* network)
{
    QDomDocument domDoc;
    QFile        file("C:\\Users\\anvial\\CppPr\\SystemModel\\net_config.xml");
    if(file.open(QIODevice::ReadOnly))
    {
        if(domDoc.setContent(&file))
        {
            QDomElement domElement= domDoc.documentElement();
            traverseNode(domElement, network);
        }
        file.close();
    }

}

int main()
{
    GCN* network = new GCN();

    Read_Config(network);
    network->show_all();

    //BEGIN TEST ZONE
    //END TEST ZONE

    // main loop
    time_curr = 0.0;

    while(time_curr <= time_fin)
    {
        cout << "Curr time = " << time_curr << endl;

        //time inc
        time_curr += time_step;
        network->tick();

    }

    return 0;
}

