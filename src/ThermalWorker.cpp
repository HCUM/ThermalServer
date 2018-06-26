//
// Created by Pascal Knierim on 02/02/17.
//

#include "ThermalWorker.h"

using namespace cv;
using namespace evo;


Worker::Worker(IRImagerHandler *handler, CVImageWidget *imageWidget) {
    mHandler = handler;
    mImageWidget = imageWidget;

    minScale = Settings::getInstance().min;
    maxScale = Settings::getInstance().max;
}

float Worker::getTemperatureAt(int x, int y) {
    return iBuilder.getTemperatureAt(x, y);
}

void Worker::setMinScale(int min) {
    minScale = (float) min;
}

void Worker::setMaxScale(int max) {
    maxScale = (float) max;
}

void Worker::setMinMaxScale(int min, int max) {
    minScale = (float) min;
    maxScale = (float) max;
}

void Worker::forceFlagEvent() {
    mHandler->forceFlagEvent();
}

void Worker::start() {

    while (1) {
        if (mHandler->checkForNewFrame()) {

            unsigned int w = mHandler->getThermalWidth();
            unsigned int h = mHandler->getThermalHeight();

            if (w == 0 || h == 0) {
                continue;
            }

            img = mHandler->getThermalImage();

            if (thermal == NULL)
                thermal = new unsigned short[w * h];

            memcpy(thermal, img, w * h * sizeof(unsigned short));

            iBuilder.setPaletteScalingMethod(eManual);
            iBuilder.setManualTemperatureRange(minScale, maxScale);
            iBuilder.setPalette(evo::eIron);
            iBuilder.setData(w, h, thermal);

            if (!thermalImage)
                thermalImage = new unsigned char[iBuilder.getStride() * h * 3];

            iBuilder.convertTemperatureToPaletteImage(thermalImage, true);

            unsigned char *ptr = thermalImage;

            if (!image)
                image = new Mat(h, w, CV_8UC3);

            int rowInc = 0;
            int rowStart = 0;
            unsigned int i = 0;
            unsigned int j = 0;

            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {

                    Vec3b intensity;
                    float x = ((float) img[0]) / (float) 1800.0f * 255;

                    intensity.val[2] = ptr[0];
                    intensity.val[1] = ptr[1];
                    intensity.val[0] = ptr[2];

                    image->at<Vec3b>(i, j) = intensity;
                    img += 1;
                    ptr += 3;
                }
            }

            try {
                mImageWidget->showImage(*image);
            } catch (...) {
                std::cout << "Could not render image" << std::endl;
            }
        }
    }
}
