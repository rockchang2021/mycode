#include <iostream>

#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define UDP_SERVER_PORT 37778
#define TCP_SERVER_PORT 37777

int tcp_client()
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
    socklen_t len = sizeof(struct sockaddr_in);
    int cfd = socket(PF_INET,SOCK_STREAM,0);
    if (cfd < 0)
    {
        return -1;
    }

    int iRet = connect(cfd,(struct sockaddr*)&ipaddr,len);

    close(cfd);
    return 0;
}

int udp_client()
{
    struct sockaddr_in ipaddr;
    bzero(&ipaddr,sizeof(struct sockaddr_in));
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ipaddr.sin_port = htons(UDP_SERVER_PORT);

    int ufd = socket(PF_INET,SOCK_DGRAM,0);

    socklen_t len = sizeof(struct sockaddr_in);
    char buf[128];
    bzero(buf,sizeof(buf));
    strcat(buf,"changquanli");
    sendto(ufd,buf,sizeof(buf),0,(struct sockaddr*)&ipaddr,len);
    bzero(buf,sizeof(buf));

    struct sockaddr_in clientaddr;
    bzero(&clientaddr,sizeof(struct sockaddr_in));
    int iLen = recvfrom(ufd,buf,sizeof(buf),0,(struct sockaddr*)&clientaddr,&len);
    if(iLen != -1)
    {
        printf("recv: %s\n",buf);
    }

    close(ufd);

    return 0;
}


int main(int argc,char** argv)
{
    std::cout << "main begin"<<std::endl;
    //tcp_client();
    udp_client();
    std::cout<< "main end"<<std::endl;
    return 0;
}