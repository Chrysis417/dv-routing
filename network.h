
//本文件定义路由表的结构以及相关操作
#ifndef NETWORK_H
#define NETWORK_H

#include<string>
#include<bits/stdc++.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include "TransportLayer.h"

using namespace std;

//邻居信息结构体
typedef struct{
    string node_id;
    int16_t address;
    double ttl;//邻居剩余存活时间
    bool flag;//是否接收到该邻居的消息
    int cost;
}neigh;

class netNode{
private:
    string node_id;//本路由器
    unordered_map<string,pair<string,int16_t>> routing_table;//目标路由器id : <下一跳路由器id,cost>
    unordered_map<string,int16_t> address_table;//路由器id : 路由器地址 用于查询地址
    unordered_map<string,neigh> neighs;//邻居id : 邻居信息
public:
    netNode(string);
    //getter&setter
    void set_Node_id(string node_id) {this->node_id=node_id;}
    string get_Node_id() {return this->node_id;}
    unordered_map<string,pair<string,int16_t>> get_Routing_table(){return this->routing_table;}
    unordered_map<string,int16_t> get_address_table(){return this->address_table;}
    unordered_map<string,neigh> get_neigh_list(){return this->neighs;}

    //对路由表的操作
    void insert_routing(string dest,string next,int cost){routing_table[dest]=make_pair(next,cost);}
    void update_routing(string dest,string new_next,int new_cost)
    {
        if(new_cost<routing_table[dest].second) routing_table[dest]=make_pair(new_next,new_cost);
    }
    void delete_routing(string dest){routing_table[dest].second=12450;}

    //其他操作

    //初始化routing_table和neigh_table.接收topology.txt
    int init_routing_table(string);
    //初始化address_table.接收nodeaddr.txt
    int init_address_table(string);
    
    int display_node_table();
    int update_table(vector<string>);//根据接收的信息更新路由表

    static void* update_neigh(void*);//定时(5s)检查邻居超时情况，并更新路由表
    static void* periodec_flood(void*);//定时(2s)向邻居发送路由信息

    static string encode(netNode&);//根据路由表生成并返回发送的数据包
};

//存储传入线程的对象
typedef struct{
    netNode* netnode;
    transportLayer* p_t_l;
}thread_object;

#endif //NETWORK_H