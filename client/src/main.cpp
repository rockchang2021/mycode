#include <iostream>

#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>

int tcp_client()
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


int main(int argc,char** argv)
{
    std::cout << "main begin"<<std::endl;
    tcp_client();
    std::cout<< "main end"<<std::endl;
    return 0;
}