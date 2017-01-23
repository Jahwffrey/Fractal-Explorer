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

bool buddhaBool = false;
bool flamingship = true;
int colorDelt = 0;
Mat buddha(640,640,CV_8UC1);

double calcReal(double x,double y,double x2,double y2){
	return (x * x2) - (y * y2);
}

double calcImag(double x,double y,double x2, double y2){
	return (x * y2) + (y * x2); 
}

double magnitude(double x,double y, int mode){
	if(mode == 1) return sqrt(pow(x,2) - pow(y,2));
	return sqrt(pow(x,2) + pow(y,2));
}

char getR(int num,int mode){
	if(num == -1) return 0;
	num = (num + colorDelt) % 255;
	if(mode == 0) return (num % 127) * (255/127);
	if(mode == 1) return (tan((double)num/16) + 1) * 127;
	if(mode == 2) return 255 * ((sin((double)num*PI/64) + 1)/2.0);
	if(mode == 3) return 255 * ((sin((double)num*PI/16) + 1)/2.0);
	return 255;
}

char getG(int num,int mode){
	if(num == -1) return 0;
	num = (num + colorDelt) % 255;
	if(mode == 0) return (num % 63) * (255/63);
	if(mode == 1) return (sin((double)num/16) + 1) * 127;
	if(mode == 2) return 255 * ((sin((double)num*PI/64 + ((2/3.0)*PI) ) + 1)/2.0);
	if(mode == 3) return 255 * ((sin((double)num*PI/16 + ((2/3.0)*PI) ) + 1)/2.0);
	return 255;
}

char getB(int num,int mode){
	if(num == -1) return 0;
	num = (num + colorDelt) % 255;
	if(mode == 0) return (num % 31) * (255/31);
	if(mode == 1) return (cos((double)num/16) + 1) * 127;
	if(mode == 2) return 255 * ((sin((double)num*PI/64 + ((4/3.0)*PI) ) + 1)/2.0);
	if(mode == 3) return 255 * ((sin((double)num*PI/16 + ((4/3.0)*PI) ) + 1)/2.0);
	return 255;
}

double getComplexCoord(double imgLen,int imgCoord, double low, double high){
	return low - (imgCoord/imgLen) * (low - high);
}

int getPoint(int iters,double x,double y,double cx, double cy,int power,int magMode){
	double tx = x;
	double ty = y;

	for(int i = 0;i < iters;i++){
		if(fabs(magnitude(x,y,magMode)) >= 2){
			if(!buddhaBool){
				return i;
			} else {
				x = tx;
				y = ty;
				for(int j = 0;j < iters;j++){
				
					int bX = (x + 2.0) * ((double)buddha.cols/4.0);
					int bY = buddha.rows - (y + 2.0) * ((double)buddha.rows/4.0);
					//std::cout << "(" << bX << "," << bY << ")\n";
					int val = buddha.at<uchar>(bY,bX);
					val += 15;
					if(val > 255) val = 255;
					buddha.at<uchar>(bY,bX) = val;
	
					double valx = calcReal(x,y,x,y);
					double valy = calcImag(x,y,x,y);
					for(int k = 2;k < power;k++){
						double tempx = calcReal(valx,valy,x,y);
						double tempy = calcImag(valx,valy,x,y);
						valx = tempx;
						valy = tempy;
					}
				
					if(flamingship){
						x = fabs(valx + cx);
						y = fabs(valy + cy);
					} else {
						x = valx + cx;
						y = valy + cy;
					}

					if(fabs(magnitude(x,y,magMode)) >= 2){
						return j;
					}	
				}	
				
			}
		}
		double valx = calcReal(x,y,x,y);
		double valy = calcImag(x,y,x,y);
		for(int k = 2;k < power;k++){
			double tempx = calcReal(valx,valy,x,y);
			double tempy = calcImag(valx,valy,x,y);
			valx = tempx;
			valy = tempy;
		}
		if(flamingship){
			x = fabs(valx + cx);
			y = fabs(valy + cy);
		} else {
			x = valx + cx;
			y = valy + cy;
		}
	}

	return -1;
}

void displayFractal(Mat img,double lf,double rg,double up,double dw,int iterMode,int iters,double cx, double cy,int cmode,int power,int magMode){
	for(double i = 0;i < img.rows;i++){
		for(double j = 0;j < img.cols;j++){
			
			if(buddhaBool){
				buddha.at<char>(i,j) = 0;
			}
			
			double x = getComplexCoord(img.cols,j,lf,rg);//rg - (i / img.rows) * (rg - lf);
			double y = -getComplexCoord(img.rows,i,dw,up);//dw - (j /img.cols) * (dw - up);
			
			int it;
			switch(iterMode){
				case 0: //julia
					it = getPoint(iters,x,y,cx,cy,power,magMode);
					break;
				case 1: //mandelbrot
					it = getPoint(iters,cx,cy,x,y,power,magMode);
					break;
				case 2: //reals
					it = getPoint(iters,x,cx,y,cy,power,magMode);
					break;
				case 3: //imaginary
					it = getPoint(iters,cx,x,cy,y,power,magMode);
					break;
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

int doesLineCollideSet(float delt,int iters,float sx,float sy,float sz,float sw,float ex,float ey,float ez,float ew){
	float dist = sqrt(pow(sx - ex,2) + pow(sy - ey,2) + pow(sz - ez,2) + pow(sw - ew,2));
	float dx = -(sx - ex)/delt;
	float dy = -(sy - ey)/delt;
	float dz = -(sz - ez)/delt;
	float dw = -(sw - ew)/delt;
	
	for(int i = 0;i < delt;i++){
		float cx = sx + i*dx;
		float cy = sy + i*dy;
		float cz = sz + i*dz;
		float cw = sw + i*dw;
		int pt = getPoint(iters,cx,cy,cz,cw,2,0);
		if(pt == -1) return i;
	}
	return -1;
}

void raymarch3DFractal(Mat img,float scrnW, float scrnH, float delt, int iters, float x,float y,float z,float w){

	//First, find the normal vector to my plane as being at the center defined by parameters and pointing towards center

	//if we always look at 0,0,0,0, the vector pointng from me to the center is merely negative the center
	float len = sqrt(x*x + y*y + z*z + w*w);
			
	//a unit vector pointing from me to the center
	float ux = -x/len;
	float uy = -y/len;
	float uz = -z/len;
	float uw = -w/len;

	for(double i = 0;i < img.rows;i++){
		for(double j = 0;j < img.cols;j++){
			//this is the two dimensional coordinate of the point within the plane
			float px1 = ((i/img.rows) - 0.5) * 4;//* scrnW;
			float py1 = ((j/img.cols) - 0.5) * 4;//((j - img.cols/2)/(img.cols/2)) * scrnH;
			float pz1 = -2;
			float pw1 = -2;

			float px2 = px1;
			float py2 = py1;
			float pz2 = 2;
			float pw2 = 2;




			//I have a point in two dimensions withing a plane that exists in four dimensions
			//I need to find the four dimensional location of this point

			//ux(nx - x) + ... + = 0
			
			//int coll = doesLineCollideSet(delt,iters,x + relI,y + relJ,z,2,x+relI,y + relJ,z,-2);
			int coll = doesLineCollideSet(delt,iters,px1,py1,pz1,pw1,px2,py2,pz2,pw2);
			int pColor = 255;
			if(coll != -1){
				pColor = 255 - (255 * ((float)coll/delt));
			}
			Vec3b final = img.at<Vec3b>(i,j);
			final[0] = pColor;
			final[1] = pColor;
			final[2] = pColor;
			img.at<Vec3b>(i,j) = final;
		}
	}		
}

int main(int argc,char** argv){
	bool cycle = false;
	bool smallize = true;
	int mandel = 0;

	int mouse[3] = {0,0,-1};

	int imWidth = 640;
	int imHeight = 640;
	//double cX = -0.3482 //Whirlygoo
	double cX = -0.778;//-.79; //fancydance
	//double cX = 0; //fancydance
	//double cY = 0.6108 //Whirlygoo
	double cY = -0.116;//0.15; //fancydance
	//double cY = 0; //fancydance
	double lf = -2;
	double rg = 2;//-2;
	double up = 2;
	double dw = -2;//-2;
	int order = 2;

	double speed = 0.01;

	int sqX = 0;
	int sqY = 0;
	int sqW = 0;

	int colorMode = 1;

	bool drawSq = false;

	int iterNum = 100;
	int iterMode = 0;
	int magMode = 0;

	if(cycle && smallize){
		imWidth /= 4;
		imHeight /= 4;
	}

	namedWindow("Fractal",1);
	setMouseCallback("Fractal", mouseFunc, mouse);

	srand(time(NULL));


	//Mat firstImg(imWidth,imHeight,CV_8UC3);
	//raymarch3DFractal(firstImg,4,4, 100,32, 0,0,0,2);
	//imshow("More Fractal",firstImg);
	
	while(true){
		Mat img(imWidth,imHeight,CV_8UC3);

		displayFractal(img,lf,rg,up,dw,mandel,iterNum,cX,cY,colorMode,order,magMode);	
	
		while(true){
			Mat disp = img.clone();

			if(drawSq){
				rectangle(disp,Rect(sqX,sqY,sqW,sqW),Scalar(255,255,255));
			}

			imshow("Fractal",disp);
			if(buddhaBool){
				imshow("Buddha",buddha);
			}
			/*for(int l = 0;l < 120;l++){
				for(int ll = 0;ll < 120;ll++){
					std::cout << "[" << buddha.at<char>(l,ll) << "]";
				}
				std::cout << std::endl;
			}*/

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
				mandel = (mandel + 1) % 4;
				break;
			}
			if(c == 'n'){
				colorMode = (colorMode + 1) % 5;
				break;
			}
			if(c == 'b'){
				magMode = (magMode  + 1) % 2;
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
			if(c == '7'){
				colorDelt += 1;
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
	}

	return 0;
}
