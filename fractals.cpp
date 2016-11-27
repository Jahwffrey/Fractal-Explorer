#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>

#define PI 3.141592

using namespace cv;

double calcReal(double x,double y,double x2,double y2){
	return (x * x2) - (y * y2);
}

double calcImag(double x,double y,double x2, double y2){
	return (x * y2) + (y * x2); 
}

double magnitude(double x,double y){
	return sqrt(pow(x,2) - pow(y,2));
}

char getR(int num,int mode){
	if(num == -1) return 0;
	if(mode == 0) return (num % 127) * (255/127);
	if(mode == 1) return (tan((double)num/16) + 1) * 127;
	if(mode == 2) return 255 * ((sin((double)num*PI/64) + 1)/2.0);
	if(mode == 3) return 255 * ((sin((double)num*PI/16) + 1)/2.0);
	return 255;
}

char getG(int num,int mode){
	if(num == -1) return 0;
	if(mode == 0) return (num % 63) * (255/63);
	if(mode == 1) return (sin((double)num/16) + 1) * 127;
	if(mode == 2) return 255 * ((sin((double)num*PI/64 + ((2/3.0)*PI) ) + 1)/2.0);
	if(mode == 3) return 255 * ((sin((double)num*PI/16 + ((2/3.0)*PI) ) + 1)/2.0);
	return 255;
}

char getB(int num,int mode){
	if(num == -1) return 0;
	if(mode == 0) return (num % 31) * (255/31);
	if(mode == 1) return (cos((double)num/16) + 1) * 127;
	if(mode == 2) return 255 * ((sin((double)num*PI/64 + ((4/3.0)*PI) ) + 1)/2.0);
	if(mode == 3) return 255 * ((sin((double)num*PI/16 + ((4/3.0)*PI) ) + 1)/2.0);
	return 255;
}

double getComplexCoord(double imgLen,int imgCoord, double low, double high){
	return low - (imgCoord/imgLen) * (low - high);
}

int getPoint(int iters,double x,double y,double cx, double cy,int power){
	for(int i = 0;i < iters;i++){
		double valx = calcReal(x,y,x,y);
		double valy = calcImag(x,y,x,y);
		for(int i = 2;i < power;i++){
			double tempx = calcReal(valx,valy,x,y);
			double tempy = calcImag(valx,valy,x,y);
			valx = tempx;
			valy = tempy;
		}
	
		x = valx + cx;
		y = valy + cy;
	
		if(fabs(magnitude(x,y)) >= 2){
			return i;
		}
	}
	return -1;
}

void displayFractal(Mat img,double lf,double rg,double up,double dw,bool mandel,int iters,double cx, double cy,int cmode,int power){
	for(double i = 0;i < img.rows;i++){
		for(double j = 0;j < img.cols;j++){
			double x = getComplexCoord(img.cols,j,lf,rg);//rg - (i / img.rows) * (rg - lf);
			double y = -getComplexCoord(img.rows,i,dw,up);//dw - (j /img.cols) * (dw - up);
			
			int it;
			if(mandel) {
				it = getPoint(iters,cx,cy,x,y,power);
			} else {
				it = getPoint(iters,x,y,cx,cy,power);
			}

			Vec3b final = img.at<Vec3b>(i,j);
			final[0] = getB(it,cmode);
			final[1] = getG(it,cmode);
			final[2] = getR(it,cmode);
			img.at<Vec3b>(i,j) = final;
		}
	}
}

void mouseFunc(int event, int x, int y, int flags,void * mouse){
	int * m = (int *)mouse;
	if (event == EVENT_LBUTTONDOWN){
		*m = x;
		*(m + 1) = y;
		*(m + 2) = EVENT_LBUTTONDOWN;
	} else if (event == EVENT_RBUTTONDOWN) {
		*m = x;
		*(m + 1) = y;
		*(m + 2) = EVENT_RBUTTONDOWN;

	} else if (event == EVENT_MBUTTONDOWN) {
		//std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")\n";
	} else if (event == EVENT_MOUSEMOVE) {
		*m = x;
		*(m + 1) = y;
		*(m + 2) = EVENT_MOUSEMOVE;
	}
}

int main(int argc,char** argv){
	bool cycle = false;
	bool smallize = true;
	bool mandel = false;

	int mouse[3] = {0,0,-1};

	int imWidth = 640;
	int imHeight = 640;
	//double cX = -0.3482 //Whirlygoo
	double cX = -.79; //fancydance
	//double cY = 0.6108 //Whirlygoo
	double cY = 0.15; //fancydance
	double lf = -2;
	double rg = 2;//-2;
	double up = 2;
	double dw = -2;//-2;
	int order = 2;

	double speed = 0.001;

	int sqX = 0;
	int sqY = 0;
	int sqW = 0;

	int colorMode = 1;

	bool drawSq = false;

	int iterNum = 100;

	if(cycle && smallize){
		imWidth /= 4;
		imHeight /= 4;
	}

	namedWindow("Fractal",1);
	setMouseCallback("Fractal", mouseFunc, mouse);

	srand(time(NULL));


	while(true){
		Mat img(imWidth,imHeight,CV_8UC3);
	
		displayFractal(img,lf,rg,up,dw,mandel,iterNum,cX,cY,colorMode,order);	
	
		while(true){
			Mat disp = img.clone();

			if(drawSq){
				rectangle(disp,Rect(sqX,sqY,sqW,sqW),Scalar(255,255,255));
			}

			imshow("Fractal",disp);
			char c = waitKey(5);
			if(c == '1'){
				iterNum+=5;
				//cX = 0;
				//cY = 0;
				break;
			}
			if(c == '2'){
				iterNum*=2;
				//cX = 0;
				//cY = 0;
				break;
			}
			if(c == '3'){
				iterNum/=2;
				//cX = 0;
				//cY = 0;
				break;
			}
			if(c == '4'){
				iterNum-=5;
				//cX = 0;
				//cY = 0;
				break;
			}
			if(c == '0'){
				cX = 0;
				cY = 0;
				break;
			}
			if(c == 'r'){
				cX = (-5000 + (rand() % 10000))/5000.0;
				cY = (-5000 + (rand() % 10000))/5000.0;
				std::cout << "" << cX << " + " << cY << "i\n" << std::flush;
				break;
			}
			if(c == 'c'){
				drawSq = false;
			}
			if(c == 'm'){
				if(mandel == true){
					mandel = false;
				} else {
					mandel = true;
				}
				break;
			}
			if(c == 'n'){
				colorMode = (colorMode + 1) % 5;
				break;
			}
			if(c == 'q'){
				lf = -2;
				rg = 2;
				up = 2;
				dw = -2;
				iterNum = 100;
				//cX = 0;
				//cY = 0;
				break;
			}
			if(c == '6'){
				order++;
				break;
			}
			if(c == '5'){
				order--;
				break;
			}
			if(c == 'w'){
				cY -= speed;
				break;
			}
			if(c == 's'){
				cY += speed;
				break;
			}
			if(c == 'd'){
				cX += speed;
				break;
			}
			if(c == 'a'){
				cX -= speed;
				break;
			}
			//Check mouse
			if(mouse[2] == EVENT_LBUTTONDOWN){
				if(drawSq == false){
					drawSq = true;
					sqX = mouse[0];
					sqY = mouse[1];
					mouse[2] = -1;
				} else {
					drawSq = false;
					mouse[2] = -1;
					double nlf = getComplexCoord(img.cols,sqX,lf,rg);
					double nrg = getComplexCoord(img.cols,sqX + sqW,lf,rg);
					double ndw = -getComplexCoord(img.rows,sqY,dw,up);
					double nup = -getComplexCoord(img.rows,sqY + sqW,dw,up);
					lf = nlf;
					rg = nrg;
					up = nup;
					dw = ndw;
					nlf = getComplexCoord(img.cols,0,lf,rg);
					nrg = getComplexCoord(img.cols,img.cols,lf,rg);
					nup = -getComplexCoord(img.rows,img.rows,dw,up);
					ndw = -getComplexCoord(img.rows,0,dw,up);
					lf = nlf;
					rg = nrg;
					up = nup;
					dw = ndw;	
					sqW = 0;
					break;
				}
			} else if(mouse[2] == EVENT_MOUSEMOVE){
				mouse[2] = -1;
				if(drawSq == true){
					int hW = mouse[0] - sqX;
					int vW = mouse[1] - sqY;
					sqW = hW;
					if(vW > hW) sqW = vW;
				}
			} else if(mouse[2] == EVENT_RBUTTONDOWN){
				mouse[2] = -1;
				double nlf = getComplexCoord(img.cols,-img.cols,lf,rg);
				double nrg = getComplexCoord(img.cols,2*img.cols,lf,rg);
				double ndw = -getComplexCoord(img.rows,-img.rows,dw,up);
				double nup = -getComplexCoord(img.rows,2*img.rows,dw,up);
				lf = nlf;
				rg = nrg;
				up = nup;
				dw = ndw;
				nlf = getComplexCoord(img.cols,0,lf,rg);
				nrg = getComplexCoord(img.cols,img.cols,lf,rg);
				nup = -getComplexCoord(img.rows,img.rows,dw,up);
				ndw = -getComplexCoord(img.rows,0,dw,up);
				lf = nlf;
				rg = nrg;
				up = nup;
				dw = ndw;	
				break;
			}
		}
		//if(randomize){
			
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
