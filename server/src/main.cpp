#include <iostream>
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define UDP_SERVER_PORT 37778
#define TCP_SERVER_PORT 37777

int tcp_svr()
{
    //ip and addr;
    int port = 37777;
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

int udp_svr()
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

int main(int argc,char** argv)
{
    std::cout << "main begin"<<std::endl;
    //tcp_svr();
    udp_svr();
    std::cout<< "main end"<<std::endl;
    return 0;
}