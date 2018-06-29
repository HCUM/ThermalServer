//
// Created by Pascal Knierim on 26/02/17.
//

#include "HandleThermalDataConnection.h"
#include <iomanip>


#define DEBUG  true

HandleThermalDataConnection::HandleThermalDataConnection(int client_fd, ThermalDataServer *server) {
    _server = server;
    _client_fd = client_fd;
}




vector<float> getNextLineAndSplitIntoTokens(std::istream& str)
{
    vector<float>   result;
    string                line;
    getline(str,line);

    stringstream          lineStream(line);
    string                cell;

    //set min and max // ToDo: make it dynamic
    result.push_back(20);
    result.push_back(35);

    while(getline(lineStream,cell, ','))
    {
        float tmp = strtof(cell.c_str(),NULL);
        result.push_back(tmp);
    }
    return result;
}



void HandleThermalDataConnection::start() {
    while (1) {
        char buffer[256];
        bzero(buffer, 256);
        int n = read(_client_fd, buffer, 255);
        if (n == 0) {
            break;
        }


        if (buffer[0] == 'E') {
            cout << TAG << "Export data" << endl;
            _server->exportData();
        }

        if (buffer[0] == 'S') {

            vector<float> ar;
            float temp = 0;

            if(Settings::getInstance().isStreaming()) {
                std::cout << TAG << "Send Replay Data !!!!" << _client_fd << std::endl;
                std::ifstream       file(Settings::getInstance().getStreamFilename());
                ar = getNextLineAndSplitIntoTokens(file);
            } else{
                std::cout << TAG << "Send Live Data !!!!" << _client_fd << std::endl;
                ar.push_back(_server->worker->getMinScale());
                ar.push_back(_server->worker->getMaxScale());

                for (auto &point : _server->line) // access by reference to avoid copying
                {
                    temp = _server->worker->getTemperatureAt(point.x(),point.y());
                    ar.push_back(temp);
                }
            }
            Send(ar, _client_fd);
        }

        if (buffer[0] == 'M') {
            send(_client_fd, &(_server->maxChange), sizeof(float), 0);
        }


    }
    close(_client_fd);
#if DEBUG
    std::cout << TAG << "connection closed; ConnectionID: " << _client_fd << std::endl;
#endif
}