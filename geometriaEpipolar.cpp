#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <highgui.h>
#include "geoEpi.h"


typedef unsigned short int USHORT;

CvScalar _color2 = CV_RGB(0,255,0);
CvScalar _color3 = CV_RGB(255,0,0);
bool _rojo = false;
IplImage *_img, *_img2;
GeoEpi* _miGE;

std::string _imageName("img/parte1.jpg");
std::string _imageName2("img/parte2.jpg");
    

void drawLine(CvPoint p1, CvPoint p2, USHORT option);
void CallBackMouse(int event, int x, int y, int flags, void *ptr);
void CallBackMouse2( int event, int x, int y, int flags, void* param );
void processing(CvPoint p, USHORT image);
void pintaPuntos(CvPoint points[], USHORT index, USHORT n);
void drawPoint(CvPoint p, USHORT opc);


int main(int argc, char *argv[])
{
    USHORT cont = 0, si = 0;
    //std::string imageName("img/Art_Institute_of_Chicago_Lion_Statue.jpg");
    
    _img = cvLoadImage(argc == 2 ? argv[1] : _imageName.c_str(), 1);
    _img2 = cvLoadImage(argc == 3 ? argv[2] : _imageName2.c_str(), 1);
    
    std::ifstream fich;
    fich.open(argc == 3 ? argv[3] : "documentation/correspondencias.txt", std::ios::in);
    if(fich.fail())
                    std::cerr << "ERROR al abrir el archivo" << std::endl;
    
    std::string dato1, dato2;
    
    CvPoint miPunto, previousPoint;
    CvPoint points1[500], points2[500];
    
    CvScalar color = CV_RGB(255, 0, 0 );
        
    USHORT index1, index2;
    index1 = index2 = 0;
    
    while(!fich.eof()){
                       fich >> dato1;
                       fich >> dato2;
                       
                       miPunto.x = atoi(dato1.c_str());
                       miPunto.y = atoi(dato2.c_str());
                       
                       if(cont % 2 == 0)
                               points1[index1++] = miPunto;
                       else
                               points2[index2++] = miPunto;
                       
                       cont++;
                       previousPoint = miPunto;
    }                          
    fich.close();
    
    _miGE = new GeoEpi(index1); 
    
    _miGE->setDataP1(points1);
    _miGE->setDataP2(points2);
    
    pintaPuntos(points1, index1, 1);
    pintaPuntos(points2, index1, 2);
     
    //Generate the fundamental matrix
    _miGE->fundMat();  
    _miGE->printFundMat();
        
    //cvSaveImage("img/resultado2.jpg", _img);
    
    //create windows for images
    cvNamedWindow("MI_VENTANA", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("MI_VENTANA2", CV_WINDOW_AUTOSIZE);
    
    //capture mouse click
    CvPoint p;
    cvSetMouseCallback("MI_VENTANA", CallBackMouse, &p);
    cvSetMouseCallback("MI_VENTANA2", CallBackMouse2, &p);
    
    cvShowImage("MI_VENTANA", _img);
    cvShowImage("MI_VENTANA2", _img2);
    
    cvWaitKey(0);
    
    cvReleaseImage(&_img);
    cvReleaseImage(&_img2);
    
    cvDestroyWindow("MI_VENTANA");
    cvDestroyWindow("MI_VENTANA2");
        
    system("PAUSE");
    return EXIT_SUCCESS;
}




void drawLine(CvPoint p1, CvPoint p2, USHORT option){
     std::cout << "Punto1 = P.X: " << p1.x << ", P.Y: " << p1.y << std::endl;
     std::cout << "Punto2 = P.X: " << p2.x << ", P.Y: " << p2.y << std::endl;
     
     if (option == 1)
          cvLine(_img, p1, p2, _color2); 
     else
          cvLine(_img2, p1, p2, _color2);
     
     if (_rojo){
        _color2 = CV_RGB(0, 255, 0);
        _rojo = false;
     }
     else{
          _color2 = CV_RGB(255, 0, 0);
          _rojo = true;
     }
                   
     cvShowImage("MI_VENTANA", _img);
}





/** Capturar los eventos del ratón
* parametro event        tipo de evento que se generea (1 -> click izquierdo)
* parametro x, y         coordenadas del punto
* parametro flags
* parametro ptr          punto de coordenadas
*/
void CallBackMouse(int event, int x, int y, int flags, void *ptr){
     if (event == 1){
         CvPoint *p = (CvPoint*)ptr;
         //CvPoint *p2 = (CvPoint*)ptr;
         //CvMat *p = cvCreateMat(2, 1, CV_32FC1);
         p->x = x;
         p->y = y;
         
         std::cout << "Left point (" << x << ", " << y << ")" << std::endl;
         
         processing(*p, 1);

         
     }
}




void CallBackMouse2(int event, int x, int y, int flags, void *ptr){
     if(event == 1){          
         CvPoint *p = (CvPoint*)ptr;
         p->x = x;
         p->y = y;
         
         std::cout << "Right point (" << x << ", " << y << ")" << std::endl;
         
         processing(*p, 2);
     }
}


/**
* image                 se crea una nueva variable imagen para que solo se muestre
*                       la nueva linea epipolar
*/
void processing(CvPoint p, USHORT image){
     _miGE->setPoint(p);
                  
     CvPoint p1, p2;
                
     if (image == 1){
         _miGE->drawEpiLines(*_img, p, p1, p2, 1);
         
         IplImage *image = cvLoadImage(_imageName2.c_str(), 1);
                  
         std::cout << "PUNTO1 EQUIVALENTE: (" << p1.x << ", " << p1.y << ")" << std::endl;
         std::cout << "PUNTO2 EQUIVALENTE: (" << p2.x << ", " << p2.y << ")" << std::endl;
         cvLine(image, p1, p2, _color2);
         cvShowImage("MI_VENTANA2", image);
     }
     else{
         _miGE->drawEpiLines(*_img2, p, p1, p2, 2);
         
         IplImage *image = cvLoadImage(_imageName.c_str(), 1);
         
         std::cout << "PUNTO1 EQUIVALENTE: (" << p1.x << ", " << p1.y << ")" << std::endl;
         std::cout << "PUNTO2 EQUIVALENTE: (" << p2.x << ", " << p2.y << ")" << std::endl;
         cvLine(image, p1, p2, _color2);
         cvShowImage("MI_VENTANA", image);
     }
}




void pintaPuntos(CvPoint points[], USHORT index, USHORT n){
     for (int i = 0; i < index; i++)
         drawPoint(points[i], n);     
}



void drawPoint(CvPoint p, USHORT opc){ 
     if (opc == 1){
         cvCircle(_img, p, 1, _color3, 3);
         cvShowImage("MI_VENTANA", _img);
     }
     else{
         cvCircle(_img2, p, 1, _color3, 3);
         cvShowImage("MI_VENTANA2", _img2);
     }
}
