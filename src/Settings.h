//
// Created by Pascal Knierim on 17/08/17.
//

#ifndef THERMALEXPERIMENTSERVER_SETTINGS_H
#define THERMALEXPERIMENTSERVER_SETTINGS_H

class Settings {
public:
    static Settings &getInstance() {
        static Settings instance;
        return instance;
    }

    float tubelength = 212;
    bool onlyHorizontal = true;
    int min = 15;
    int max = 80;

private:
    Settings() {}

    bool mDebug = false;
    bool mStream = false;
    const char *mFilename = nullptr;

public:
    Settings(Settings const &) = delete;

    void operator=(Settings const &)  = delete;

    // Setter
    void setDebug(bool debug) {
        mDebug = debug;
    };

    void setIsStreaming(bool stream) {
        mStream = stream;
    };

    void  setStreamFilename(char *filename){
        mFilename = filename;
    }

    //Getter
    bool getDebug() {
        return mDebug;
    };

    bool isStreaming(){
        return mStream;
    }

    const char* getStreamFilename(){
        return mFilename;
    }
};


#endif //THERMALEXPERIMENTSERVER_SETTINGS_H
