#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <highgui.h>
#include "geoEpi.h"


typedef unsigned short int USHORT;

CvScalar _color = CV_RGB(0,255,0);
CvScalar _color2 = CV_RGB(255,0,0);
IplImage *_img, *_img2;
GeoEpi* _miGE;

std::string _imageName("img/parte1.jpg");
std::string _imageName2("img/parte2.jpg");
    

void CallBackMouse(int event, int x, int y, int flags, void *ptr);
void CallBackMouse2( int event, int x, int y, int flags, void* param );
void processing(CvPoint p, USHORT image);
void drawPoints(CvPoint points[], USHORT index, USHORT n);
void drawPoint(CvPoint p, USHORT opc);


int main(int argc, char *argv[])
{
    USHORT cont = 0, si = 0;
    
    _img = cvLoadImage(argc == 2 ? argv[1] : _imageName.c_str(), 1);
    _img2 = cvLoadImage(argc == 3 ? argv[2] : _imageName2.c_str(), 1);
    
    std::ifstream fich;
    fich.open(argc == 3 ? argv[3] : "documentation/correspondencias.txt", std::ios::in);
    if(fich.fail())
          std::cerr << "ERROR opening the file" << std::endl;
    
    std::string dato1, dato2;
    
    CvPoint miPunto, previousPoint;
    CvPoint points1[500], points2[500];
            
    USHORT index1, index2;
    index1 = index2 = 0;
    
    /**store the points in different vectors*/
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
    
    /*drawPoints(points1, index1, 1);
    drawPoints(points2, index1, 2);*/
     
    /**Generate the fundamental matrix*/
    _miGE->fundMat();  
    _miGE->printFundMat();
            
    /**create windows for images*/
    cvNamedWindow("MI_VENTANA", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("MI_VENTANA2", CV_WINDOW_AUTOSIZE);
    
    /**capture mouse click*/
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





/** Capture mouse click image 1
* parameter event        type of event that is generated (1 -> left click)
* parameter x, y         point coordinates
* parameter flags
* parameter ptr          coordinates's point
*/
void CallBackMouse(int event, int x, int y, int flags, void *ptr){
     if (event == 1){
         CvPoint *p = (CvPoint*)ptr;
         p->x = x;
         p->y = y;
         
         processing(*p, 1);

         
     }
}




/** Capture mouse click image 1*/
void CallBackMouse2(int event, int x, int y, int flags, void *ptr){
     if(event == 1){          
         CvPoint *p = (CvPoint*)ptr;
         p->x = x;
         p->y = y;
         
         processing(*p, 2);
     }
}


/**Calculate the epi line since pick onClick point
* image                 is create a new image variable for see only
*                       the new epipolar line
*/
void processing(CvPoint p, USHORT image){
     _miGE->setPoint(p);
                  
     CvPoint p1, p2;
                
     if (image == 1){
         _miGE->drawEpiLines(*_img, p, p1, p2, 1);
         
         IplImage *image = cvLoadImage(_imageName2.c_str(), 1);
                  
         cvLine(image, p1, p2, _color);
         cvShowImage("MI_VENTANA2", image);
     }
     else{
         _miGE->drawEpiLines(*_img2, p, p1, p2, 2);
         
         IplImage *image = cvLoadImage(_imageName.c_str(), 1);
         
         cvLine(image, p1, p2, _color);
         cvShowImage("MI_VENTANA", image);
     }
}



/**Draw vector's points
*parameter points[]    vector points
*parameter index       vector size
*parameter n           image*/
void drawPoints(CvPoint points[], USHORT index, USHORT n){
     for (int i = 0; i < index; i++)
         drawPoint(points[i], n);     
}


/**Draw one point
*parameter p     point
*parameter opc   image*/
void drawPoint(CvPoint p, USHORT opc){ 
     if (opc == 1){
         cvCircle(_img, p, 1, _color2, 3);
         cvShowImage("MI_VENTANA", _img);
     }
     else{
         cvCircle(_img2, p, 1, _color2, 3);
         cvShowImage("MI_VENTANA2", _img2);
     }
}
