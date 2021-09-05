#ifndef INCLUDE_HEADER_SESSION_H
#define INCLUDE_HEADER_SESSION_H
class CSession
{
public:
    CSession(int fd);
    ~CSession();

    int run();

private:
    int m_fd;

};

#endif