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
	int num_of_lrs=4;
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


	float lr_scale=1/1.25;
	for(int i=0; i<num_of_lrs; i++)
	{
		Size dsize(img.size().width*lr_scale, img.size().height*lr_scale);
//		lr_scale=1/pow(1.25, i);
		resize(img, img_lr[i], dsize, 0, 0,  CV_INTER_AREA);
		lr_scale/=1.25;
	}

//	resize(img, img_lr[0], img.size()*5, CV_INTER_CUBIC);
	
	// get the image data
	namedWindow("original", CV_WINDOW_AUTOSIZE);
	imshow("original", img);

	float hr_scale=5;

	Mat dst;
	Size dsize(img.size().width*hr_scale, img.size().height*hr_scale);
	resize(img, dst, dsize, 0, 0, CV_INTER_CUBIC);
	dst_data = dst.data;
	int p_size=5;
	int lr_size=p_size/hr_scale;
	Mat patch(p_size, p_size, 0);
	Mat lr_patch(lr_size, lr_size, 0);
	Mat patch_diff(lr_size, lr_size, 0);

	int sum_diff=0;
	int min_diff=INT_MAX;
	int min_x=0;
	int min_y=0;

	for (int x=0; x<dst.rows; x++)
	{
		for(int y=0; y<dst.cols; y++)
		{

			// find patch region of high resolution image
			for(int n_x=x; n_x<p_size; n_x++)
			{
				for(int n_y=y; n_y<p_size; n_y++)
				{
					patch.data[n_x*p_size+n_y]=dst.data[x*dst.rows+y];
					patch.data[n_x*p_size+n_y+1]=dst.data[x*dst.rows+y+1];
					patch.data[n_x*p_size+n_y+2]=dst.data[x*dst.rows+y+2];
				}
			}

			Size dsize(patch.size().width/hr_scale, patch.size().height/hr_scale);

			// find patch region on original image
			resize(patch, lr_patch, dsize, 0, 0, CV_INTER_AREA);

			for(int n_x=x; n_x<lr_size; n_x++)
			{
				for(int n_y=y; n_y<lr_size; n_y++)
				{
					lr_patch.data[n_x*lr_size+n_y]=img.data[x*dst.rows+y];
					lr_patch.data[n_x*lr_size+n_y+1]=img.data[x*dst.rows+y+1];
					lr_patch.data[n_x*lr_size+n_y+2]=img.data[x*dst.rows+y+2];
				}
			}


			// find region NN closest to patch 
			for(int i=0; i<num_of_lrs; i++)
			{
				for(int lr_x=0; lr_x<img_lr[i].size().width; lr_x++)
				{
					for(int lr_y=0; lr_y<img_lr[i].size().height; lr_y++)
					{
						Mat tmp_patch(lr_size, lr_size, 0);
						for(int n_x=x; n_x<lr_size; n_x++)
						{
							for(int n_y=y; n_y<lr_size; n_y++)
							{
								tmp_patch.data[n_x*lr_size+n_y]=  img_lr[i].data[lr_x*img_lr[i].rows+lr_y];
								tmp_patch.data[n_x*lr_size+n_y+1]=img_lr[i].data[lr_x*img_lr[i].rows+lr_y+1];
								tmp_patch.data[n_x*lr_size+n_y+2]=img_lr[i].data[lr_x*img_lr[i].rows+lr_y+2];
							}
						}
						absdiff(lr_patch, tmp_patch, patch_diff);
						for(int tmp=0; tmp<lr_size*lr_size*3; tmp++)
						{
							sum_diff += patch_diff.data[tmp];
						}
						//dprintf("sum_diff=%d min_diff=%d lr_x=%d,  lr_y=%d\n", sum_diff, min_diff, lr_x, lr_y);
						if(sum_diff < min_diff)
						{
						//dprintf("sum_diff=%d min_diff=%d lr_x=%d,  lr_y=%d\n", sum_diff, min_diff, lr_x, lr_y);
							if(lr_x==0)
								min_x=0;
							else
								min_x = lr_x/(lr_scale*(i+1));

							if(lr_y==0)
								min_y=0;
							else
								min_y = lr_y/(lr_scale*(i+1));
							min_diff = sum_diff;
						}
						sum_diff=0;
					}
				}
		
			}

			//dprintf("min_x=%d min_y=%d\n", min_x, min_y);

			// find corresponding patch on original image.
			for(int hr_x=x; hr_x<p_size; hr_x++)
			{
				for(int hr_y=y; hr_y<p_size; hr_y++)
				{
					dst.data[hr_x*dst.rows+hr_y]=  img.data[min_x*img.rows+min_y];
					dst.data[hr_x*dst.rows+hr_y+1]=img.data[min_x*img.rows+min_y+1];
					dst.data[hr_x*dst.rows+hr_y+2]=img.data[min_x*img.rows+min_y+2];
				}
			}
			min_diff=INT_MAX;

		}
	}


		namedWindow("img_hr", CV_WINDOW_AUTOSIZE);
		imshow("img_hr", dst);

	waitKey(0);
			
	return 0;
}
