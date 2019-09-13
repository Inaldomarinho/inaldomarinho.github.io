#include <iostream>
#include <opencv2/opencv.hpp>
#include <highgui.h>

using namespace std;
using namespace cv;

int main(int, char**){
  Mat image, modificada, quadrante1, quadrante2, quadrante3, quadrante4;

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu biel.png" << endl;
  
  namedWindow("Imagem",WINDOW_AUTOSIZE);
  namedWindow("Imagem Modificada",WINDOW_AUTOSIZE);

  image.copyTo(modificada);

  quadrante1= image(Rect(0,image.cols/2,image.rows/2,image.cols/2));
  quadrante2= image(Rect(0,0,image.rows/2,image.cols/2));
  quadrante3= image(Rect(image.rows/2,0,image.rows/2,image.cols/2));
  quadrante4=  image(Rect(image.rows/2,image.cols/2,image.rows/2,image.cols/2));

  quadrante1.copyTo(modificada(Rect(image.rows/2,0,image.rows/2,image.cols/2)));
  quadrante2.copyTo(modificada(Rect(image.rows/2,image.cols/2,image.rows/2,image.cols/2)));
  quadrante3.copyTo(modificada(Rect(0,image.cols/2,image.rows/2,image.cols/2)));
  quadrante4.copyTo(modificada(Rect(0,0,image.rows/2,image.cols/2)));

  imshow("Imagem", image);
  imshow("Imagem Modificada", modificada);
  waitKey();

  imwrite("TrocaRegions.png", modificada);
  waitKey();

  return 0;
}
