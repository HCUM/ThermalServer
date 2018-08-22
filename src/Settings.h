//
// Created by Pascal Knierim on 17/08/17.
//

#ifndef THERMALEXPERIMENTSERVER_SETTINGS_H
#define THERMALEXPERIMENTSERVER_SETTINGS_H

#include <stdio.h>

class Settings {
public:
    static Settings &getInstance() {
        static Settings instance;
        return instance;
    }

    float tubelength = 212;
    bool onlyHorizontal = true;
    int min = 20;
    int max = 60;

private:
    Settings() {}

    bool mDebug = false;
    bool mStream = false;
    char *mFolderName = nullptr;

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

    void setStreamFolder(char *foldername){
        int l = strlen(foldername);
        if (foldername[l-1] == '/'){
            mFolderName = foldername;}
        else{
            mFolderName = static_cast<char *>( malloc(l+2));
            strcpy(mFolderName, foldername);
            strcat(mFolderName, "/");
        }
    }

    //Getter
    bool getDebug() {
        return mDebug;
    };

    bool isStreaming(){
        return mStream;
    }

    char* getStreamFolder(){
        return mFolderName;
    }
};


#endif //THERMALEXPERIMENTSERVER_SETTINGS_H
