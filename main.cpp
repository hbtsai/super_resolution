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
  dprintf("src widthStep=%d\n", step);
  dprintf("src channels=%d\n", channels);


  IplImage *dst = cvCreateImage(cvSize(img->width*5, img->height*5), img->depth, img->nChannels);
  dprintf("dst widthStep=%d\n", dst->widthStep);
  dprintf("dst channels=%d\n", dst->nChannels);

  dst_data = (uchar *)dst->imageData;
  /*
  cvResize(img, dst, cv::INTER_CUBIC);
	sprintf(dstFile, "cubic_%s", argv[1]);
  cvSaveImage(dstFile,dst);

  cvResize(img, dst, cv::INTER_NEAREST);
	sprintf(dstFile, "nearest_%s", argv[1]);
  cvSaveImage(dstFile,dst);

  cvResize(img, dst, cv::INTER_LANCZOS4);
	sprintf(dstFile, "lanc_%s", argv[1]);
  cvSaveImage(dstFile,dst);

  for(i=0; i<height*5; i++)
  {
	  for(j=0; j<width*5; j++)
	  {
		  for(k=0; k<channels; k++)
		  {
			  dprintf(" %#x ", data[i*step%5+j*channels%5+k]);
			dst_data[i*(dst->widthStep)+j*channels+k]=data[i*step%5+j*channels%5+k];
		  }
	  }
  }
  */
  int count=0;

  for(i=0;i<height*5;i++) 
  {
	  for(j=0;j<width*5;j++)
	  {
		  for(k=0;k<channels;k++)
		  {
			dst_data[i*dst->widthStep+j*channels+k]=data[i/5*step+j/5*channels+k];
		  }
	  }
  }
	sprintf(dstFile, "sr_%s", argv[1]);
  cvSaveImage(dstFile,dst);


  /*
  // create a window
  cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("mainWin", 100, 100);

  // invert the image
  for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
    data[i*step+j*channels+k]=255-data[i*step+j*channels+k];

  // show the image
  cvShowImage("mainWin", dst );

  // wait for a key
  cvWaitKey(0);
	*/
  // release the image
  cvReleaseImage(&img );
  cvReleaseImage(&dst );
  return 0;
}
