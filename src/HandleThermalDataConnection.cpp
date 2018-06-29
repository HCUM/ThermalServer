//
// Created by Pascal Knierim on 26/02/17.
//

#include "HandleThermalDataConnection.h"
#include <iomanip>


#define DEBUG  true

HandleThermalDataConnection::HandleThermalDataConnection(int client_fd, ThermalDataServer *server) {
    _server = server;
    _client_fd = client_fd;
}




vector<float> getNextLineAndSplitIntoTokens(std::istream& str)
{
    vector<float>   result;
    string                line;
    getline(str,line);

    stringstream          lineStream(line);
    string                cell;

    //set min and max // ToDo: make it dynamic
    result.push_back(10);
    result.push_back(25);
    while(getline(lineStream,cell, ','))
    {


        cout << cell.c_str() << "cell char " << endl;
        float tmp = strtof(cell.c_str(),NULL);
        cout <<"convert: " << cell << " and add: " << tmp << endl;

        cout.setf(ios::fixed);
        cout << setprecision(3) << "yeah" << tmp << endl;

        result.push_back(tmp);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        //result.push_back("");
    }
    return result;
}



void HandleThermalDataConnection::start() {
    while (1) {
        char buffer[256];
        bzero(buffer, 256);
        int n = read(_client_fd, buffer, 255);
        if (n == 0) {
            break;
        }


        if (buffer[0] == 'E') {
            cout << TAG << "Export data" << endl;
            _server->exportData();
            //Send confirmation
            //send(_client_fd, "sucsess", 7*sizeof(char), 0);
        }

        if (buffer[0] == 'S') {
#if DEBUG
            cout << TAG << "send thermal data" << endl;
#endif
            vector<float> ar;
            float temp = 0;



            // check if we replay data....

            if(Settings::getInstance().isStreaming()) {
                std::cout << TAG << "Send Replay Data !!!!" << _client_fd << std::endl;

                std::ifstream       file(Settings::getInstance().getStreamFilename());

                //ifstream file(Settings::getInstance().getStreamFilename());
                ar = getNextLineAndSplitIntoTokens(file);


            } else{
                // Save min and max to array
                std::cout << TAG << "Send Live Data !!!!" << _client_fd << std::endl;

                ar.push_back(_server->worker->getMinScale());
                ar.push_back(_server->worker->getMaxScale());

                for (auto &point : _server->line) // access by reference to avoid copying
                {
                    temp = _server->worker->getTemperatureAt(point.x(),
                                                             point.y()); // -1 ;//_server->_pBuilder->getTemperatureAt(point.x(), point.y());
                    ar.push_back(temp);
                }
            }
            Send(ar, _client_fd);
        }

        if (buffer[0] == 'M') {
            send(_client_fd, &(_server->maxChange), sizeof(float), 0);
        }


    }
    close(_client_fd);
#if DEBUG
    std::cout << TAG << "connection closed; ConnectionID: " << _client_fd << std::endl;
#endif
}