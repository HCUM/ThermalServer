//
// Created by Pascal Knierim on 01/02/17.
//

#ifndef THERMALVISUALIZATION_CONNECTIONHANDLER_H
#define THERMALVISUALIZATION_CONNECTIONHANDLER_H


#include "Server.h"

class ConnectionHandler {

public:

    ConnectionHandler::ConnectionHandler(int _client_fd, Server* _server);
    void handleConnection();
    int client_fd;

};


#endif //THERMALVISUALIZATION_CONNECTIONHANDLER_H
