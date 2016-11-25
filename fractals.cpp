#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>


using namespace cv;

double cx = -2;//-0.79;
double cy = -2;// 0.15;

int imWidth = 640;
int imHeight = 640;

int iters = 100;

double speed = 0.1;

bool mandelbrot = false;
bool randomize = false;
bool cycle = false;
bool smallize = true;


//x + yi x + yi
//xx
//xyi xyi
//yi yi
//yi * yi = -y^2
double calcReal(double x,double y){
	return pow(x,2) - pow(y,2) + cx;
}

double calcImag(double x,double y){
	return 2 * x * y + cy;
}

double magnitude(double x,double y){
	return sqrt(pow(x,2) - pow(y,2));
}

char getR(int num){
	return (num % 127) * (255/127);
}

char getG(int num){
	return (num % 63) * (255/63);
}

char getB(int num){
	return (num % 31) * (255/31);
}

int main(int argc,char** argv){
	if(cycle && smallize){
		imWidth /= 4;
		imHeight /= 4;
	}

	srand(time(NULL));

	double tx = cx;
	double ty = cy;
	while(true){
		Mat img(imWidth,imHeight,CV_8UC3);
		double biggest = 0;
		for(double i = 0;i < img.rows;i++){
			for(double j = 0;j < img.cols;j++){
				double x = (i - (imWidth/2))/(imWidth/4);
				double y = (j - (imHeight/2))/(imHeight/4);

				double mag = 0;
		
				char r = 0;
				char g = 0;
				char b = 0;

				if(mandelbrot){
					cx = x;
					cy = y;
					x = tx;
					y = ty;


					for(int i = 0;i < iters;i++){
						double valx = calcReal(x,y);
						double valy = calcImag(x,y);
						x = valx;
						y = valy;
						mag = magnitude(x,y);
						if(fabs(mag) > 2){
							r = getR(i);
							g = getG(i);
							b = getB(i);
							i = iters;
						}
					}
				} else {

					for(int i = 0;i < iters;i++){
						double valx = calcReal(x,y);
						double valy = calcImag(x,y);
						x = valx;
						y = valy;
						mag = magnitude(x,y);
						if(fabs(mag) > 2){
							r = getR(i);
							g = getG(i);
							b = getB(i);
							i = iters;
						}
					}
				}

				//if(mag > 2) mag = 2;
				//if(mag < 2) mag = -2;

				//double col = fabs(mag);//127.5;

				//char final = img.at<char>(i,j);
				//final = col;
				//img.at<char>(i,j) = fabs(mag) * 10000;//255;

				//if(fabs(mag) > biggest){
				//	biggest = fabs(mag);
				//	std::cout << biggest << "\n" << std::flush;
				//}
				//double col = 0;
				//if(fabs(mag) > 2){ 
				//	r = 255;
				//	g = 255;
				//	b = 255;
				//}
				Vec3b final = img.at<Vec3b>(i,j);
				final[0] = b;
				final[1] = g;
				final[2] = r;
				img.at<Vec3b>(i,j) = final;
			}
		}

		//iters++;

		if(randomize){
			cx = (-5000 + (rand() % 10000))/5000.0;
			cy = (-5000 + (rand() % 10000))/5000.0;
			tx = cx;
			ty = cy;
		}

		imshow("Fractal",img);
		waitKey(5);
		if(cycle){
			if(mandelbrot){
				ty += speed;
				if(ty > 2){
					ty = -2;
					tx += speed;
				}
				if(tx > 2){
					tx = -2;
				}
			} else {
				cx += speed;
				if(cx > 2){
					cx = -2;
					cy += speed;
				}
				if(cy > 2){
					cy = -2;
				}
			}
		}
	}

	return 0;
}
