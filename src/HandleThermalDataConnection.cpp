//
// Created by Pascal Knierim on 26/02/17.
//

#include "HandleThermalDataConnection.h"

#define DEBUG  true

HandleThermalDataConnection::HandleThermalDataConnection(int client_fd, ThermalDataServer *server) {
    _server = server;
    _client_fd = client_fd;
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
            //Send confirmation
            //send(_client_fd, "sucsess", 7*sizeof(char), 0);
        }

        if (buffer[0] == 'S') {
#if DEBUG
            cout << TAG << "send thermal data" << endl;
#endif
            std::vector<float> ar;
            float temp = 0;

            // Save min and max to array
            ar.push_back(_server->worker->getMinScale());
            ar.push_back(_server->worker->getMaxScale());

            for (auto &point : _server->line) // access by reference to avoid copying
            {
                temp = _server->worker->getTemperatureAt(point.x(),
                                                         point.y()); // -1 ;//_server->_pBuilder->getTemperatureAt(point.x(), point.y());
                ar.push_back(temp);
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