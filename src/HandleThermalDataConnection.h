//
// Created by Pascal Knierim on 26/02/17.
//

#ifndef THERMALVISUALIZATION_HANDLETHERMALDATACONNECTION_H
#define THERMALVISUALIZATION_HANDLETHERMALDATACONNECTION_H


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

typedef std::int64_t size_type;

class HandleThermalDataConnection {

private:
    int _client_fd;
    ThermalDataServer* _server;

    void Send(std::vector<float>& vec, int sock )
    {
        const size_type sz = vec.size();
        float* t = &(vec.front());
        send( sock, t , sz * sizeof( float ) ,0);
    }

public:

    HandleThermalDataConnection(int client_fd, ThermalDataServer *server){
        _server = server;
        _client_fd = client_fd;
    }

    void handleConnection(){
        while(1) {
            char buffer[256];
            bzero(buffer, 256);
            int n = read(_client_fd, buffer, 255);
            if(n==0) break;


            if(buffer[0] == 'E'){
                time_t rawtime;
                struct tm * timeinfo;
                char bufferTime[80];

                time (&rawtime);
                timeinfo = localtime(&rawtime);
                strftime(bufferTime,80,"/tmp/export/thermalExport%d-%m-%Y %I:%M:%S.csv",timeinfo);
                std::string str(bufferTime);

                cout << "export thermal data" << endl;
                std::ofstream tempFile(str);
                float temp = 0;
                for (auto &point : _server->line) // access by reference to avoid copying
                {
                    temp = _server->_pBuilder->getTemperatureAt(point.x(), point.y());
                    tempFile << temp << ", ";
                }
                tempFile.close();
            }

            if(buffer[0] == 'S') {
                cout << "send thermal data" << endl;
                std::vector<float> ar;
                float temp = 0;

                // Save min and max to array
                ar.push_back(_server->_min);
                ar.push_back(_server->_max);

                for (auto &point : _server->line) // access by reference to avoid copying
                {
                    temp = _server->_pBuilder->getTemperatureAt(point.x(), point.y());
                    ar.push_back(temp);
                }
                Send(ar, _client_fd);
            }

            if(buffer[0] == 'M') {
                cout << "send MaxCahnge" << endl;
                send( _client_fd, &(_server->maxChange), sizeof( float ) ,0);
            }

        }
        std::cout << "ThermalData connection closed. ConnectionID: " << _client_fd << std::endl;
    }
};



#endif //THERMALVISUALIZATION_HANDLETHERMALDATACONNECTION_H