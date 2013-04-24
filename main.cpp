////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file, inverts it, and displays the result. 
//
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "libimage.h"

#if defined(_DEBUG)
#define dprintf(fmt, ...) printf("%s():%d "fmt,__func__,__LINE__,##__VA_ARGS__)
#else
#define dprintf(fmt, ...)
#endif

int main(int argc, char *argv[])
{
  IplImage* img = 0; 
  int height,width,step,channels;
  uchar *data;
  uchar *dst_data;
  int i,j,k;

  if(argc<2){
    printf("Usage: main <image-file-name>\n\7");
    exit(0);
  }

  // load an image  
  img=cvLoadImage(argv[1]);
  if(!img){
    printf("Could not load image file: %s\n",argv[1]);
    exit(0);
  }

  // get the image data
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
char dstFile[256]={};
  printf("Processing a %dx%d image with %d channels\n",height,width,channels); 


  IplImage *dst = NULL;
  dst = bilinear(img, width*5, height*5);
  sprintf(dstFile, "bl_%s", argv[1]);
  cvSaveImage(dstFile,dst);

  cvReleaseImage(&dst);
  dst = NULL;

  dst = bilinear1(img, width*5, height*5);
  sprintf(dstFile, "bl1_%s", argv[1]);
  cvSaveImage(dstFile,dst);

  cvReleaseImage(&dst);
  dst = NULL;

  dst = nearRest(img, width*5, height*5);
  sprintf(dstFile, "nr_%s", argv[1]);
  cvSaveImage(dstFile,dst);

//  cvReleaseImage(&img );
  cvReleaseImage(&dst );
  return 0;
}
