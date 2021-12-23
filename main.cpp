
#include<bits/stdc++.h>
#include<fstream>
#include<utility>
#include<threads.h>
#include <unistd.h>

#include "msg.h"
#include "network.h"
#include "TransportLayer.h"
#include "user_cmd.h"

using std::cout;
using std::endl;

std::string src_IP_address {"127.0.0.1"};
std::string dest_IP_address {"127.0.0.1"};
#define MAX_CMD_LEN 100

int main(int argc, char* argv[])
{
    
    string node_id,nodeAddrPath,nodeTopologyPath;
    string input_str;
    if(argc!=4)
    {
        cout<<"you must run this program with nodeid, path of nodeaddr and topology"<<endl;
        return 0;
    }
    node_id=argv[1];
    nodeAddrPath=argv[2];
    nodeTopologyPath=argv[3];
    cout<<"The ID of this router is : "<<node_id<<endl;

    //初始化本路由器
    netNode* netnode=new netNode(node_id);
    netnode->init_address_table(nodeAddrPath);
    netnode->init_routing_table(nodeTopologyPath);
    //初始化传输层
    unordered_map<string,int16_t> address_table=netnode->get_address_table();
    transportLayer* p_t_l=new transportLayer(src_IP_address,address_table[node_id]);


    unordered_map<string,neigh> neighs=netnode->get_neigh_list();
    cout<<node_id<<"'s neighbors are : "<<endl;
    for(auto i:neighs) cout<< i.first << "  " << i.second.address <<endl;


    thread_object* to=new thread_object();
    to->netnode=netnode;
    to->p_t_l=p_t_l;
    //周期性地清除不活跃邻居
    pthread_t neigh_thread,flood_thread;
    pthread_create(&neigh_thread,NULL,netNode::update_neigh,(void*) to);
    //周期性向邻居发送路由表
    pthread_create(&neigh_thread,NULL,netNode::periodec_flood,(void*) to);

    char user_cmd[MAX_CMD_LEN];
    char recv_buf[1000];
    int byte_received;
    string encoded_msg_str;
    while(1)
    {
        if(read_user_cmd(user_cmd,MAX_CMD_LEN) == 1){
            if(strcmp(user_cmd,"exit") == 0) exit(0);
            else if(strcmp(user_cmd,"disp") == 0) netnode->display_node_table();
        }

        byte_received=0;
        byte_received = p_t_l->get_msg(recv_buf,1000,1000);

        if(byte_received <= 0) continue;
        recv_buf[byte_received] = '\0';

        string received_string = recv_buf;

        DV_Msg* recv_msg=new DV_Msg();
        recv_msg->decoded_result=DV_Msg::decode(received_string);
        netnode->update_table(recv_msg->decoded_result);
        
    }


}