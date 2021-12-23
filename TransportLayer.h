
//本文件给出socket收发信息的接口
//仿照给出的示例
#ifndef TRANS_H
#define TRANS_H
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

using std::string;
#define MAX_EVENTS 10
class transportLayer{
private:
    unsigned short src_port,dest_port;
    int sock_fd;
    int flags;
    int epollfd,nfds;
    struct epoll_event ev,events[MAX_EVENTS];

    struct sockaddr_in src_addr,dest_addr,peer_addr;
    int len;
    int peer_addr_len;

    int recv_num;
    int send_num;

public:
    transportLayer(string & src_IP,int16_t src_port);
    ~transportLayer();

    int get_msg(char *recv_buf, int16_t buf_len, int to);
    int send_msg(std::string & dest_IP, int16_t dest_port, char *send_buf, int16_t msg_len);


};
#endif //TRANS_H