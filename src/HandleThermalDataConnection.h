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

#include <stdlib.h>
#include <iomanip>

#include <time.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <string>

#include "dirent.h"

#include "Settings.h"

using namespace std;

typedef std::int64_t size_type;

class HandleThermalDataConnection {

public:
    HandleThermalDataConnection(int client_fd, ThermalDataServer *server);

    void start();

private:
    int _client_fd;
    ThermalDataServer *_server;



    double elapsedSeconds;
    bool init = false;

    vector<float> getNextLineAndSplitIntoTokens(std::istream& str);
    vector<float> prepareRecordedData(int time);

    void Send(std::vector<float> &vec, int sock) {
        const size_type sz = vec.size();
        float *t = &(vec.front());
        send(sock, t, sz * sizeof(float), 0);
    }

    string TAG = "[HandleThermalConnection]: ";



};


#endif //THERMALVISUALIZATION_HANDLETHERMALDATACONNECTION_H
