//
// Created by Pascal Knierim on 07/02/17.
//

#ifndef THERMALVISUALIZATION_THERMALDATASERVER_H
#define THERMALVISUALIZATION_THERMALDATASERVER_H

#include<stdio.h>
#include <iostream>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <vector>

#include <QObject>
#include "ImageBuilder.h"


using namespace std;

class ThermalDataServer : public QObject {
Q_OBJECT


public slots:
    void setTemperatureLine(QPoint start, QPoint end);

public:
    ThermalDataServer(optris::ImageBuilder *pBuilder);
    void start();
    std::vector<QPoint> line;
    optris::ImageBuilder *_pBuilder;

private:
    void * get_in_addr(struct sockaddr * sa);
    //void Send(std::vector<float>& vec, int sock );

    int status;
    struct addrinfo hints, * res;
    int listner;

    // We should wait now for a connection to accept
    int new_conn_fd;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    char s[INET6_ADDRSTRLEN]; // an empty string
};


#endif //THERMALVISUALIZATION_THERMALDATASERVER_H
