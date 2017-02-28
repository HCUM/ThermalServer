#include <QDialog>
#include <QApplication>
#include <QMainWindow>

#include <iostream>

/**
 * Optris device interface
 */
#include "PIImager.h"

/**
 * Optris image converter
 */
#include "ImageBuilder.h"

/**
 * Visualization
 */
#include "Obvious2D.h"

/**
 * OpenCV
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/**
 * QtImage
 */
#include <CVImageWidget.h>
#include <QObject>


//threading
#include <thread>

/*
 * Server
 */
#include "Server.h"
#include "worker.h"
#include "ThermalDataServer.h"
#include "mainwindow.h"


//UI
#include <QtGui>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QGridLayout>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>


#include <QApplication>


using namespace std;
using namespace optris;
using namespace cv;

#define WIDTH    960
#define HEIGHT   600

//obvious::Obvious2D _viewer(WIDTH, HEIGHT, "Optris Imager example");

unsigned char* _bufferThermal = NULL;
unsigned char* _bufferVisible = NULL;
optris::PIImager* _imager;
optris::ImageBuilder* _iBuilder;

bool _showVisibleChannel = false;
bool record = false;
static int cnt = 0;
CVImageWidget* imageWidget = NULL;


Server* server = NULL;
unsigned char* bufferRaw;


//ToDo: Write a Header file
int serializePPM(const char* filename, void* buffer, unsigned int width, unsigned int height, bool increasingOrder);


cv::Mat *image;





void drawMeasurementInfo(unsigned int w, unsigned int h, unsigned int x, unsigned int y, float value)
{
  char text[20];



    sprintf(text, "%2.1f", value);
  unsigned int xOffset = w/20;
  unsigned int yOffset = h/20;

  if(x>xOffset)
    x -= xOffset;

  if(y<h-yOffset)
    y += yOffset;
  else
    y -= yOffset/2;

  //_viewer.addText(text, WIDTH * x/w, HEIGHT - HEIGHT * y/h);
}

void drawHelp()
{
  char text1[] = "p: Switch palette";
  char text2[] = "t: Toggle thermal/visible channel";
 // _viewer.addText(text1, WIDTH - 320, HEIGHT - 20);
  //if(_imager->hasBispectralTechnology()) _viewer.addText(text2, WIDTH - 320, HEIGHT - 40);
}

void cbOnThermalFrame(unsigned short* image, unsigned int w, unsigned int h)
{
  if(!_showVisibleChannel)
  {
    _iBuilder->setData(w, h, image);
    if(_bufferThermal==NULL)
      _bufferThermal = new unsigned char[_iBuilder->getStride() * h * 3];
    _iBuilder->convertTemperatureToPaletteImage(_bufferThermal);

    //_viewer.clearText();
    drawHelp();

    int radius = 3;
    ExtremalRegion minRegion;
    ExtremalRegion maxRegion;
    _iBuilder->getMinMaxRegion(radius, &minRegion, &maxRegion);
    //drawMeasurementInfo(w, h, (minRegion.u1+minRegion.u2)/2, (minRegion.v1+minRegion.v2)/2, minRegion.t);
    //drawMeasurementInfo(w, h, (maxRegion.u1+maxRegion.u2)/2, (maxRegion.v1+maxRegion.v2)/2, maxRegion.t);
    //drawMeasurementInfo(w, h, (minRegion.u1+minRegion.u2)/2, (minRegion.v1+minRegion.v2)/2, minRegion.t);

    //_viewer.draw(_bufferThermal, _iBuilder->getStride(), h, 3);



      /**
      static int frame = 0;
    if((frame++)%80==0)
    {
      cout << "Frame rate: " << _imager->getAverageFramerate() << " fps" << endl;
    }**/
  }

    char filename[] = "/home/pascalknierim/Desktop/tmp.ppm";
    if(record){
        //Saving a image
        sprintf(filename, "/home/mci/Desktop/recording/image%05d.ppm", cnt++);
    }

    //ToDo: use buffer directly
    serializePPM(filename, _bufferThermal, _iBuilder->getStride(), h, false);
}


int serializePPM(const char* filename, void* buffer, unsigned int width, unsigned int height, bool increasingOrder)
{

    int rowInc=0;
    int rowStart=0;
    unsigned int i=0;
    unsigned int j=0;

    if (increasingOrder)
    {
        rowStart = (width * 3 * (height - 1));
        rowInc = -width * 3;
    }
    //FILE *pFile;

    //pFile = fopen(filename, "wb");
    //if (pFile == NULL) return 0;

    // Write header
    //fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    //fprintf(pFile, "P6 %d %d 255", width, height);


    unsigned char* buf = (unsigned char*) buffer;


    //cout << "Tmp" << _iBuilder->getTemperatureAt( 191,  144)<< endl;





    // Write pixel data
    buf += rowStart;
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            Scalar color = Scalar(buf[0],buf[1],buf[2]);

            Vec3b intensity;
            intensity.val[2]= buf[0];
            intensity.val[1]=buf[1];
            intensity.val[0]= buf[2];

            image->at<Vec3b>(i,j) = intensity;

            //image.at<short>(i,j) = buf[0];
            //putc(buf[0], pFile); /* b */
            //putc(buf[1], pFile); /* g */
            //putc(buf[2], pFile); /* r */
            buf += 3;



        }
        /* Move to next line */
        buf += 2 * rowInc;
    }


    //class data members of ints
    int total = image->cols * image->rows;
    std::vector<uchar> vec(image->data, image->data + total);

    std::vector<uchar> mybuf;
    imencode( ".jpg", image->clone(), mybuf);
    server->buf = mybuf;



    try {
        imageWidget->showImage(image->clone());
    }catch (...){
        cout <<  "Could not render image" << endl ;
    }

    return 1;
}


void cbPalette()
{
  EnumOptrisColoringPalette palette = _iBuilder->getPalette();
  unsigned int val = (unsigned int)palette;
  val++;
  if(val>eAlarmRed) val = 1;
  _iBuilder->setPalette((EnumOptrisColoringPalette) val);
}

void cbChannel()
{
  if(_imager->hasBispectralTechnology()) _showVisibleChannel = !_showVisibleChannel;
}



int main (int argc, char* argv[])
{
  if(argc!=2)
  {
    cout << "usage: " << argv[0] << " <xml configuration file>" << endl;
    return -1;
  }

  /**
   * Initialize Optris image processing chain
   */
  _imager = new PIImager(argv[1]);
  if(_imager->getWidth()==0 || _imager->getHeight()==0)
  {
    cout << "Error: Image streams not available or wrongly configured. Check connection of camera and config file." << endl;
    return -1;
  }



    image = new Mat(_imager->getHeight(), _imager->getWidth(), CV_8UC3);


    QApplication app(argc, argv);

    imageWidget = new CVImageWidget();

    MainWindow *window = new MainWindow(imageWidget);


    cout << "Thermal channel: " << _imager->getWidth() << "x" << _imager->getHeight() << "@" << _imager->getFramerate() << "Hz" << endl;

  bufferRaw = new unsigned char[_imager->getRawBufferSize()];

  if(_imager->hasBispectralTechnology())
  {
    _bufferVisible        = new unsigned char[_imager->getVisibleWidth() * _imager->getVisibleHeight() * 3];
    cout << "Visible channel: " << _imager->getVisibleWidth() << "x" << _imager->getVisibleHeight() << "@" << _imager->getFramerate() << "Hz" << endl;
  }
  _imager->setFrameCallback(cbOnThermalFrame);
  //_imager->setVisibleFrameCallback(cbOnVisibleFrame);
    // _viewer.registerKeyboardCallback('p', cbPalette);
  //_viewer.registerKeyboardCallback('t', cbChannel);


    _iBuilder = new ImageBuilder();
    _iBuilder->setPaletteScalingMethod(eManual);
    _iBuilder->setManualTemperatureRange(15.0f, 40.0f);




    server = new Server();
    server->start();



  /**
   * Enter endless loop in order to pass raw data to Optris image processing library.
   * Processed data are supported by the frame callback function.
   */


    _imager->startStreaming();


    worker* worker1 = new worker(_imager, bufferRaw);
    thread* workerThread = new thread();
    std::thread tmp(&worker::start, worker1);
    workerThread->swap(tmp);
    workerThread->detach();


    ThermalDataServer* tds;
    tds = new ThermalDataServer(_iBuilder);

    thread* tdsThread = new thread();
    std::thread tdsT(&ThermalDataServer::start, tds);
    tdsThread->swap(tdsT);
    tdsThread->detach();

    // Add some logic
    QObject::connect(imageWidget, SIGNAL(valueChanged(QPoint, QPoint)), tds, SLOT(setTemperatureLine(QPoint, QPoint)));
    QObject::connect(tds, SIGNAL(maxTempChanged(float)), window, SLOT(setMaxChangeValue(float)));
    QObject::connect(window, SIGNAL (minValueChanged(int)), tds, SLOT(setMinVisualisationValue(int)));
    QObject::connect(window, SIGNAL (maxValueChanged(int)), tds, SLOT(setMaxVisualisationValue(int)));

    int ret = app.exec();

    printf("Exiting application\n");
    delete [] bufferRaw;
    if(_bufferThermal) delete [] _bufferThermal;
    if(_bufferVisible) delete [] _bufferVisible;
    delete _imager;
    delete _iBuilder;

    return ret;
}