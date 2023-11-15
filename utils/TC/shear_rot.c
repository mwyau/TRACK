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
double dirangle_shear(struct tot_tr * , double * , double * , int , int , int , int );
float bicubic_intrp(float * , float , float , float * , float *, int , int );
float linear_intrp(float * , float , float *, int , int );
int rot_check(double , double , int );
int qsearch(float * , float , int , int * , int * );

void shear_rot(struct tot_tr *altr, VEC *vecg, float *srad, float *stmp, float *slng, float *slat, int nx, int ny, int ifld, int ndsmth, int ifdir, int ifdirp, int ifshx, int ifshy, int igsmp_typ)
{
    int i=0, j=0, ii=0;
    int rok=0;
    
    double arotd=0.0, arots=0.0, angr=0.0;
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

    arots = dirangle_shear(altr, &cc1, &ss1, ifld, ndsmth, ifshx, ifshy);

    angr = fabs(arots - arotd);
    if(angr > FPI) angr = FPI2 - angr;

    rok = rot_check(arotd, arots, igsmp_typ);

    angr *= -(float) rok;

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

/* function to check vector rotation is correct */

int rot_check(double arotd, double arots, int igsmp_typ)
{
    int rok=0;

    double angr=0.0, angn=0.0;
    double dp=0.0;
    double c1=0.0, s1=0.0;
    double norm=0.0;

    VEC pt1={0.0, 0.0, 0.0};
    VEC vsdir={0.0, 0.0, 0.0};
    VEC vt1, vt2, vt3, vnd, vns, vtmp;

    angr = fabs(arots - arotd);
    if(angr > FPI) angr = FPI2 - angr;

    if(!igsmp_typ){
      pt1.x = 0.0;
      pt1.y = 0.0;
      pt1.z = 1.0;
      vsdir.x = -1.0;
      vsdir.y = 0.0;
      vsdir.z = 0.0;
    }
    else {
      pt1.x = 1.0;
      pt1.y = 0.0;
      pt1.z = 0.0;
      vsdir.x = 0.0;
      vsdir.y = 0.0;
      vsdir.z = 1.0;
    }

    sincos(arotd, &s1, &c1);

/* rotate standard direction to propgation direction */
     norm = dotp(&vsdir, &pt1);
     crosp(&vsdir, &pt1, &vt1);
     mulv(&vt1, &vt1, s1);
     mulv(&pt1, &vt2, ((1.0 - c1) * norm));
     mulv(&vsdir, &vt3, c1);
     addv(&vt1, &vt2, &vnd);
     addv(&vnd, &vt3, &vnd);

     sincos(arots, &s1, &c1);

/* rotate standard direction to shear direction */
/*     norm = dotp(&vsdir, &pt1); */
     crosp(&vsdir, &pt1, &vt1);
     mulv(&vt1, &vt1, s1);
     mulv(&pt1, &vt2, ((1.0 - c1) * norm));
     mulv(&vsdir, &vt3, c1);
     addv(&vt1, &vt2, &vns);
     addv(&vns, &vt3, &vns);

     dp = dotp(&vns, &vnd);
     if(dp > 1.0) dp = 1.0;
     else if(dp < -1.0) dp = -1.0; 
     angn = acos(dp);

     if(fabs(angr - angn) > 1.0e-4) {
       printf("%f %f\n", arots, arotd);
       printf("Inconsistent rotation angle, %f %f\n", angr, angn);
       exit(1); 
     }

/* check which side shear vector is to direction vector */

     crosp(&pt1, &vns, &vtmp);
     norm = dotp(&vtmp, &vnd);
     rok = (norm < 0.0) ? 1 : -1;

     sincos(angr * (float)rok, &s1, &c1);

/* rotate shear direction to propagation direction for check */
     norm = dotp(&vns, &pt1);
     crosp(&vns, &pt1, &vt1);
     mulv(&vt1, &vt1, s1);
     mulv(&pt1, &vt2, ((1.0 - c1) * norm));
     mulv(&vns, &vt3, c1);
     addv(&vt1, &vt2, &vtmp);
     addv(&vtmp, &vt3, &vtmp);   

     if(fabs(dotp(&vtmp, &vnd) - 1.0) > 1.0e-4){
        printf("****ERROR****, orientating radial grid to shear direction incorrect, dotp=%e > 1.0e-4.\n\n", fabs(dotp(&vtmp, &vnd) - 1.0));
        exit(1);
     }

     return rok;
}

float linear_intrp(float *ap, float xx, float *xg, int iy, int nx)
{

   int ix1=0, ix2=0;

   static int dx=-1.0;

   float val=0.0;
   float p1=0.0, p2=0.0;
   float xd=0.0;

   if(dx < 0.0) dx = *(xg + 1) - *xg;

/* find neighbours */

   if(xx > *(xg + nx - 1)){
     if(xx - *(xg + nx - 1) <= dx) {
       p1 = *(ap + iy * nx + nx - 1);
       p2 = *(ap + iy * nx);
       if(p1 > ADD_CHECK || p2 > ADD_CHECK) return ADD_UNDEF;
       xd = (xx - *(xg + nx - 1)) / dx;
     }
     else{
       return ADD_UNDEF;
     }
   }

   else if(xx < *xg) {
     if(*xg - xx <= dx) {
       p1 = *(ap + iy * nx + nx - 1);
       p2 = *(ap + iy * nx);
       if(p1 > ADD_CHECK || p2 > ADD_CHECK) return ADD_UNDEF;
       xd = (dx - *xg + xx) / dx;
     }
     else {
        return ADD_UNDEF;
     }
   }

   else {
      qsearch(xg, xx, nx, &ix1, &ix2);

      p1 = *(ap + iy * nx + ix1);
      p2 = *(ap + iy * nx + ix2);

      if(p1 > ADD_CHECK || p2 > ADD_CHECK) return ADD_UNDEF; 

      xd = (xx - *(xg + ix1)) / dx;
   }

   val = p1 + xd * (p2 - p1);

   return val;
}
