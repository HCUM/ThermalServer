//
// Created by Pascal Knierim on 07/02/17.
//

#include "ThermalDataServer.h"
#include <QPoint>
#include <thread>
#include "math.h"
#include <string.h>
#include <iostream>
#include <fstream>

#include <QTimer>


#include <iostream>
#include <sys/stat.h>
#include <ctime>


#include <stdlib.h>

#include <chrono>
#include <thread>

#include "HandleThermalDataConnection.h"


using namespace std;
using namespace evo;


void ThermalDataServer::updateStationaryState(){

    if(reset){
        reset = false;
        float temp = 0;
        for (auto &point : line) // access by reference to avoid copying
        {
            //temp = _pBuilder->getTemperatureAt(point.x(), point.y());
            oldTempVector.push_back(temp);
        }
        return;
    }

    std::vector<float> newTempVector;
    float temp = 0;
    int i = 0;
    float _maxChange = 0;
    for (auto &point : line) // access by reference to avoid copying
    {
        float diff = -1; //oldTempVector.at(i) -  _pBuilder->getTemperatureAt(point.x(), point.y());
        if(abs(diff)> _maxChange)
            _maxChange = abs(diff);
        //oldTempVector.at(i)  = _pBuilder->getTemperatureAt(point.x(), point.y());
        i++;
    }
    maxChange = _maxChange;
    //emit maxTempChanged(maxChange);
}

void ThermalDataServer::exportData() {
    cout << TAG << "Export Data " << endl;

    time_t rawtime;
    struct tm *timeinfo;
    char bufferTime[128];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char path[128];
    char *home = getenv("HOME");
    if (home != NULL) {
        snprintf(path, sizeof(path), "%s/export", home);
        strftime(bufferTime, 80, "/export/thermalExport%d-%m-%Y %I:%M:%S.csv", timeinfo);
        snprintf(path, sizeof(path), "%s%s", home, bufferTime);
    }
    std::string str(path);

    //cout << "export thermal data to "<< str << endl;
    std::ofstream tempFile(str);
    float temp = 0;

    //position:
    int values = line.size();

    float steps = Settings::getInstance().tubelength / values;

    for (int i = 0; i < values; i++) {
        tempFile << fixed << setprecision(3) << i * steps << ", ";
    }

    //new line
    tempFile << "\r\n";

    // Temp Value
    for (auto &point : line) // access by reference to avoid copying
    {
        temp = worker->getTemperatureAt(point.x(), point.y());
        tempFile << temp << ", ";
    }

    tempFile.close();

}

void ThermalDataServer::setTemperatureLine(QPoint start, QPoint end)
{

cout << TAG << "Set new Temperature Line" <<
endl;
    line.clear();
    reset = true;
    timer->start(30000);


    // calculate line with Bresenham
    int x = start.x()/2;
    int y = start.y()/2;

    int w = end.x()/2 - x ;
    int h = end.y()/2 - y ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
    if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;
    if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;
    if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;
    int longest = abs(w) ;
    int shortest = abs(h) ;
    if (!(longest>shortest)) {
        longest = abs(h) ;
        shortest = abs(w) ;
        if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;
        dx2 = 0 ;
    }
    int numerator = longest >> 1 ;
    for (int i=0;i<=longest;i++) {

        line.push_back(QPoint(x,y));
        numerator += shortest ;
        if (!(numerator<longest)) {
            numerator -= longest ;
            x += dx1 ;
            y += dy1 ;
        } else {
            x += dx2 ;
            y += dy2 ;
        }
    }
}


ThermalDataServer::ThermalDataServer(Worker *_worker) { //ImageBuilder *pBuilder) {

    worker = _worker;



    // Before using hint you have to make sure that the data structure is empty
    memset(& hints, 0, sizeof hints);
    // Set the attribute for hint
    hints.ai_family = AF_UNSPEC; // We don't care V4 AF_INET or 6 AF_INET6
    hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM
    hints.ai_flags = AI_PASSIVE;
    //hints.ai_flags = SO_REUSEADDR;

    // Fill the res data structure and make sure that the results make sense.
    status = getaddrinfo(NULL, "8888" , &hints, &res);


    if(status != 0)
    {
        fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
    }

    // Create Socket and check if error occured afterwards
    listner = socket(res->ai_family,res->ai_socktype, res->ai_protocol);

    //listner = socket(AF_INET, SOCK_STREAM, 0);

    int reuse = 1;

    //  if (setsockopt(listner, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    //      perror("setsockopt(SO_REUSEADDR) failed");

    if (setsockopt(listner, SOL_SOCKET, SO_REUSEPORT, (const char *) &reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEPORT) failed");

    if(listner < 0 )
    {
        fprintf(stderr,"socket error: %s\n",gai_strerror(status));
    }

    // Bind the socket to the address of my local machine and port number
    status = bind(listner, res->ai_addr, res->ai_addrlen);

    while (status < 0)
    {
        fprintf(stderr,"bind: %s\n",gai_strerror(status));

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        cout << TAG << ("Try to bind again, please wait...") << endl;
        status = bind(listner, res->ai_addr, res->ai_addrlen);
    }

    status = listen(listner, 10);
    if(status < 0)
    {
        fprintf(stderr,"listen: %s\n",gai_strerror(status));
    }

    // Free the res linked list after we are done with it
    freeaddrinfo(res);


    // Calculate the size of the data structure
    addr_size = sizeof client_addr;


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStationaryState()));
}



void ThermalDataServer::start() {


    char path[128];
    char *home = getenv ("HOME");
    if (home != NULL) {
        snprintf(path, sizeof(path), "%s/export", home);
        const int dir_err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }else {
        printf("Failed to create export folder");
    }


    while(1){
        // Accept a new connection and return back the socket desciptor
        //cout << TAG + "Server is accepting connections. " << endl;
        new_conn_fd = accept(listner, (struct sockaddr *) & client_addr, &addr_size);
        if(new_conn_fd < 0)
        {
            fprintf(stderr,"accept: %s\n",gai_strerror(new_conn_fd));
            continue;
        }

        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s);

        HandleThermalDataConnection* h = new HandleThermalDataConnection(new_conn_fd, this);
        std::thread* connectionThread = new std::thread();
        std::thread tmp(&HandleThermalDataConnection::start, h);
        connectionThread->swap(tmp);
        connectionThread->detach();
    }
}


void * ThermalDataServer::get_in_addr(struct sockaddr * sa)
{
    if(sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}



/*
void ThermalDataServer::setMinVisualisationValue(int min) {
    _min = min;
}

void ThermalDataServer::setMaxVisualisationValue(int max) {
    _max = max;
}
*/