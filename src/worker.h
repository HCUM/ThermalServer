//
// Created by Pascal Knierim on 02/02/17.
//

#ifndef THERMALVISUALIZATION_WORKER_H
#define THERMALVISUALIZATION_WORKER_H

#include "PIImager.h"
#include "ImageBuilder.h"

class worker {
public:
    worker(optris::PIImager* _imager, unsigned char* bufferRaw);
    void start();

private:
    unsigned char* bufferRaw;
    optris::PIImager* _imager;

};


#endif //THERMALVISUALIZATION_WORKER_H
