//
// Created by Pascal Knierim on 01/02/17.
//

#include "Server.h"
#include "stdio.h"
#include "ConnectionHandler.h"
#include <unistd.h>
#include <iostream>


using namespace std;



void Server::connect() {

    printf("Prepare Server\n");

    // PREPARE SERVER
    int one = 1;
    struct sockaddr_in svr_addr, cli_addr;
    socklen_t sin_len = sizeof(cli_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        err(1, "can't open socket");

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    int port = 8081;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
        close(sock);
        err(1, "Can't bind");
    }

    listen(sock, 5);

    try
    {
        while (1) {
            int client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
            printf("got connection\n");

            ConnectionHandler* h = new ConnectionHandler(client_fd, this);
            thread* connectionThread = new thread();
            std::thread tmp(&ConnectionHandler::handleConnection, h);
            connectionThread->swap(tmp);
            connectionThread->detach();

            cout << "waiting for next connection, last done" << endl;
        }
    }
    catch (exception& e)
    {
        cout << "Server crashed" << endl;
        cout << e.what() << '\n';
        return;
    }
    catch (...) { cout << "Server exception"; }

}


void Server::start(){
    std::thread tmp(&Server::connect, this);
    t.swap(tmp);
}

/**
Server::Server() {
    printf("Create new Server\n");
    static int i = 5;
    printf("Server created\n");
}
**/

