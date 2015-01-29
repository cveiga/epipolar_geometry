#include <iostream>

#include "geoEpi.h"



GeoEpi::GeoEpi(unsigned short int tam){
     std::cout << "TAMAÑO: " << tam << std::endl;
     
     this->tam = tam;
     
     points1 =  cvCreateMat(2, this->tam, CV_32FC1);
     points2 = cvCreateMat(2, this->tam, CV_32FC1);        
     
     status = cvCreateMat(1, this->tam, CV_32FC1);    
     fundamental_matrix = cvCreateMat(3, 3, CV_32FC1);
     
     corrLines = cvCreateMat(3, this->tam, CV_32FC1);
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
         cvSetReal2D(points1, 0, i, point[i].x);
         cvSetReal2D(points1, 1, i, point[i].y);
     }
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



double GeoEpi::getCorrLines(unsigned short int row, unsigned short int col) const{
      return cvmGet(corrLines, row, col);
}



double GeoEpi::getFunMat(unsigned short int row, unsigned short int col) const{
       return cvmGet(fundamental_matrix, row, col);
}



/** \parametro 5              desviación */
void GeoEpi::fundMat(){
    if(cvFindFundamentalMat(points1, points2, fundamental_matrix, CV_FM_RANSAC, 1, 0.99, status))
        std::cout << "Fundamental matrix was found" << std::endl;
    else
        std::cout << "Fundamental matrix was not found" << std::endl;
}



void GeoEpi::directionLines(unsigned short int numImages){
     cvComputeCorrespondEpilines(points2, numImages, fundamental_matrix, corrLines);
}




void GeoEpi::printFundMat() const{
     std::cout << "  FUNDAMENTAL MATRIX" << std::endl;
     std::cout << "======================" << std::endl;
     std::cout << "(";
     
     for (int i = 0; i < 3; i++){
         for (int j = 0; j < 3; j++){
             std::cout << cvmGet(fundamental_matrix, i, j) << ", ";
         }
         std::cout << std::endl;
     }
     std::cout << ")" << std::endl;
}
