//
// Created by Pascal Knierim on 26/02/17.
//

#include "HandleThermalDataConnection.h"
#include <string>
#include <vector>
#include <iostream>
#include "QJsonDocument"
#include "QJsonObject"
#include <QPoint>
#include "ControlCommand.h"

HandleThermalDataConnection::HandleThermalDataConnection(int client_fd, ThermalDataServer *server) {
    _server = server;
    _client_fd = client_fd;
}


vector<string> *listdir(const char *dirname) {
    DIR *dp;
    dirent *d;
    vector<string> *vec = new vector<string>;

    dp = opendir(dirname);
    while((d = readdir(dp)) != NULL)
        vec->push_back(d->d_name);

    sort(vec->begin(), vec->end());

    vec->erase (vec->begin(),vec->begin()+2); // remove . and ..

    return vec;
}


vector<float> HandleThermalDataConnection::prepareRecordedData(int time) {
    vector<float> result;

    //set min and max // ToDo: make it dynamic
    result.push_back(Settings::getInstance().min);
    result.push_back(Settings::getInstance().max);

    if (_server->files == NULL) {
        std::cout << "files not loaded, yet" << std::endl;
        return result;
    }

    if (time >= _server->files->size()) {
        time = _server->files->size() - 1;
    }


    // prepare path to file
    string str = _server->files->at(time);
    char * cstr = new char [str.length()+1];
    std::strcpy (cstr, str.c_str());

    char path[256];
    strcpy ( path, Settings::getInstance().getStreamFolder());
    strcat ( path, cstr);

    std::ifstream       file(path);

    string                line;

    // read second line
    getline(file,line);
    getline(file,line);

    stringstream          lineStream(line);
    string                cell;

    while(getline(lineStream,cell, ','))
    {
        float tmp = strtof(cell.c_str(),NULL);
        result.push_back(tmp);
    }
    return result;
}



void HandleThermalDataConnection::start() {

    QObject::connect(this, SIGNAL(valueChanged(QPoint, QPoint)), _server, SLOT(setTemperatureLine(QPoint, QPoint)));


    while (1) {
        char buffer[1024];
        bzero(buffer, 1024);
        int n = read(_client_fd, buffer, 1023);
        if (n == 0) {
            break;
        }

        // Parse JSON file
        QJsonObject obj;
        QJsonDocument doc = QJsonDocument::fromJson(buffer);

        ControlCommand cmd;

        // check validity of the document
        if(!doc.isNull())
        {
            if(doc.isObject())
            {
                obj = (QJsonObject)doc.object();
                cmd.read(obj);
            }
            else
            {
                std::cout << "Document is not an object" << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid JSON...\n" << buffer << std::endl;

            {
                // legacy implementation
                // ToDo: Playback recording of stream
                if (buffer[0] == 'P') {

                    if(Settings::getInstance().isStreaming()) {
                        std::cout << TAG << "Start playback timer for recorded data." << std::endl;

                        //Start Time
                        _server->startSecond = time(NULL);
                        init = true;
                        //Update file list for streaming
                        _server->files = listdir(Settings::getInstance().getStreamFolder());

                    }else {
                        std::cout << TAG << "We are in live mode, you can not start playback." << std::endl;
                        // ToDo: raise error.
                    }
                }

                if (buffer[0] == 'E') {
                    cout << TAG << "Export data" << endl;
                    _server->exportData();
                }

                if (buffer[0] == 'S') {

                    vector<float> ar;
                    float temp = 0;

                    if(Settings::getInstance().isStreaming()) {
                        std::cout << TAG << "Send Replay Data!" << _client_fd << std::endl;

                        elapsedSeconds = difftime(time(NULL), _server->startSecond);
                        ar = prepareRecordedData(elapsedSeconds);

                    } else{
                        std::cout << TAG << "Send Live Data!" << _client_fd << std::endl;
                        ar.push_back(_server->worker->getMinScale());
                        ar.push_back(_server->worker->getMaxScale());

                        for (auto &point : _server->line) // access by reference to avoid copying
                        {
                            temp = _server->worker->getTemperatureAt(point.x(),point.y());
                            ar.push_back(temp);
                        }
                    }
                    Send(ar, _client_fd);
                }

                if (buffer[0] == 'M') {
                    send(_client_fd, &(_server->maxChange), sizeof(float), 0);
                }
            }
        }

        //Handle JSON files on command.
        if(cmd.command == "SetLine"){
            _server->worker->setMinMaxScale(cmd.min,cmd.max);
            QPoint firstPoint(cmd.left,cmd.top);
            QPoint lastPoint(cmd.right,cmd.top);
            emit valueChanged(firstPoint,lastPoint);
            //ToDo: very dirty coding.
            _server->worker->mImageWidget->firstPoint = firstPoint;
            _server->worker->mImageWidget->lastPoint = lastPoint;
        }





    }
    close(_client_fd);
}