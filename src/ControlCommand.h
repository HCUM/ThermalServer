//
// Created by Pascal Knierim on 19.03.19.
//

#ifndef THERMALEXPERIMENTSERVER_CALIBRATIONDATA_H
#define THERMALEXPERIMENTSERVER_CALIBRATIONDATA_H


#include "QJsonDocument"
#include "QJsonObject"

class ControlCommand {

public:
    void read(const QJsonObject &json);

    //the command
    QString command;

    // definition of height, left and right end point of measurement line
    int top, left, right;

    // definition of min and max of thermal image and plot
    int min, max;
};



#endif //THERMALEXPERIMENTSERVER_CALIBRATIONDATA_H