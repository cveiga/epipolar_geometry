#include <iostream>
#include <string>
#include <vector>
#include "geoEpi.h"


GeoEpi::GeoEpi(unsigned short int tam){
     std::cout << "TAMAÑO: " << tam << std::endl;
     
     this->tam = tam;
     
     points1 =  cvCreateMat(2, this->tam, CV_32FC1);
     points2 = cvCreateMat(2, this->tam, CV_32FC1);        
     
     status = cvCreateMat(1, this->tam, CV_32FC1);    
     fundamental_matrix = cvCreateMat(3, 3, CV_32F);
}


GeoEpi::GeoEpi(unsigned short int tam, CvPoint p1[], CvPoint p2[]){
     this->tam = tam;
     /*points1 = cvMat(2, tam, CV_32FC1, p1[0]);
     points2 = cvMat(2, tam, CV_32FC1, &p2[0]);*/
          
     std::cout << "OBJETO CREADO" << std::endl;
}

GeoEpi::~GeoEpi(){
    delete points1;
    delete points2;
    
    std::cout << "OBJETO DESTRUIDO" << std::endl;
}


void GeoEpi::setDataP1(CvPoint point[]){
     for (int i = 0; i < tam; i++){
         cvSetReal2D(points1, 0, i, point[i].x);    //static_cast<double>(point.x));
         cvSetReal2D(points1, 1, i, point[i].y);    //static_cast<double>(point.x));
     }
     //cvmSet(points1, 0, 0, static_cast<double>(point.x));   
     //std::cout << "Tamaño del vector 1: " << sizeof(points1)/sizeof(CvMat*)-1 << std::endl;
     //std::cout << "PUNTO 173: X = " << cvGetReal2D(points1, 0, 173) << ", Y = " << cvGetReal2D(points1, 1, 173) << std::endl;
}


void GeoEpi::setDataP2(CvPoint point[]){
     for (int i = 0; i < tam; i++){
         cvSetReal2D(points2, 0, i, point[i].x);
         cvSetReal2D(points2, 1, i, point[i].y);
     }
}


CvMat* GeoEpi::getP1() const{
      return points1;      
}


CvMat* GeoEpi::getP2() const{
      return points2;      
}


void GeoEpi::fundMat(){
    if(cvFindFundamentalMat(points1, points2, fundamental_matrix, CV_FM_RANSAC, 15, 0.99, status))
        std::cout << "Fundamental matrix was found" << std::endl;
    else
        std::cout << "Fundamental matrix was not found" << std::endl;
}
