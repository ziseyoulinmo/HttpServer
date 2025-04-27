#pragma once


#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<cstring>
#include"Log.hpp"


#define BACKLOG 10

class TcpServer
{
private:
    int port_;
    int listenSock_;
    static TcpServer *srv;

private:
    TcpServer(int port)
    :port_(port),
    listenSock_(-1)
    {};

    TcpServer(const TcpServer &t){};

public:
    static TcpServer* getInstance(int port)
    {
        static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        if(srv == nullptr)
        {
            pthread_mutex_lock(&lock);
            if(srv == nullptr)
            {
                srv = new TcpServer(port);
                srv->init();
            }
            pthread_mutex_unlock(&lock);
        }
        return srv;
    }

    void init()
    {
        Sock();
        Bind();
        Listen();
    }

    void Sock()
    {
        listenSock_ = socket(AF_INET,SOCK_STREAM,0);
        if(listenSock_ < 0)
        {
            LOG(FATAL,"socket error!!!");
            exit(1);
        }
        
        int opt = 1;
        setsockopt(listenSock_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        LOG(INFO,"create socket success");
    }

    void Bind()
    {
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        local.sin_addr.s_addr = INADDR_ANY;

        if(bind(listenSock_,(struct sockaddr*)&local,sizeof(local)) < 0)
        {
            LOG(FATAL,"bind error!!!");
            exit(2);
        }
        LOG(INFO,"bind success");
    }

    void Listen()
    {
        if(listen(listenSock_,BACKLOG) < 0)
        {
            LOG(FATAL,"listen error!!!");
            exit(3);
        }
        LOG(INFO,"listen success");
    }

    ~TcpServer()
    {
        if(listenSock_ >= 0)
            close(listenSock_);
    }
};


TcpServer* TcpServer::srv = nullptr;
