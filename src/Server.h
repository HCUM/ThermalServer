//
// Created by Pascal Knierim on 01/02/17.
//

#ifndef THERMALVISUALIZATION_SERVER_H
#define THERMALVISUALIZATION_SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <thread>
#include <opencv2/core/types_c.h>
#include <vector>


class Server {


public:
    std::vector<uchar> buf;


public:
    Server();
    void start();

private:
    void connect() ;
    std::thread t;
};



#endif //THERMALVISUALIZATION_SERVER_H
