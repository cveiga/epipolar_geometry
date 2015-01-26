#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <windows.h>
#include <vector>
#include "geoEpi.h"
//#include "opencv2/opencv.hpp"
//#include <opencv2/opencv.hpp>

//using namespace cv;



//GeoEpi prueba;
CvScalar _color2 = CV_RGB(0,255,0);
bool _rojo = false;
IplImage *_img;

void drawLine(CvPoint p1, CvPoint p2);
void imprimeVector(CvPoint points[], unsigned short int cont);

int main(int argc, char *argv[])
{
    unsigned short int cont = 0;
    //std::string imageName("img/Art_Institute_of_Chicago_Lion_Statue.jpg");
    std::string imageName("img/parte1.jpg");
    
    _img = cvLoadImage(argc == 2? argv[1] : imageName.c_str(), 1);
    
    std::ifstream fich;
    fich.open(argc == 3 ? argv[3] : "documentation/correspondencias.txt", std::ios::in);
    if(fich.fail())
                    std::cerr << "ERROR al abrir el archivo" << std::endl;
    
    std::string dato1, dato2;
    
    CvPoint miPunto, previousPoint;
    CvPoint points1[500], points2[500];
    
    CvScalar color = CV_RGB(255, 0, 0 );
        
    unsigned short int index1, index2;
    index1 = index2 = 0;
    while(!fich.eof()){
                       fich >> dato1;
                       fich >> dato2;
                       
                       miPunto.x = atoi(dato1.c_str());
                       miPunto.y = atoi(dato2.c_str());
                       if(cont % 2 != 0){
                               //miPunto.x += 1384; 
                               drawLine(miPunto, previousPoint);                            
                       }
                       
                       //inline CvPoint cvPoint(dato1, dato2);
                       cvCircle(_img, miPunto, 1, color, 3);
                       
                       //fich.getline((float*)dato, sizeof(float));
                       //if(fich.good()){
                       //std::cout << "PUNTO: " << cont << "--> " << dato1 << ", " << dato2 << std::endl;
                       //Sleep(100);
                       //}
                       //circle(img, Point(dato1, dato2), 1, CV_RGB(0,255,0));
                       //circle(img, Point(dato1, dato2), 1, Scalar(255, 0, 0), 3);
                       if(cont % 2 == 0)
                               points1[index1++] = miPunto;
                       else
                               points2[index2++] = miPunto;
                       
                       cont++;
                       previousPoint = miPunto;
    }                          
    fich.close();
    
    //imprimeVector(points1, cont);
    GeoEpi miGE(cont/2); 
    //GeoEpi miGE(cont/2, points1, points2);
    miGE.setDataP1(points1);
    miGE.setDataP2(points2);
    
    /*cvSaveImage("img/resultado2.jpg", _img);
    
    cvNamedWindow("MI_VENTANA", CV_WINDOW_AUTOSIZE);
    cvShowImage("MI_VENTANA", _img);
    
    cvWaitKey(0);
    
    cvReleaseImage(&_img);
    
    cvDestroyWindow("MI_VENTANA");*/
    
    /*for (int i = 0; i < tam; i++){
         cvSetReal2D(_p1, 0, i, point[i].x);
         cvSetReal2D(, 1, i, point[i].y);
     }*/
   
    //std::cout << "TAMAÑO: " << Point(img.size().width, img.size().height) << std::endl;
    
    //_p1 = miGE.getP1;
    //_p2 = miGE.getP2;
    miGE.fundMat();    
        
    system("PAUSE");
    return EXIT_SUCCESS;
}




void drawLine(CvPoint p1, CvPoint p2){
       cvLine(_img, p1, p2, _color2);        
                   if (_rojo){
                      _color2 = CV_RGB(0, 255, 0);
                      _rojo = false;
                   }
                   else{
                      _color2 = CV_RGB(255, 0, 0);
                      _rojo = true;
                   }
}




void imprimeVector(CvPoint points[], unsigned short int cont){
     for (int i = 0; i < cont; i++){
         std::cout << "PUNTO Nº " << i << ": " << std::endl;
         std::cout << "\t" << points[i].x << std::endl;
         std::cout << "\t" << points[i].y << std::endl;
     }
}
