#ifndef INCLUDE_HEADER_CLIENT_H
#define INCLUDE_HEADER_CLIENT_H
class CClient
{
public:
    CClient();
    ~CClient();

    int run();
private:
    int tcp_block();
    int udp_block();

};

#endif