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
	Mat img, img_gray;
	Mat grad;
	int height,width,step,channels;
	uchar *data;
	uchar *dst_data;
	int scale=1;
	int delta=0;
	int ddepth=CV_16S;
	
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
	
	// get the image data

	Mat img_blur, img_sub;
	GaussianBlur(img, img_blur, Size(5, 5), 0, 0, BORDER_DEFAULT);

	cvtColor(img_blur, img_gray, CV_RGB2GRAY);

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	Scharr(img_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Scharr(img_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	subtract(img_gray, grad, img_sub);
	
	namedWindow("original", CV_WINDOW_AUTOSIZE);
	imshow("original", img);

	namedWindow("img_blur", CV_WINDOW_AUTOSIZE);
	imshow("img_blur", img_blur);

	namedWindow("grad", CV_WINDOW_AUTOSIZE);
	imshow("grad", grad);

	namedWindow("img_sub", CV_WINDOW_AUTOSIZE);
	imshow("img_sub", img_sub);
	waitKey(0);
			
	return 0;
}
