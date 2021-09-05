#include <iostream>
#include "client.h"

int main(int argc,char** argv)
{
    std::cout << "main begin"<<std::endl;
    CClient clt;
    clt.run();
    std::cout<< "main end"<<std::endl;
    return 0;
}