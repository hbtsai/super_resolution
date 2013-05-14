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

using namespace cv;

void cyclic_upscale(IplImage* img, float scale)
{

}

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
	char srcFile[256]={};
	printf("Processing a %dx%d image with %d channels\n",height,width,channels); 
	float scale=5.0;
	
	IplImage* dst = cvCreateImage(cvSize(width*scale, height*scale), IPL_DEPTH_8U, 3);
	
	cvResize(img, dst,  CV_INTER_CUBIC);
	
	IplImage* dst2=cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	
	cvResize(dst, dst2,  CV_INTER_CUBIC);
	
	IplImage* diffImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	cvSub(img, dst2, diffImg, NULL);

	IplImage* diff = cvCreateImage(cvSize(width*scale, height*scale), IPL_DEPTH_8U, 3);
	cvResize(diffImg, diff, CV_INTER_CUBIC);
	

	IplImage* result = cvCreateImage(cvSize(width*scale, height*scale), IPL_DEPTH_8U, 3);
	cvAdd(diff, dst, result, NULL);
	
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("bicubic_und", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
	
	cvShowImage("original", img);
	cvShowImage("bicubic_und", dst2);
	cvShowImage("result", result);
	cvWaitKey(0);
	
	cvDestroyWindow("original");
	cvDestroyWindow("bicubic_und");
	cvDestroyWindow("result");

	cvReleaseImage(&img);
	cvReleaseImage(&dst);
	cvReleaseImage(&dst2);
	cvReleaseImage(&diffImg);
	return 0;
}
