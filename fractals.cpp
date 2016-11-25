#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>


using namespace cv;

double calcReal(double x,double y){
	return pow(x,2) - pow(y,2);
}

double calcImag(double x,double y){
	return 2 * x * y;
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

void displayFractal(Mat img,double rg,double lf,double up,double dw,bool mandel,int iters,double cx, double cy){
	double tx = cx;
	double ty = cy;
	for(double i = 0;i < img.rows;i++){
		for(double j = 0;j < img.cols;j++){
			double x = rg - (i / img.rows) * (rg - lf);
			double y = dw - (j /img.cols) * (dw - up);

			double mag = 0;
			char r = 0;
			char g = 0;
			char b = 0;

			if(mandel){
				cx = x;
				cy = y;
				x = tx;
				y = ty;


				for(int i = 0;i < iters;i++){
					double valx = calcReal(x,y) + cx;
					double valy = calcImag(x,y) + cy;
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
					double valx = calcReal(x,y) + cx;
					double valy = calcImag(x,y) + cy;
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
			Vec3b final = img.at<Vec3b>(i,j);
			final[0] = b;
			final[1] = g;
			final[2] = r;
			img.at<Vec3b>(i,j) = final;
		}
	}
}

int main(int argc,char** argv){
	int imWidth = 640;
	int imHeight = 640;

	bool cycle = false;
	bool smallize = true;

	if(cycle && smallize){
		imWidth /= 4;
		imHeight /= 4;
	}

	srand(time(NULL));


	while(true){
		Mat img(imWidth,imHeight,CV_8UC3);
	
//void displayFractal(Mat img,double rg,double lf,double up,double dw,bool mandel,int iters){

		displayFractal(img,-2,2,-2,2,false,100,-.79,0.15);	

		//iters++;

		//if(randomize){
			//cx = (-5000 + (rand() % 10000))/5000.0;
			//cy = (-5000 + (rand() % 10000))/5000.0;
			//tx = cx;
			//ty = cy;
		//}

		imshow("Fractal",img);
		waitKey(5);
		//if(cycle){
			//if(mandelbrot){
				//ty += speed;
				//if(ty > 2){
				//	ty = -2;
				//	tx += speed;
				//}
				//if(tx > 2){
				//	tx = -2;
				//}
			//} else {
		//		cx += speed;
		//		if(cx > 2){
		//			cx = -2;
		//			cy += speed;
		//		}
		//		if(cy > 2){
		//			cy = -2;
		//		}
			//}
		//}
	}

	return 0;
}
