#include "network.h"
using namespace std;

netNode::netNode(string node_id){
    this->node_id=node_id;
}
int netNode::init_routing_table(string neigh_costs_file_in)
{
    ifstream neigh_costs_in {neigh_costs_file_in};
    if(!neigh_costs_in)
    {
        cerr<<neigh_costs_file_in<<" not open ."<<endl;
        exit(1);
    }
    string fromNode,toNode,cost;
    while(neigh_costs_in >> toNode && neigh_costs_in >> fromNode && getline(neigh_costs_in,cost))
    {
        if(fromNode == this->node_id && toNode == this->node_id) continue;
        if(fromNode == this->node_id) 
        {
            neigh n;
            n.address=address_table[toNode];
            n.node_id=toNode;
            n.flag=false;
            n.ttl=5.0;
            n.cost=stoi(cost);
            neighs[toNode]=n;
            routing_table[toNode]=make_pair(toNode,stoi(cost));
            // cout<<"neigh:"<<neighs[toNode].node_id<<endl;
            // cout<<"rtable:"<<routing_table[toNode].first<<endl;
        }
        if(toNode == this->node_id)
        {
            neigh n;
            n.address=address_table[fromNode];
            n.node_id=fromNode;
            n.flag=false;
            n.ttl=5.0;
            n.cost=stoi(cost);
            neighs[fromNode]=n;
            routing_table[fromNode]=make_pair(fromNode,stoi(cost));
            // cout<<"neigh:"<<neighs[fromNode].node_id<<endl;
            // cout<<"rtable:"<<routing_table[fromNode].first<<endl;
        }
    }
    return 0;
}
int netNode::init_address_table(string address_file_in)
{
    ifstream address_in {address_file_in};
    if(!address_in)
    {
        cerr<<address_file_in<<" not open ."<<endl;
        exit(1);
    }
    string nodeID,address;
    while(address_in >> nodeID && getline(address_in,address))
    {
        address_table[nodeID]=stoi(address);
    }
    return 0;
}

int netNode::display_node_table()
{
    string node_id=get_Node_id();
    cout<<"router "<<node_id<<"'s routing table:"<<endl;
    for(auto iter=routing_table.begin();iter!=routing_table.end();iter++)
    {
        cout<<node_id<<" to "<<iter->first<<" via "<<iter->second.first<<" costs "<<iter->second.second<<endl;
    }
    return 0;
}
//  nodeA#nodeB/14#nodeF/31#

int netNode::update_table(vector<string> encoded)
{
    string from_node=encoded[0];
    // 本例dv算法中，发送方一定是邻居
    neighs[from_node].flag=true;
    routing_table[from_node].second=neighs[from_node].cost;
    // cout<<"encoded size:"<<encoded.size()<<endl;
    // for(int i=0;i<encoded.size();i++) cout<<encoded[i]<<endl;
    //assert:encoded.size()一定是奇数
    for(int i=1;i<encoded.size();i+=2)
    {
        string to_node=encoded[i];
        if(to_node==this->node_id) continue;
        int cost=stoi(encoded[i+1]);

        cost+=routing_table[from_node].second;


        auto old_cost=routing_table.find(to_node);
        if(old_cost == routing_table.end())//路由表中没有该条目
        {
            routing_table[to_node]=make_pair(from_node,cost);
        }else if(routing_table[to_node].second>1000)//该条目被删除过
        {
            routing_table[to_node]=make_pair(from_node,cost);
        }
        else {//路由表中有该条目
            if(old_cost->second.second>cost) routing_table[to_node]=make_pair(from_node,cost);
        }      
    }
    //cout<<this->node_id<<"'s routing_table has been updated by "<<from_node<<"'s message"<<endl;
    return 0;
}

void* netNode::update_neigh(void *args)
{
    thread_object* to=(thread_object*)args;
    netNode* netnode=to->netnode;
    while(1)
    {
        sleep(5);//#define NEIGH_TIME_TO_LIVE 5
        for(auto i:netnode->neighs)
        {
            if(!i.second.flag) netnode->delete_routing(i.second.node_id);
            i.second.flag=false;
        }
        
    }
}
void* netNode::periodec_flood(void *args)
{ 
    thread_object* to=(thread_object*)args;
    netNode* netnode=to->netnode;
    transportLayer* p_t_l=to->p_t_l;
    while(1)
    {
        //把string转换为c-string   
        string msg=netNode::encode(*netnode);
        char* c_msg=(char *)malloc((msg.size()+1)*sizeof(char));
        msg.copy(c_msg,msg.size(),0);
        c_msg[msg.size()]='\0';

        string dest_ip="127.0.0.1";
        for(auto i:netnode->neighs)
        {
            p_t_l->send_msg(dest_ip,i.second.address,c_msg,msg.size()); 
        }

        sleep(2);//#define FLOOD_PERIOD 2
    }
}

string netNode::encode(netNode& node)
{
    //假设由路由器A到B、F的cost分别为14，31，则编码为
    //nodeA#nodeB/14#nodeF/31#
    //信息接收方不关心下一跳，可以省略
    //由于每个路由器都存储了当前网络内其他路由器的id与地址的对应关系address_table，不用另行传输
    string message=node.get_Node_id();
    unordered_map<string,pair<string,int16_t>> rtable=node.get_Routing_table();
    for(auto it=rtable.begin();it!=rtable.end();it++)
    {
        string toNode=it->first;
        int cost=it->second.second;
        if(cost>1000) continue;
        message+="#" + toNode + "/" + to_string(cost) + "#";
    }
    return message;
   
}