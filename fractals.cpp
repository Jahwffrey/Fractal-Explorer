#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>


using namespace cv;

double calcReal(double x,double y,double x2,double y2){
	//return pow(x,2) - pow(y,2);
	return (x * x2) - (y * y2);
}

double calcImag(double x,double y,double x2, double y2){
	//return 2 * x * y;
	return (x * y2) + (y * x2); 
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
					double valx = calcReal(x,y,x,y) + cx;
					double valy = calcImag(x,y,x,y) + cy;
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
					double valx = calcReal(x,y,x,y) + cx;
					double valy = calcImag(x,y,x,y) + cy;
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

void mouseFunc(int event, int x, int y, int flags,void * mouse){
	int * m = (int *)mouse;
	if (event == EVENT_LBUTTONDOWN){
		std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")\n";
		*m = x;
		*(m + 1) = y;
		*(m + 2) = EVENT_LBUTTONDOWN;
	} else if (event == EVENT_RBUTTONDOWN) {
		std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")\n";
	} else if (event == EVENT_MBUTTONDOWN) {
		std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")\n";
	} else if (event == EVENT_MOUSEMOVE) {
		std::cout << "Mouse move over the window - position (" << x << ", " << y << ")\n";
	}
}

int main(int argc,char** argv){
	bool cycle = false;
	bool smallize = true;

	int mouse[3] = {0,0,-1};

	int imWidth = 640;
	int imHeight = 640;
	double cX = -.79;
	double cY = 0.15;

	int sqX = 0;
	int sqY = 0;
	bool drawSq = false;

	if(cycle && smallize){
		imWidth /= 4;
		imHeight /= 4;
	}

	namedWindow("Fractal",1);
	setMouseCallback("Fractal", mouseFunc, mouse);

	srand(time(NULL));


	while(true){
		Mat img(imWidth,imHeight,CV_8UC3);
	
		displayFractal(img,-2,2,-2,2,true,100,cX,cY);	
	
		while(true){
			Mat disp = img.clone();

			if(drawSq){
				rectangle(disp,Rect(sqX,sqY,100,100),Scalar(255,255,255));
			}

			imshow("Fractal",disp);
			char c = waitKey(5);
			if(c == 'a'){
				cX = 0;
				cY = 0;
				break;
			}
			//Check mouse
			if(mouse[2] == EVENT_LBUTTONDOWN){
				drawSq = true;
				sqX = mouse[0];
				sqY = mouse[1];
				mouse[2] = -1;
			}
		}
		//if(randomize){
			//cx = (-5000 + (rand() % 10000))/5000.0;
			//cy = (-5000 + (rand() % 10000))/5000.0;
			//tx = cx;
			//ty = cy;
		//}

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
