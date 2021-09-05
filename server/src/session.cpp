#include "session.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 64

CSession::CSession(int fd)
:m_fd(fd)
{

}
CSession::~CSession()
{

}
int CSession::run()
{
    char buf[BUFFER_SIZE];
    printf("event trigger once\n");
    while(true)
    {
        memset(buf,0,sizeof(buf));
        int iRecvLen = recv(m_fd,buf,BUFFER_SIZE-1,0);
        if (iRecvLen < 0)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                printf("read later\n");
                break;
            }
            close(m_fd);
            printf("close %d\n",m_fd);
            break;
        }
        else if(iRecvLen == 0)
        {
            close(m_fd);
        }
        else
        {
            printf("get %d bytes of content: %s\n",iRecvLen,buf);
        }
        
    }
    return 0;
}