#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, greyimage, greyimageeq;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat comhist, semhist;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  
  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }
  
  int histw = nbins, histh = nbins/2;
  Mat histImgS(histh, histw, CV_8UC1, Scalar(0,0,0));
  Mat histImgC(histh, histw, CV_8UC1, Scalar(0,0,0));

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;


  while(1){
    cap >> image;
    cvtColor(image, greyimage, CV_BGR2GRAY);
    equalizeHist( greyimage, greyimageeq);
    calcHist(&greyimage, 1, 0, Mat(), semhist, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&greyimageeq, 1, 0, Mat(), comhist, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(semhist, semhist, 0, histImgS.rows, NORM_MINMAX, -1, Mat());
    normalize(comhist, comhist, 0, histImgC.rows, NORM_MINMAX, -1, Mat());

    histImgS.setTo(Scalar(0));
    histImgC.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgS,
           Point(i, histh),
           Point(i, histh-cvRound(semhist.at<float>(i))),
           Scalar(255), 1, 8, 0);
      line(histImgC,
           Point(i, histh),
           Point(i, histh-cvRound(comhist.at<float>(i))),
           Scalar(255), 1, 8, 0);
    }

    histImgS.copyTo(greyimage(Rect(0, 0,nbins, histh)));
    histImgC.copyTo(greyimageeq(Rect(0, 0,nbins, histh)));

    imshow("imagem equalizada", greyimageeq);
    imshow("imagem sem equalização", greyimage);
    waitKey(30);
  }
  return 0;
}
