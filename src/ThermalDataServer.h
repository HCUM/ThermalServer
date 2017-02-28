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
#include <QTimer>



using namespace std;

class ThermalDataServer : public QObject {
Q_OBJECT


public slots:
    void setTemperatureLine(QPoint start, QPoint end);
    void updateStationaryState();
    void setMinVisualisationValue(int min);
    void setMaxVisualisationValue(int max);

signals:
    void  maxTempChanged(float maxChange);

public:
    ThermalDataServer(optris::ImageBuilder *pBuilder);
    void start();
    std::vector<QPoint> line;
    optris::ImageBuilder *_pBuilder;
    float maxChange = 0;
    int _min;
    int _max;

private:
    void * get_in_addr(struct sockaddr * sa);
    QTimer *timer;

    int status;
    struct addrinfo hints, * res;
    int listner;

    int new_conn_fd;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    char s[INET6_ADDRSTRLEN]; // an empty string

    bool reset = true;
    std::vector<float> oldTempVector;



};


#endif //THERMALVISUALIZATION_THERMALDATASERVER_H