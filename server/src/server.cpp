#include "server.h"
#include "session.h"
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define UDP_SERVER_PORT 37778
#define TCP_SERVER_PORT 37777
#define MAX_EVENT_NUMBER 1024



CServer::CServer()
{

}
CServer::~CServer()
{

}

int CServer::run()
{
    run_epoll();
    return 0;
}

int CServer::setNonBlocking(int fd)
{
    int iOldOption = fcntl(fd,F_GETFL);
    int iNewOption = iOldOption | O_NONBLOCK;
    fcntl(fd,F_SETFL,iNewOption);
    return iOldOption;
}
int CServer::tcp_block()
{
    //ip and addr;
    int port = TCP_SERVER_PORT;
    char ip[] = "127.0.0.1";
    //socket;
    struct sockaddr_in ipaddr;
    bzero(&ipaddr,sizeof(struct sockaddr_in));
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_port = htons(port);
    inet_pton(AF_INET,ip,&ipaddr.sin_addr);

    int sfd = socket(PF_INET,SOCK_STREAM,0);
    if (sfd < 0)
    {
        return -1;
    }

    int iRet = bind(sfd,(struct sockaddr*)&ipaddr,sizeof(struct sockaddr_in));
    if(iRet < 0)
    {
        return -1;
    }

    iRet = listen(sfd,5);
    if(iRet < 0)
    {
        return -1;
    }

    for(;;)
    {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(struct sockaddr_in);
        bzero(&clientaddr,len);
        int cfd = accept(sfd,(struct sockaddr*)&clientaddr,&len);
        if(cfd < 0)
        {
            continue;
        }
        else
        {
            printf("client %d\n",cfd);
        }
    }

    close(sfd);
    return 0;
}

int CServer::udp_block()
{
    struct sockaddr_in ipaddr;
    bzero(&ipaddr,sizeof(struct sockaddr_in));
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ipaddr.sin_port = htons(UDP_SERVER_PORT);

    int ufd = socket(PF_INET,SOCK_DGRAM,0);
    int iRet = bind(ufd,(struct sockaddr*)&ipaddr,sizeof(struct sockaddr_in));
    if(iRet < 0)
    {
        return -1;
    }

    for(;;)
    {
        struct sockaddr_in clientaddr;
        bzero(&clientaddr,sizeof(struct sockaddr_in));
        socklen_t len = sizeof(struct sockaddr_in);
        char buf[128];
        bzero(buf,sizeof(buf));
        int iLen = recvfrom(ufd,buf,sizeof(buf),0,(struct sockaddr*)&clientaddr,&len);
        if(iLen == -1)
        {
            continue;
        }
        sendto(ufd,buf,sizeof(buf),0,(struct sockaddr*)&clientaddr,len);
    }
    return 0;
}
int CServer::run_select()
{
    return 0;
}
int CServer::run_poll()
{
    return 0;
}
int CServer::run_epoll()
{
    //ip and addr;
    int port = TCP_SERVER_PORT;
    char ip[] = "127.0.0.1";
    //socket;
    struct sockaddr_in ipaddr;
    bzero(&ipaddr,sizeof(struct sockaddr_in));
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_port = htons(port);
    inet_pton(AF_INET,ip,&ipaddr.sin_addr);

    int sfd = socket(PF_INET,SOCK_STREAM,0);
    if (sfd < 0)
    {
        return -1;
    }

    int iRet = bind(sfd,(struct sockaddr*)&ipaddr,sizeof(struct sockaddr_in));
    if(iRet < 0)
    {
        return -1;
    }

    iRet = listen(sfd,5);
    if(iRet < 0)
    {
        return -1;
    }

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    if (epollfd == -1)
    {
        return -1;
    }
    
    addfd(epollfd,sfd,true);

    for(;;)
    {
        int iRet = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if (iRet < 0)
        {
            printf("epoll failure\n");
            break;
        }

        //ET mode;
        for(int i = 0;i < iRet;i++)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == sfd)
            {
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(struct sockaddr_in);
                bzero(&clientaddr,len);
                int cfd = accept(sfd,(struct sockaddr*)&clientaddr,&len);
                if(cfd < 0)
                {
                    continue;
                }
                else
                {
                    printf("client %d\n",cfd);
                    addfd(epollfd,cfd,true);
                }
            }
            else if(events[i].events & EPOLLIN)
            {
                CSession session(sockfd);
                session.run();
            }
            else
            {
                printf("something else happend\n");
            }
        }
        

    }

    close(sfd);
    return 0;
}

int CServer::addfd(int epfd,int fd,bool enable_et)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enable_et)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    setNonBlocking(fd);    
    return 0;
}