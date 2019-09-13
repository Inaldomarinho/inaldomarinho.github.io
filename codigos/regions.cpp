#include <iostream>
#include <opencv2/opencv.hpp>
#include <highgui.h>

using namespace std;
using namespace cv;

int main(int, char**){
  Mat image;
  Vec3b val;

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu biel.png" << endl;

  cout << "Width : " << image.cols << endl;
  cout << "Height: " << image.rows << endl;
  
  int p1x, p1y, p2x, p2y;
  cout << "Você irá digitar dois pontos P1 (esquerdo superior) e P2(Direito inferior)"<< endl;
  cout << "Digita a coordenada X do ponto P1:";
  cin >> p1x;
  if(p1x < 0 || p1x > image.cols){
	cout << "Coordenada fora da imagem";
	return 0;
  }
  cout << "Digita a coordenada Y do ponto P1:";
  cin >> p1y;
  if(p1y < 0 || p1y > image.rows){
	cout << "Coordenada fora da imagem";
	return 0;
  }
  cout << "Digita a coordenada X do ponto P2:";
  cin >> p2x;
  if(p2x < 0 || p2x > image.cols){
	cout << "Coordenada fora da imagem";
	return 0;
  }
  cout << "Digita a coordenada Y do ponto P2:";
  cin >> p2y;
  if(p2y < 0 || p2y > image.rows){
	cout << "Coordenada fora da imagem";
	return 0;
  }

  if(p1x>p2x && p1y > p2y){
     int auxx, auxy;
     auxx = p1x;
     auxy = p1y;
     p1x = p2x;
     p1y = p2y;
     p2x = auxx;
     p2y = auxy;	
  }
  else if (p1x>p2x || p1y > p2y){
	cout << "Coordenadas digitadas de forma inválida";
  }

  namedWindow("janela",WINDOW_AUTOSIZE);
  
  for(int i=p1y;i<p2y;i++){
    for(int j=p1x;j<p2x;j++){
      image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
    }
  }
  
  imshow("janela", image);  
  waitKey();

  return 0;
}
