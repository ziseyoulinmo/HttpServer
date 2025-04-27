#include"TcpServer.hpp"



int main(int argc,char *argv[])
{



    TcpServer* srv = TcpServer::getInstance(8080);

    return 0;
}
