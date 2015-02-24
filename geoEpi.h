#ifndef GEOEPI_H
#define GEOEPI_H

#include <iostream>
#include <vector>
#include <cv.h>
#include <string.h>

class GeoEpi{
  private:
      CvMat *points1, *points2;             //points
      CvMat *point;
      CvMat *status, *fundamental_matrix;   //status and fundamental matrix
      CvMat *lines1, *lines2;               //Corresponding lines
 
      int tam;
  public:
      GeoEpi(){}
      GeoEpi(unsigned short int);
      ~GeoEpi();
      
      void setDataP1(CvPoint []);
      void setDataP2(CvPoint []);
      void setPoint(CvPoint);
      CvMat* getP1() const;
      CvMat* getP2() const;
      CvMat* getL1() const;
      CvMat* getL2() const;
      double getCorrLines(unsigned short int, unsigned short int) const;
      double getFunMat(unsigned short int, unsigned short int) const;
      
      void fundMat();
      void directionLines(unsigned short int);
      
      void printFundMat() const;
      void drawEpiLines(IplImage &, CvPoint, CvPoint &, CvPoint &, unsigned short int);
};
#endif
