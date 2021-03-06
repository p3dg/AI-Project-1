#include "Image.h"

Image::Image(void)
{
}

Image::Image(int w, int h):
img(h,w, CV_8UC3)
{
}

void Image::operator()(Mat& frame)
{
	frame.copyTo(img);
}


Image::~Image(void)
{
}

int Image::getR(int x,int y)
{
	if(img.rows == 0)
		throw "Image is NULL";
	
	return img.data[y*img.step+img.channels()*x+2];
}

int Image::getG(int x,int y)
{
	if(img.rows == 0)
		throw "Image is NULL";
	
	return img.data[y*img.step+img.channels()*x+1];
}

int Image::getB(int x,int y)
{
	if(img.rows==0)
		throw "Image is NULL";
	
	return img.data[y*img.step+img.channels()*x+0];
}

Color Image::get(int x,int y)
{
	Color c;
	unsigned char* pixel = &img.data[img.step*y + img.channels()*x];
	c.r = img.data[y*img.step+img.channels()*x+2];
	c.g = img.data[y*img.step+img.channels()*x+1];
	c.b = img.data[y*img.step+img.channels()*x+0];
	return c;
}

void Image::set(int x,int y, Color& c)
{
	set(x,y,c.r,c.g,c.b);
}

void Image::set(int x,int y, int r, int g, int b)
{
	if(img.rows==0)
		throw "Image is NULL";
    img.data[y*img.step+img.channels()*x+2] =r;
    img.data[y*img.step+img.channels()*x+1] =g;
    img.data[y*img.step+img.channels()*x+0] =b;
}



void Image::rescale(double scaleFactor)
{
	if(scaleFactor == 0)
		throw "invalid scaleFactor:";

	Mat resized;
	resize(img, resized, Size(), scaleFactor, scaleFactor);

	img = resized.clone();
}
