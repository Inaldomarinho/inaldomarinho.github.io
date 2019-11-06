#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 3
#define JITTER 5
#define RAIO 3

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, frame, points, border;

  int width, height, gray;
  int x, y;
  
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  srand(time(0));
  
  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);
  
  iota(xrange.begin(), xrange.end(), 0); 
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }
  
  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());
  
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }
  imwrite("pontosantes.jpg", points);
  int intervalos[5] = {5, 50, 500, 550, 600};
    for(int k = 0; k<5; k++){
      Canny(image, border, intervalos[k], 3*intervalos[k]);
        for(int i=0;i<height;i++){
          for(int j=1;j<width;j++){
	    if (border.at<uchar>(j,i) == 255 and border.at<uchar>(j-1,i) == 0){
		    gray = image.at<uchar>(j,i);
		    circle(points,
		      cv::Point(i,j),
			     k+1,
			     CV_RGB(gray,gray,gray),
			     -1,
			     CV_AA);
	     }
          }
       }
    }
  imwrite("pontos.jpg", points);
  return 0;
}
