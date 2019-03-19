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
#include "ThermalWorker.h"
#include <ctime>


using namespace std;
using namespace evo;

class ThermalDataServer : public QObject {
Q_OBJECT


public slots:
    void setTemperatureLine(QPoint start, QPoint end);
    void updateStationaryState();

    void exportData();
    // void setMinVisualisationValue(int min);
    // void setMaxVisualisationValue(int max);

signals:
    void  maxTempChanged(float maxChange);

public:
    ThermalDataServer(Worker *_worker); //ImageBuilder *pBuilder);
    vector<string> * files;
    double startSecond;

    void start();
    std::vector<QPoint> line;
    Worker *worker;
    float maxChange = 0;

    string TAG = "[ThermalDataServer]: ";

private:
    void * get_in_addr(struct sockaddr * sa);
    QTimer *timer;

    bool printDiff = false;
    int counter = 0;
    float newTemp = 0;
    float oldTemp = 0;

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
