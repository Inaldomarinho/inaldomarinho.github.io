#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects;
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;
 
  int auxcont=0;
  // busca objetos na imagem
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		// achou um objeto
		nobjects++;
		if (nobjects == 254){
			nobjects = 0;
			auxcont +=1;

                }
		p.x=j;
		p.y=i;
		floodFill(image,p,nobjects);
	  }
	}
  }
  
		 
  cout << "Número de Objetos na Imagem:" << nobjects << endl;
  imshow("Contando o Número de Objetos", image);

	// retirando os objetos das bordas
   for(int i=0; i<width; i++){
        floodFill(image,Point(0, i), 255);
        floodFill(image,Point(height-1, i), 255);
  }

    for(int i=0; i<height; i++){
        floodFill(image,Point(i, 0), 255);
        floodFill(image,Point(i, width-1), 255);
  } 

  imshow("Sem objetos na Borda", image);

  int nburaco=0;
 // contando quantos objetos tem buraco
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){
		// achou um objeto
		nburaco++;
		p.x=j;
		p.y=i;
		floodFill(image,p,255);
                p.x=j-1;
		p.y=i;
		floodFill(image,p,255);
	  }
	}
  }

  int semburaco=0;
  // contando quantidade de objetos com buraco
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) != 255){
		// achou um objeto
		semburaco++;
		p.x=j;
		p.y=i;
		floodFill(image,p,255);
	  }
	}
  }
 
 
  
  cout << "Número de objetos com buracos na imagem: "<< nburaco << endl;
  cout << "Número de objetos sem buracos na imagem: "<< semburaco << endl;

  imshow("Contando o Número de Buracos", image);

  //imwrite("labeling.png", image);
  waitKey();
  return 0;
}
