#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

  // definindo variaveis como globais -----------------------------
Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, tmp; 
  Mat_<float> realInput, zeros;
  vector<Mat> planos;
  int gammaL, gammaH, sharpC, cutoff;

  // valor do ruido
  float mean;

  // guarda tecla capturada
  char key;

  // valores ideais dos tamanhos da imagem
  // para calculo da DFT
  int dft_M, dft_N;


  // inicializa as variaveis dos sliders
int gammaL_slider = 9, gammaH_slider = 34, sharpC_slider = 53, cutoff_slider = 16;
const int gammaL_max = 10, gammaH_max = 50, sharpC_max = 100, cutoff_max = 200;
// -----------------------------------------------------


// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

// função que prepara o filtro homomorfico baseado nos parametros gl, gh, c, d0
Mat homomorphicFilter(double gl, double gh, double c, double d0){
  Mat filter = Mat(padded.size(), CV_32FC2, Scalar(0));
  Mat tmp = Mat(dft_M, dft_N, CV_32F);
  
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      tmp.at<float> (i,j) = (gh - gl)*(1 - exp(-c*(( (i-dft_M/2)*(i-dft_M/2) + (j-dft_N/2)*(j-dft_N/2) ) / (d0*d0) ))) + gl;
    }
  }

  Mat comps[]= {tmp,tmp};
  merge(comps, 2, filter);
  return filter;
}

Mat filtragemHomomorfica(){

    // realiza o padding da imagem
    copyMakeBorder(image, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);

    // adiciona 1 a cada pixel da imagem para não haver log de 0
    realInput += Scalar::all(1);
    
    // realiza o log da imagem
    log(realInput,realInput);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);
    
    // filtro homomorfico    
    Mat filter = homomorphicFilter(gammaL,gammaH,sharpC,cutoff);

    // aplica o filtro homomorfico
    mulSpectrums(complexImage,filter,complexImage,0);

    // limpa o array de planos
    planos.clear();
    // separa as partes real e imaginaria para modifica-las
    split(complexImage, planos);
 
    // recompoe os planos em uma unica matriz complexa
    merge(planos, complexImage);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

	
    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    return planos[0];
}
void on_trackbar(int, void*){
  gammaL = (double) gammaL_slider/10;
  gammaH = (double) gammaH_slider/10;
  sharpC = (double) sharpC_slider;
  cutoff = (double) cutoff_slider;
  tmp = filtragemHomomorfica();
  imshow("Filtrada", tmp);
}
int main(int argc, char** argv){
  
  namedWindow("Filtrada", WINDOW_KEEPRATIO);
  namedWindow("Original", WINDOW_KEEPRATIO);
    
  // ler a imagem
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  imshow("Original",image);

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);

  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);

  char Name[50];

  sprintf( Name, "Gamma L x %d", gammaL_max );
  createTrackbar( Name, "Filtrada", &gammaL_slider, gammaL_max, on_trackbar);

  sprintf( Name, "Gamma H x %d", gammaH_max );
  createTrackbar( Name, "Filtrada", &gammaH_slider, gammaH_max, on_trackbar);

  sprintf( Name, "C x %d", sharpC_max );
  createTrackbar( Name, "Filtrada", &sharpC_slider, sharpC_max, on_trackbar);
  
  sprintf( Name, "Cutoff Frequency x %d", cutoff_max );
  createTrackbar( Name, "Filtrada", &cutoff_slider, cutoff_max, on_trackbar);
  

  waitKey(0);
  return 0;
}
