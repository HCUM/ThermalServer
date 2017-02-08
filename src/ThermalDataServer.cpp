//
// Created by Pascal Knierim on 07/02/17.
//

#include "ThermalDataServer.h"
#include <QPoint>
#include "math.h"

typedef std::int64_t size_type;






std::vector<QPoint> line;

void ThermalDataServer::setTemperatureLine(QPoint start, QPoint end)
{
    std::cout << "TRIGGERED" << start.x()<< std::endl;
    std::cout << "TMP" << _pBuilder->getTemperatureAt(start.x()/2,start.y()/2) << std::endl;


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

ThermalDataServer::ThermalDataServer(optris::ImageBuilder *pBuilder) {
    _pBuilder = pBuilder;

    // Before using hint you have to make sure that the data structure is empty
    memset(& hints, 0, sizeof hints);
    // Set the attribute for hint
    hints.ai_family = AF_UNSPEC; // We don't care V4 AF_INET or 6 AF_INET6
    hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM
    hints.ai_flags = AI_PASSIVE;

    // Fill the res data structure and make sure that the results make sense.
    status = getaddrinfo(NULL, "8888" , &hints, &res);
    if(status != 0)
    {
        fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
    }

    // Create Socket and check if error occured afterwards
    listner = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
    if(listner < 0 )
    {
        fprintf(stderr,"socket error: %s\n",gai_strerror(status));
    }

    // Bind the socket to the address of my local machine and port number
    status = bind(listner, res->ai_addr, res->ai_addrlen);
    if(status < 0)
    {
        fprintf(stderr,"bind: %s\n",gai_strerror(status));
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
}

void ThermalDataServer::Send(std::vector<float>& vec, int sock )
{
    cout << "send shit! now " << "\n";
    const size_type sz = vec.size();
    float* t = &(vec.front());
    send( sock, t , sz * sizeof( float ) ,0);
}

void ThermalDataServer::start() {



    printf("I am now accepting connections ...\n");

    while(1){
        // Accept a new connection and return back the socket desciptor
        new_conn_fd = accept(listner, (struct sockaddr *) & client_addr, &addr_size);
        if(new_conn_fd < 0)
        {
            fprintf(stderr,"accept: %s\n",gai_strerror(new_conn_fd));
            continue;
        }

        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s);
        printf("I am now connected to %s \n",s);


        std::vector<float> ar;

        float temp =0;
        for (auto &point : line) // access by reference to avoid copying
        {
            temp = _pBuilder->getTemperatureAt(point.x(),point.y());
            ar.push_back(temp);
            //cout << "x: " << point.x() << " y: " << point.y()<< " temp: " << temp << endl;
        }


        Send(ar, new_conn_fd );


        if(status == -1)
        {
            close(new_conn_fd);
            _exit(4);
        }

    }
    // Close the socket before we finish
    //close(new_conn_fd);

    //return 0;


}


void * ThermalDataServer::get_in_addr(struct sockaddr * sa)
{
    if(sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}