//
// Created by Pascal Knierim on 02/02/17.
//

#include "worker.h"



worker::worker(optris::PIImager* i, unsigned char* b){
    _imager = i;
    bufferRaw = b;
}


void worker::start(){

    while (1){//_viewer.isAlive()) {
        //Render image
        _imager->getFrame(bufferRaw);
        _imager->process(bufferRaw);
        _imager->releaseFrame();

    }
}