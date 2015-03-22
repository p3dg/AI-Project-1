#ifndef _CS440P0_H
#define _CS440P0_H

#include <cv.h>
#include "Image.h"

class CS440P0
{
public:
	CS440P0():
	myChar(0)
	{
		//make a new window where we can show our processed results
		namedWindow("processed",1);
		
	}
	


/*
	bool cvtFrame(Mat& frame)
	{
		cvtColor(frame,converted,CV_BGR2GRAY);
		imshow("converted",converted);

		return true;
	}
*/
	/*bool doWork(Mat& frame)
	{
		//cvtColor(frame,processed.getImage(),CV_BGR2HSV);
		for(int x=0; x<200; x++)
				{
					for(int y=0; y<200; y++)
					{
						if(myChar == 'r')
							processed.set(x,y,255,0,0);
						if(myChar == 'g')
							processed.set(x,y,0,255,0);
						if(myChar == 'b')
							processed.set(x,y,0,0,255);
					}
				}
		imshow("processed",processed.getImage());

		return true;
	}*/

	
	bool doWork(Mat& frame)
	{

		processed(frame);
		//cvtColor(frame,processed.getImage(),CV_BGR2HSV);
		int sizex = processed.getWidth();
		int sizey = processed.getHeight();

		//This code block makes a binary image filtering out a known range of skin tones (makes it black)
		//calculate moment of blob to get position
		//calculate area so we can differentiate actions
		//
		int pixelcnt = 0;
		for(int x=0; x<sizex; x++)
		{
			for(int y=0; y<sizey; y++)
			{
				if(processed.getR(x,y)>220 && processed.getR(x,y)<255 &&
				   processed.getG(x,y)>170 && processed.getG(x,y)<210 &&
				   processed.getB(x,y)>180 && processed.getB(x,y)<200)
				{
					processed.set(x,y,0,0,0);
					pixelcnt++;
				}
				else
					processed.set(x,y,255,255,255);

			}
		}
		//int area = pixelcnt;
		//printf("area (%d,%d)\n", area);


		//CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
       //cvMoments(processed, moments, 1);

        // The moment values
       // double moment10 = cvGetSpatialMoment(moments, 1, 0);
        //double moment01 = cvGetSpatialMoment(moments, 0, 1);
        //double area = cvGetCentralMoment(moments, 0, 0);

		//show the processed image
		imshow("processed", processed.getImage());
		cvWaitKey(30);

		return true;
	}

	void setKey(char c)
	{
		myChar = c;
	}
	//Mat converted;
	Image processed;
	char myChar;

};

#endif