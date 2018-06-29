//
// Created by Pascal Knierim on 26.06.18.
//

#ifndef THERMALEXPERIMENTSERVER_IWORKER_H
#define THERMALEXPERIMENTSERVER_IWORKER_H



class IWorker {

public:
    virtual ~IWorker() {}

    virtual void start();

    virtual float getTemperatureAt(int x, int y);

    virtual int getMinScale();

    virtual int getMaxScale();

};



#endif //THERMALEXPERIMENTSERVER_IWORKER_H
