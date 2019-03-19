#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"




//UI
#include <QtGui>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QGridLayout>
#include <QStyle>
#include <QDesktopWidget>


#include <opencv2/opencv.hpp>
#include "opencv2/opencv.hpp"


#include "ThermalWorker.h"
#include "StreamWorker.h"
#include "ThermalDataServer.h"

//QtImage
#include "CVImageWidget.h"
#include <QObject>

#include "Server.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>









/*#include <iostream>

// OpenCV





//threading
#include <thread>

// Server

#include "Server.h"







*/
// New Optris stuff

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>

// Optris device interface
#include "IRDeviceUVC.h"

// Optris imager interface
#include "IRImager.h"

// Optris frame rate counter
#include "FramerateCounter.h"

// Optris logging interface
#include "IRLogger.h"

// Class wrapping callback routines
#include "IRImagerHandler.h"

// Graphical User Interface
#include "IRImagerGUI.h"

using namespace std;
using namespace evo;

CVImageWidget *imageWidget = NULL;

ImageBuilder *_iBuilder;


int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        cout << "usage: " << argv[0] << " <xml configuration file>" << endl;
        cout << argv[0] << "-stream <file>" << endl;
        return -1;
    }
    if (argc == 2 && argv[1] == std::string("-stream")) {
        cout << "usage: " << argv[0] << " <xml configuration file>" << endl;
        cout << argv[0] << "-stream <file>" << endl;
        return -1;
    }

    if (argc == 3) {
        if (argv[2] == std::string("-tutor")) {
            cout << "Enable tutor mode" << endl;
            Settings::getInstance().setDebug(true);
        }

        if (argv[1] == std::string("-stream")) {
            cout << "StreamMode: Streaming files from: "<<  argv[2] << endl;
            cout << argv << endl;
            Settings::getInstance().setIsStreaming(true);
            Settings::getInstance().setStreamFolder(argv[2]);
        }
    }


    /// Setup the Gui
    QApplication app(argc, argv);

    imageWidget = new CVImageWidget();
    MainWindow *window = new MainWindow(imageWidget);

    window->setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    window->size(),
                    qApp->desktop()->availableGeometry()));
    //window->move(0, -100);

    if(!Settings::getInstance().isStreaming()) {
        IRLogger::setVerbosity(IRLOG_ERROR, IRLOG_OFF);
        IRDeviceParams params;
        IRDeviceParamsReader::readXML(argv[1], params);
        IRDeviceUVC *dev = IRDeviceUVC::createInstance(NULL, params.serial, params.videoFormatIndex);

        // Initialize Optris image processing chain
        IRImager imager;

        if (imager.init(&params, dev->getFrequency(), dev->getWidth(), dev->getHeight())) {
            unsigned int w = imager.getWidth();
            unsigned int h = imager.getHeight();
            if (w == 0 || h == 0) {
                cout
                        << "Error: Image streams not available or wrongly configured. Check connection of camera and config file."
                        << endl;
                return -1;
            }

            cout << "Connected camera, serial: " << dev->getSerial()
                 << ", HW(Rev.): " << imager.getHWRevision()
                 << ", FW(Rev.): " << imager.getFWRevision() << endl;
            cout << "Thermal channel: " << w << "x" << h << "@" << params.framerate << "Hz" << endl;
            if (imager.hasBispectralTechnology())
                cout << "Visible channel: " << imager.getVisibleWidth() << "x" << imager.getVisibleHeight() << "@"
                     << params.framerate << "Hz" << endl;

            //IRImagerHandler handler(IRDeviceUVC *device, IRImager *imager);

            IRImagerHandler *handler = new IRImagerHandler(dev, &imager);

            if (dev->startStreaming() != 0) {
                cout << "Error occurred in starting stream ... aborting. You may need to reconnect the camera." << endl;
                exit(-1);
            }

            // Worker is grabing new frames, rendering them to the CV canvas,
            // and provide thermal data

            Worker *worker1 = new Worker(handler, imageWidget);
            thread *workerThread = new thread();
            std::thread tmp(&Worker::start, worker1);
            workerThread->swap(tmp);
            workerThread->detach();


            ThermalDataServer *thermalServer;
            thermalServer = new ThermalDataServer(worker1);

            thread *tdsThread = new thread();
            std::thread thermalServerThread(&ThermalDataServer::start, thermalServer);
            tdsThread->swap(thermalServerThread);
            tdsThread->detach();


            Server *s = new Server(worker1);
            s->start();

            /// Add some logic

            // changes in mix and max temperature for image render (color)
            QObject::connect(window, SIGNAL(minValueChanged(int)), worker1, SLOT(setMinScale(int)));
            QObject::connect(window, SIGNAL(maxValueChanged(int)), worker1, SLOT(setMaxScale(int)));

            //changes in the measurment line
            QObject::connect(imageWidget, SIGNAL(valueChanged(QPoint, QPoint)), thermalServer, SLOT(setTemperatureLine(QPoint, QPoint)));


            // some buttons:
            QObject::connect(window->recalibrate, SIGNAL(clicked()), worker1, SLOT(forceFlagEvent()));
            QObject::connect(window->exportData, SIGNAL(clicked()), thermalServer, SLOT(exportData()));

            //Timer for data recording.
            QObject::connect(window->timer, SIGNAL(timeout()), thermalServer, SLOT(exportData()));

            int ret = app.exec();
            cout << "Exiting application" << endl;
            delete dev;
            //raise(SIGTERM);
            dev->stopStreaming();
        }
    }
    // Replay Data
    else{
        //TODO: adapt ui.

        ThermalDataServer *thermalServer;
        thermalServer = new ThermalDataServer(nullptr);

        thread *tdsThread = new thread();
        std::thread thermalServerThread(&ThermalDataServer::start, thermalServer);
        tdsThread->swap(thermalServerThread);
        tdsThread->detach();

        int ret = app.exec();
        cout << "Exiting application" << endl;

    }

    return 1;
}