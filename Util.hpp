#pragma once

#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>


class Util{
public:
    static int ReadLine(int sock,std::string &out)
    {
        char ch = '0';
        while(char != '\n')
        {
            ssize_t s = recv(sock,&ch,1,0);
            if(s > 0)
            {
                //将\r\n或者\r都转变为\n;
                if(ch == '\r')
                {
                    recv(sock,&ch,1,MSG_PEEK);
                    if(ch == '\n')
                    {
                        recv(sock,&ch,1,0);
                    }
                    else
                    {
                        ch = '\n';
                    }
                }
                out.push_back(ch);
            }
            else if(s == 0)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }
        return out.size();
    }
};
