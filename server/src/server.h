#ifndef INCLUDE_HEADER_SERVER_H
#define INCLUDE_HEADER_SERVER_H
class CServer
{
public:
    CServer();
    ~CServer();

    int run();

private:
    int setNonBlocking(int fd);
    int tcp_block();
    int udp_block();
    int run_select();
    int run_poll();
    int run_epoll();
    int addfd(int epfd,int fd,bool enable_et);



};

#endif