#include <iostream>

#include "geoEpi.h"



GeoEpi::GeoEpi(unsigned short int tam){
     this->tam = tam;
     
     points1 = cvCreateMat(2, this->tam, CV_32FC1);
     points2 = cvCreateMat(2, this->tam, CV_32FC1);     
     point   = cvCreateMat(2, 3, CV_32F);   
     
     status  = cvCreateMat(1, this->tam, CV_32FC1);    
     fundamental_matrix = cvCreateMat(3, 3, CV_32F);
     
     lines1  = cvCreateMat(3, 3, CV_32FC1);
     lines2  = cvCreateMat(3, 3, CV_32FC1);
}




GeoEpi::~GeoEpi(){
    delete points1;
    delete points2;
    delete point;
    
    delete lines1;
    delete lines2;
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



void GeoEpi::setPoint(CvPoint p){
     cvmSet(point, 0, 0, p.x);
     cvmSet(point, 1, 0, p.y);
}



CvMat* GeoEpi::getP1() const{
      return points1;      
}



CvMat* GeoEpi::getP2() const{
      return points2;      
}



CvMat* GeoEpi::getL1() const{
       return lines1;
}



CvMat* GeoEpi::getL2() const{
       return lines2;
}



double GeoEpi::getCorrLines(unsigned short int row, unsigned short int col) const{
      return cvmGet(lines2, row, col);
}




double GeoEpi::getFunMat(unsigned short int row, unsigned short int col) const{
       return cvmGet(fundamental_matrix, row, col);
}




/** \parameter 5              desviation */
void GeoEpi::fundMat(){
    if(cvFindFundamentalMat(points1, points2, fundamental_matrix, CV_FM_RANSAC, 1, 0.99, status))
        std::cout << "Fundamental matrix was found" << std::endl;
    else
        std::cout << "Fundamental matrix was not found" << std::endl;
}




/**Specification the direction for calculate the epi lines
* \parameter 2   number of imagenes
*/
void GeoEpi::directionLines(unsigned short int num){
     if (num == 1)
          cvComputeCorrespondEpilines(point, 2, fundamental_matrix, lines2);
     else 
          cvComputeCorrespondEpilines(point, 1, fundamental_matrix, lines1);
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




void GeoEpi::drawEpiLines(IplImage &img, CvPoint n, CvPoint &p1, CvPoint &p2, unsigned short int nImage){
     float a, b, c;
     float m, m2;
          
     this->directionLines(nImage);
     
     if (nImage == 1){
         a = cvmGet(lines2, 0, 0);      
         b = cvmGet(lines2, 1, 0);  
         c = cvmGet(lines2, 2, 0);
     }
     else{
         a = cvmGet(lines1, 0, 0);      
         b = cvmGet(lines1, 1, 0);      
         c = cvmGet(lines1, 2, 0);
     }   
             
     p1.x = 0;
     p1.y = ceil(-c/b);
     p2.x = img.width;
     p2.y = ceil(-(c + abs(a*img.width)) / b);
}
