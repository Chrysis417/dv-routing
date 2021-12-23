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


#include "TransportLayer.h"
#include "network.h"

transportLayer::transportLayer(std::string & src_IP, int16_t src_port)
{
    //以默认参数创建socket,传输层协议为UDP
    sock_fd = socket(AF_INET, SOCK_DGRAM,0);
    memset(&src_addr, 0, sizeof(struct sockaddr_in));
    src_addr.sin_family = AF_INET; 
    src_addr.sin_port = htons(src_port); 
    src_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    len = sizeof(src_addr);

    /* 绑定socket */
    if(bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0)
    {
        perror("bind error:");
        exit(1);
    }

    const int on = 1;
    setsockopt(sock_fd,SOL_SOCKET ,SO_REUSEADDR,&on,sizeof(on));

    //创建epoll
    flags = fcntl(sock_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(sock_fd, F_SETFL, flags);

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sock_fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_fd, &ev) == -1) {
        perror("epoll_ctl: sock_fd");
        exit(EXIT_FAILURE);
    }

}

transportLayer::~transportLayer()
{
    close(sock_fd);
}

int transportLayer::get_msg(char * recv_buf, int16_t buf_len, int TO)
{
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, TO);
    if(nfds == -1) return -1;
    else {
        for(int i=0;i<nfds;i++)
        {
            if(events[i].data.fd == sock_fd){
                recv_num = recvfrom(sock_fd, recv_buf, buf_len, 0, (struct sockaddr *) &peer_addr,
                        (socklen_t *) &peer_addr_len);

                if (recv_num < 0) {
                    perror("recvfrom error:");
                    exit(1);
                }

                recv_buf[recv_num] = '\0';
                return recv_num;
            }
        } 
    } 

    return 0;
}


int transportLayer::send_msg(std::string & dest_IP, int16_t dest_port, char * send_buf, int16_t msg_len) {
    memset(&dest_addr, 0, sizeof(struct sockaddr_in)); 
    dest_addr.sin_family = AF_INET; 
    dest_addr.sin_port = htons(dest_port); 
    dest_addr.sin_addr.s_addr = inet_addr(dest_IP.c_str());

    send_num = sendto(sock_fd, send_buf, msg_len, 0, (struct sockaddr *)&dest_addr, len);

    if(send_num < 0){
        perror("sendto error:");
        exit(1);
    }

    return send_num;
}




