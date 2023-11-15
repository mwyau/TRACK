#include <Stdio.h>
#include <stdlib.h>
#include <Math.h>
#include <string.h>
#include "splice.h"
#include "vec.h"
#include "m_values.h"
#include "mem_er.h"

/* function to rotate grid to shear direction with interpolation. */

double dirangle(struct tot_tr * , double * , double * , int , int , int , int );
float bicubic_intrp(float * , float , float , float * , float *, int , int );
float linear_intrp(float * , float , float *, int , int );

void dir_rot(struct tot_tr *altr, VEC *vecg, float *srad, float *stmp, float *slng, float *slat, int nx, int ny, int ifld, int ndsmth, int ifdir, int ifdirp, int igsmp_typ)
{
    int i=0, j=0, ii=0;
    
    double arotd=0.0, angr=0.0;
    double cc1=0.0, ss1=0.0;

    double xx=0.0, yy=0.0;
    double norm=0.0;
    
    float *atmp=NULL;

    VEC pt1={0.0, 0.0, 0.0};
    VEC vt1, vt2, vt3, vn;
    VEC newv={0.0, 0.0, 0.0};

    if(!igsmp_typ){
      pt1.x = 0.0;
      pt1.y = 0.0;
      pt1.z = 1.0;
    }
    else {
      pt1.x = 1.0;
      pt1.y = 0.0;
      pt1.z = 0.0;
    }
     
    arotd =   dirangle(altr, &cc1, &ss1, ifld, ndsmth, ifdir, ifdirp);

    angr = arotd;

    sincos(angr, &ss1, &cc1);

    for(i=0; i < ny; i++){

       for(j=0; j < nx; j++){
          ii = i * nx + j;
	  atmp = stmp + ii;
	  *atmp = 0.0;
          memcpy(&newv, vecg+ii, sizeof(VEC));
          norm = dotp(&newv, &pt1);
          crosp(&newv, &pt1, &vt1);
          mulv(&vt1, &vt1, ss1);
          mulv(&pt1, &vt2, ((1.0 - cc1) * norm));
          mulv(&newv, &vt3, cc1);
          addv(&vt1, &vt2, &vn);
          addv(&vn, &vt3, &newv);

          yy = (FP_PI2 - acos(newv.z)) / FP_PI;
          if(yy > 90.0) yy = 90.0;
          else if(yy < -90.0) yy = -90.0;
          xx = atan2(newv.y, newv.x) / FP_PI;
          if(!igsmp_typ && xx < 0.0) xx = 360 + xx;

          if(!igsmp_typ){
             *atmp = linear_intrp(srad, (float)xx, slng, i, nx);
          }
          else {
             *atmp = bicubic_intrp(srad, (float)xx, (float)yy, slng, slat, nx, ny);
          }
       }
    }
    
    for(i=0; i < nx*ny; i++){
       *(srad + i) = *(stmp + i);
    }

    return;
}

