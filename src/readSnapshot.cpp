#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

/**
 * Optris device interface
 */
#include "PIImager.h"
#include "PIImagerSnapshot.h"

/**
 * Visualization
 */
#include "Obvious2D.h"
#include "ImageBuilder.h"

using namespace std;
using namespace optris;

#define WIDTH    960
#define HEIGHT   600

obvious::Obvious2D _viewer(WIDTH, HEIGHT, "Optris Imager Snapshot example");
unsigned char* _bufferThermal = NULL;

optris::ImageBuilder* _iBuilder;


struct timespec timeOut,remains;



void cbOnThermalFrame(unsigned short* image, unsigned int w, unsigned int h)
{
  _iBuilder->setData(w, h, image);
  if(_bufferThermal==NULL)
    _bufferThermal = new unsigned char[_iBuilder->getStride() * h * 3];
  _iBuilder->convertTemperatureToPaletteImage(_bufferThermal);
  _viewer.draw(_bufferThermal, _iBuilder->getStride(), h, 3);
}

int main (int argc, char* argv[])
{
  if(argc!=2)
  {
    cout << "usage: " << argv[0] << " <png file>" << endl;
    return -1;
  }

  const char* filename = argv[1];

  int sizeEnergy = 1000000;
  char* energyData = new char[sizeEnergy];

  int sizeMeta = 1000;
  char* metaData = new char[sizeMeta];

  PIImagerSnapshot::readSnapshot(argv[1], energyData, &sizeEnergy, metaData, &sizeMeta);

  if(sizeEnergy>0 && sizeMeta>0)
  {
    _iBuilder = new ImageBuilder;
    _iBuilder->setPaletteScalingMethod(eMinMax);
    PIImagerSnapshot* imager = new PIImagerSnapshot(energyData, sizeEnergy, metaData);
    imager->setFrameCallback(cbOnThermalFrame);

    while(_viewer.isAlive())
    {
      imager->repeatFrame();
      //nanosleep(250000);

timeOut.tv_sec = 0;
timeOut.tv_nsec = 250; /* 50 milliseconds */

nanosleep(&timeOut, &remains);
    }
    delete imager;
    if(_bufferThermal) delete [] _bufferThermal;
    delete _iBuilder;
  }
  else
  {
    cout << "Format not valid" << endl;
  }

  delete [] energyData;
  delete [] metaData;

  cout << "Exiting application" << endl;
}
