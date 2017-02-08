#include <stdio.h>
#include <iostream>

/**
 * Optris interface
 */
#include "PIImager.h"

/**
 * Visualization
 */
#include "ImageBuilder.h"

using namespace std;
using namespace optris;

unsigned char* _bufferThermal = NULL;

optris::PIImager* _imager;
optris::ImageBuilder* _iBuilder;

int serializePPM(const char* szFilename, void* pvBuffer, unsigned int nWidth, unsigned int nHeight, bool bInc);

void cbOnThermalFrame(unsigned short* image, unsigned int w, unsigned int h)
{
  _iBuilder->setData(w, h, image);
  if(_bufferThermal==NULL)
    _bufferThermal = new unsigned char[_iBuilder->getStride() * h * 3];
  _iBuilder->convertTemperatureToPaletteImage(_bufferThermal);

  int radius = 3;
  ExtremalRegion minRegion;
  ExtremalRegion maxRegion;
  _iBuilder->getMinMaxRegion(radius, &minRegion, &maxRegion);

  static int frame = 0;
  if((frame++)%80==0)
  {
    cout << "Frame rate: " << _imager->getAverageFramerate() << " fps" << endl;
    char filename[64];
    static int cnt = 0;
    sprintf(filename, "/tmp/image%05d.ppm\0", cnt++);
    serializePPM(filename, _bufferThermal, _iBuilder->getStride(), h, false);
  }
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

  cout << "Thermal channel: " << _imager->getWidth() << "x" << _imager->getHeight() << "@" << _imager->getFramerate() << "Hz" << endl;

  unsigned char* bufferRaw = new unsigned char[_imager->getRawBufferSize()];

  _imager->setFrameCallback(cbOnThermalFrame);
  _iBuilder = new ImageBuilder();
  _iBuilder->setPaletteScalingMethod(eMinMax);
  _iBuilder->setManualTemperatureRange(25.0f, 40.0f);


  /**
   * Enter endless loop in order to pass raw data to Optris image processing library.
   * Processed data are supported by the frame callback function.
   */
  _imager->startStreaming();
  while(1)
  {
    _imager->getFrame(bufferRaw);
    _imager->process(bufferRaw);
    _imager->releaseFrame();
  }

  delete [] bufferRaw;
  if(_bufferThermal) delete [] _bufferThermal;

  delete _imager;
  delete _iBuilder;

  cout << "Exiting application" << endl;
}

int serializePPM(const char* szFilename, void* pvBuffer, unsigned int nWidth, unsigned int nHeight, bool bInc)
{

  int nRowInc=0;
  int nRowStart=0;
  unsigned int i=0;
  unsigned int j=0;

  // increasing order
  if (bInc)
  {
    nRowStart = (nWidth * 3 * (nHeight - 1));
    nRowInc = -nWidth * 3;
  }
  FILE *pFile;

  // Open file
  pFile = fopen(szFilename, "wb");
  if (pFile == NULL) return 0;

  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", nWidth, nHeight);

  // Write pixel data
  unsigned char* buffer = (unsigned char*) pvBuffer;

  /* Go to the end of the buffer */
  buffer += nRowStart;
  for (i=0; i<nHeight; i++)
  {
    for (j=0; j<nWidth; j++)
    {
      putc(buffer[0], pFile); /* b */
      putc(buffer[1], pFile); /* g */
      putc(buffer[2], pFile); /* r */
      buffer += 3;
    }
    /* Move to next line */
    buffer += 2 * nRowInc;
  }

  // Close file
  fclose(pFile);

  return 1;
}
