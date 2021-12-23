//本文件给出路由器间收发数据的结构
//以及对应编码解码方法，以及发送接收操作接口
#ifndef MSG_H
#define MSG_H
#include <bits/stdc++.h>
using std::string;
using std::vector;
class DV_Msg{
public:
    // string from_node_id;
    // int cost;
    vector<string> decoded_result;

    // DV_Msg();
    // ~DV_Msg();

    static vector<string> decode(string string_to_be_decoded);
    
};
#endif //MSG_H