#include <iostream>
#include "server.h"

int main(int argc,char** argv)
{
    std::cout << "main begin"<<std::endl;
    CServer svr;
    svr.run();
    std::cout<< "main end"<<std::endl;
    return 0;
}