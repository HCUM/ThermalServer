//
// Created by Pascal Knierim on 01/02/17.
//

#include <zconf.h>
#include "ConnectionHandler.h"
#include "Server.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <vector>
#include <ml.h>
#include <highgui.h>
#include <sys/signal.h>


using namespace std;
using namespace cv;

Server* myserver;


ConnectionHandler::ConnectionHandler(int _client_fd, Server* _server) {
    client_fd=_client_fd;
    myserver = _server;

    signal(SIGPIPE, SIG_IGN);


    char response[] = "HTTP/1.0 200 OK\r\n"
            "Cache-Control: no-cache\r\n"
            "Pragma: no-cache\r\n"
            "Connection: close\r\n"
            "Content-Type: multipart/x-mixed-replace; boundary=--BoundaryString\r\n\r\n";

    write(client_fd, response, sizeof(response) - 1); /*-1:'\0'*/
    cout << "send response to " << client_fd << endl;

}

int status;
void ConnectionHandler::handleConnection() {

    //char filename[] = "/home/pascalknierim/Desktop/tmp.ppm";
    Mat image;
    while(1){
        try
        {
            std::vector<uchar> buf = myserver->buf;
            uchar* sendData = &buf[0];

            char frame [256];
            sprintf(frame, "--BoundaryString\r\nContent-type: image/jpg\r\nContent-Length: %d\r\n\r\n", buf.size());
            size_t length = strlen(frame);

            write(client_fd, frame, length);
            write(client_fd, sendData, buf.size());

            sprintf(frame, "\r\n\r\n");
            length = strlen(frame);
            status  = write(client_fd, frame, length);

            usleep(33000);  // 30 fps

        }
        catch (exception& e)
        {
            cout << "ConnectionHandler crashed" << endl;
            cout << e.what() << '\n';
            return;
        }
        catch (...) { cout << "ConnectionHandler exception"; }


        if(status == -1){
            cout << "clean up sending thread" << endl;
            break;
        }
    }
}



