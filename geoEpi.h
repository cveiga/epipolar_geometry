#ifndef GEOEPI_H
#define GEOEPI_H

#include <iostream>
#include <vector>
#include <cv.h>
#include <string.h>

class GeoEpi{
  private:
      CvMat *points1, *points2;             //puntos
      CvMat *status, *fundamental_matrix;   //matriz fundamental
      CvMat *corrLines;                     //Lineas correspondientes
      
      int tam;
  public:
      GeoEpi(){}
      GeoEpi(unsigned short int);
      GeoEpi(unsigned short int, CvPoint [], CvPoint []);
      ~GeoEpi();
      
      void setDataP1(CvPoint []);
      void setDataP2(CvPoint []);
      CvMat* getP1() const;
      CvMat* getP2() const;
      double getCorrLines(unsigned short int, unsigned short int) const;
      
      CvMat fundMat();
      void GeoEpi::directionLines(unsigned short int);
};
#endif
