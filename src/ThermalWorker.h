//
// Created by Pascal Knierim on 02/02/17.
//

#ifndef THERMALVISUALIZATION_WORKER_H
#define THERMALVISUALIZATION_WORKER_H

#include <iostream>
// Class wrapping callback routines
#include "IRImagerHandler.h"


#include <opencv2/opencv.hpp>

#include "ImageBuilder.h"
#include "CVImageWidget.h"
#include "Settings.h"

#include <QObject>


using namespace evo;

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(IRImagerHandler *handler, CVImageWidget *imageWidget);

    void start();

    float getTemperatureAt(int x, int y);

    int getMinScale() { return minScale; };

    int getMaxScale() { return maxScale; };

    std::vector<uchar> mybuf;

    void test();

public
    slots:
    void setMinScale(int min);

    void setMaxScale(int max);

    void setMinMaxScale(int min, int max);

    void forceFlagEvent();

private:

    unsigned char *bufferRaw;
    IRImagerHandler *mHandler;
    unsigned short *_thermal;
    unsigned char *_imgThermal;
    EnumOptrisColoringPalette palette;
    CVImageWidget *mImageWidget;
    float minScale;
    float maxScale;

    ImageBuilder iBuilder;


    unsigned short *img = NULL;
    unsigned short *thermal = NULL;
    unsigned char *thermalImage = NULL;

    cv::Mat *image = NULL;
};


#endif //THERMALVISUALIZATION_WORKER_H
