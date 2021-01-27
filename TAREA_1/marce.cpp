#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace cv; 
using namespace std;

Mat laplace;
Mat laplace_gaussiano;
int sal = 0;
int pimienta = -255;

Mat ruido_Mediana;
Mat ruido_Gaussiana;
Mat ruido_Blur;
Mat video_frame;
cv::Mat sobel,sobel2,sobel2Abs,bordes,bordesAbs,gris,gX,gY,gXAbs,gYAbs,a,b;
cv::Mat imagen_sal, pimienta_img,ruido_sal,ruido_Pimienta;

void trackBarEventHSV(int v, void *px){
}

int main(){

VideoCapture video("cortometraje");

if(video.isOpened()){
    
    namedWindow("Original", WINDOW_AUTOSIZE);
    moveWindow("Original", 695, 85);
    namedWindow("Ruido de sal", WINDOW_AUTOSIZE);
    moveWindow("Ruido de sal", 10, 30);
    namedWindow("Ruido de pimienta", WINDOW_AUTOSIZE);
    moveWindow("Ruido de pimienta",1320, 30);
    namedWindow("Filtro mediana", WINDOW_AUTOSIZE);
    moveWindow("Filtro mediana",10, 500);
    namedWindow("Filtro Gaussiano");
    moveWindow("Filtro Gaussiano", 695, 500);
    namedWindow("Filtro Blur");
    moveWindow("Filtro Blur", 1320, 500);
    namedWindow("Sobel Bordes 1", WINDOW_AUTOSIZE);
    namedWindow("Sobel Bordes 2", WINDOW_AUTOSIZE);
    
    while(3==3){
    
    video >> video_frame;
    resize(video_frame, video_frame, Size(), 0.35, 0.35);
    cvtColor(video_frame, gris, COLOR_BGR2GRAY);
    
    
    Sobel(gris, gX, CV_16S, 1, 0, 3);
    Sobel(gris, gY, CV_16S, 0, 1, 3); 
    Sobel(gris, sobel2, CV_16S, 1, 1, 3); 
        
    convertScaleAbs(gX, gXAbs); 
    convertScaleAbs(gY, gYAbs); 
    convertScaleAbs(sobel2, sobel2Abs);
    
    Laplacian(gris, bordes, CV_16S, 3);
    convertScaleAbs(bordes, bordesAbs);
    
    ruido_sal = Mat::zeros(gris.rows, gris.cols,CV_8U);
    randu(ruido_sal,sal,255);
    ruido_Pimienta = Mat::zeros(gris.rows, gris.cols,CV_8U);
    randu(ruido_Pimienta,pimienta,255);
    
    createTrackbar("RUIDO", "Ruido de sal",&sal, 255, trackBarEventHSV, NULL);
    createTrackbar("RUIDO", "Ruido de pimienta", &pimienta, 30, trackBarEventHSV, NULL); 

    a = ruido_Pimienta <30;
    b = ruido_sal > 225;

    imagen_sal = gris.clone();
    pimienta_img = gris.clone();
    
    imagen_sal.setTo(255,b);
    pimienta_img.setTo(0,a);  
    
    medianBlur(imagen_sal, ruido_Mediana, 3);
    GaussianBlur(pimienta_img, ruido_Gaussiana, Size(3, 3), 2, 2);
    blur(imagen_sal, ruido_Blur, Size(5, 5));
    
    imshow("ORIGINAL", gris);
    imshow("RUIDO SAL",imagen_sal);
    imshow("RUIDO PIMIENTA",pimienta_img);
    imshow("FILTRO MEDIANA",ruido_Mediana);
    imshow("GAUSS",ruido_Gaussiana);
    imshow("BLUR",ruido_Blur);
    imshow("SOBEL BORDES 1",bordesAbs);
    imshow("Sobel Bordes 2",sobel2Abs);
    
    //namedWindow("Sobel gX (Filtro mediana)", WINDOW_AUTOSIZE);
    //moveWindow("Sobel gX (Filtro mediana)",10, 550);
    //namedWindow("Sobel gY (Filtro mediana)", WINDOW_AUTOSIZE);
    //moveWindow("Sobel gY (Filtro mediana)", 695, 550);
    //imshow("Sobel gX (Filtro mediana)", gX);
    //imshow("Sobel gY (Filtro mediana)", gY);
    Laplacian(ruido_Gaussiana, laplace_gaussiano, CV_16S, 3);
    convertScaleAbs(laplace_gaussiano, laplace_gaussiano);
    
    Laplacian(video_frame, laplace, CV_16S, 3);
    convertScaleAbs(laplace, laplace);
    
    namedWindow("Laplace", WINDOW_AUTOSIZE);
    moveWindow("Laplace",350, 550);
    imshow("Laplace", laplace);

    namedWindow("Laplace Gaussiano", WINDOW_AUTOSIZE);
    moveWindow("Laplace Gaussiano",1000, 550);
    imshow("Laplace Gaussiano", laplace_gaussiano);
        
     if(waitKey(23)==27)
                break;       
        }
    destroyAllWindows();  
    }
    return 0;
}
