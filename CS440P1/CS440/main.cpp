#include <highgui.h>
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Image.h"
#include "CS440P0.h"
//Code skeleton and form assisted by the samples from the book
//("Learning OpenCV_ Computer Vision with the OpenCV Library"
//OReilly Media Copyright © 2008 Gary Bradski and Adrian Kaehler)
IplImage* findSkin(IplImage* image)
{
        //Convert to HSV image
        IplImage* imgHSV = cvCreateImage(cvGetSize(image), 8, 3);
        cvCvtColor(image, imgHSV, CV_BGR2HSV);
        IplImage* imgFiltered = cvCreateImage(cvGetSize(image), 8, 1);

        //Filter skin
        cvInRangeS(imgHSV, 
			cvScalar(0, 100, 100), 
			cvScalar(100, 255, 255), imgFiltered);
		
		//Erode image
		cvErode(imgFiltered, imgFiltered, 0, 0);

        return imgFiltered;
}

void paintSquare(int xStart, int xEnd, int yStart, int yEnd, int R, int G, int B, Image painting)
{
	for(int x=xStart; x<xEnd+1; x++)
	{
		for(int y=yStart; y<yEnd+1; y++)
		{
			painting.set(x,y,R,G,B);
		}
	}
}

double bound(double probability)
{
	if(probability < 0)
		probability = 0;
	if(probability > 1.0)
		probability = 1.0;

	return probability;
}

int main()
{
	CvCapture* capture = 0;
    capture = cvCaptureFromCAM(0);

	IplImage* framea = 0;
	framea = cvQueryFrame(capture);

	Image painting(framea->width, framea->height);

        //Capture Camera feed
        cvNamedWindow("video", 1);
        cvNamedWindow("thresh", 1);

        for(;;)
		{
			IplImage* frame = 0;
			frame = cvQueryFrame(capture);
			cvShowImage("video", frame);

                IplImage* skinFilter = findSkin(frame);
				cvShowImage("thresh", skinFilter);
        
                //Set memory space and initialize moments
				CvMoments *moments = new CvMoments();
                cvMoments(skinFilter, moments, 1);

                //Use the moment values to get the object's position
                double momenta = cvGetSpatialMoment(moments, 1, 0);
                double momentb = cvGetSpatialMoment(moments, 0, 1);
                double area = cvGetCentralMoment(moments, 0, 0);

                static int posX = 0;
                static int posY = 0;

                int lastX = posX;
                int lastY = posY;
                
				posX = momenta/area;
                posY = momentb/area;
				
				//Get object circularity
				double a = cvGetCentralMoment(moments, 2, 0);
				double b = cvGetCentralMoment(moments, 1, 1);
				double c = cvGetCentralMoment(moments, 0, 2);
				double d = sqrt(pow(b,2.0) + pow((a-c),2.0));
				double Emin = (a+c)/2 - (a-c)/2*(a-c)/d - b/2*b/d;
				double Emax = (a+c)/2 + (a-c)/2*(a-c)/d + b/2*b/d;
				double circRatio = Emin/Emax;
				
				//Values used for clarity in following code
				static int gesture = 1;
				static int FIST = 1;
				static int PALM = 2;
				static int EDGE = 3; //edge of hand, i.e. palm turned sideways

				//Get the probability of each gesture for display, although the algorithm for
				//identifying gestures will not use these.
				//Empirical average values for circRatio: ~0.6 for fist, ~0.2 for palm, ~0.05 for edge
				double probFist = 1.0 - 2.0*abs(circRatio - 0.6);
				double probPalm = 1.0 - 2.0*abs(circRatio - 0.2);
				double probEdge = 1.0 - 2.0*abs(circRatio - 0.05);

				//Change the gesture only if the gesture has convincingly been changed (to allow small
				//oscillations of probability within the use of a certain gesture)
				if(circRatio > 0.5)
					gesture = FIST;
				if(circRatio > 0.12 && circRatio < 0.3)
					gesture = PALM;
				if(circRatio < 0.08)
					gesture = EDGE;
				//React appropriately to the given gesture
				if(area > 0) //prevents crashing when no skin is detected
				{
					if(gesture==FIST)
					{
						paintSquare(posX-2, posX+2, posY-2, posY+2, 255,0,0, painting);
					}
					else if(gesture==PALM)
					{
						paintSquare(posX-5, posX+5, posY-5, posY+5, 255,0,0, painting);
					}
					else if(gesture==EDGE)
					{
						paintSquare(posX-9, posX+9, posY-9, posY+9, 0,0,0, painting);
					}
				}
				
				//if the user's position changes quickly enough (e.g. moves 40 pixels since last frame)
				//then clear the whole board.
				double distance = sqrt(pow((double)(posX-lastX),2.0)+pow((double)(posY-lastY),2.0));
				if(floor(distance) > 45)
					paintSquare(0, painting.getWidth()-1, 0, painting.getHeight()-1, 0,0,0, painting);

				imshow("painting", painting.getImage());
				
				double probSweep = distance/45.0;

				//Correct any erroneous values
				probFist = bound(probFist);
				probPalm = bound(probPalm);
				probEdge = bound(probEdge);
				probSweep = bound(probSweep);

				std::cout<<"Fist: "<<probFist<<"  Open palm: "<<probPalm
					<<"  Edge of Hand: "<<probEdge<<"  Sweep: "<<probSweep<<std::endl;

				delete moments;

                // Wait for a keypress to quit
                int key = cvWaitKey(10);
                if(key!=-1)
                {
					break;
                }

                cvReleaseImage(&skinFilter);

    }
	
    cvReleaseCapture(&capture);
    return 0;
}