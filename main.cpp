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

int main(int argc, char *argv[])
{
	Mat img;
	int height,width,step,channels;
	uchar *data;
	uchar *dst_data;
	int scale=1;
	int delta=0;
	int ddepth=CV_16S;
	int num_of_lrs=1;
	Mat img_lr[num_of_lrs];
	
	if(argc<2){
	  printf("Usage: main <image-file-name>\n\7");
	  exit(0);
	}
	
	// load an image  
	img=imread(argv[1]);
	if(!img.data)
	{
	  printf("Could not load image file: %s\n",argv[1]);
	  exit(0);
	}


	float lr_scale=0;
	for(int i=0; i<num_of_lrs; i++)
	{
//		lr_scale=1/pow(1.25, i);
		resize(img, img_lr[i], img.size()*0.5, CV_INTER_CUBIC);
	}

//	resize(img, img_lr[0], img.size()*5, CV_INTER_CUBIC);
	
	// get the image data
	namedWindow("original", CV_WINDOW_AUTOSIZE);
	imshow("original", img);

	namedWindow("img_lr", CV_WINDOW_AUTOSIZE);
	imshow("img_lr", img_lr[0]);

	waitKey(0);
			
	return 0;
}
