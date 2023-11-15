#include <Stdio.h>
#include <Math.h>
#include <stdlib.h>
#include "splice.h"

/* function to perform bi-cubic interpolation taking account of missing values. */

/* macros for cubic splice function evaluations */

#define  f2(X) ((X * X - 1.0) * (X - 2.0) * 0.5)
#define  f3(X) (-(X + 1.0) * (X - 2.0) * X * 0.5)
#define  f4(X) ((X * X - 1.0) * X  / 6.0)

#define   TOLIVAL    1.0e-3

int qsearch(float * , float , int , int * , int * );

extern int pb;

float bicubic_intrp(float *ap, float xx, float yy, float *xg, float *yg, int nx, int ny)
{

   int ix1=0, ix2=0, iy1=0, iy2=0;

   float val=0.0;
   float p0[4], p1[4], p2[4], p3[4];
   float xd, y3[4], yd[3];
   float pp[4];

/* find neighbours */

   if(xx > *(xg + nx - 1) || xx < *xg) return ADD_UNDEF;

   if(yy > *(yg + ny - 1) || yy < *yg) return ADD_UNDEF;

   qsearch(xg, xx, nx, &ix1, &ix2);
   qsearch(yg, yy, ny, &iy1, &iy2);

   if(pb == 'n'){
     if(ix1 - 1 < 0) return ADD_UNDEF;
     else if(ix2 + 1 >= nx) return ADD_UNDEF;
   }

   if(iy1 - 1 < 0) return ADD_UNDEF;
   else if(iy2 + 1 >= ny) return ADD_UNDEF;

   if(pb == 'y'){
     if(ix1 - 1 < 0) {
       p0[0] = *(ap + (iy1 - 1) * nx + nx - 2);
       p1[0] = *(ap + iy1 * nx + nx - 2);
       p2[0] = *(ap + iy2 * nx + nx - 2);
       p3[0] = *(ap + (iy2 + 1) * nx + nx - 2);
     }
     else{
       p0[0] = *(ap + (iy1 - 1) * nx + ix1 - 1);
       p1[0] = *(ap + iy1 * nx + ix1 - 1);
       p2[0] = *(ap + iy2 * nx + ix1 - 1);
       p3[0] = *(ap + (iy2 + 1) * nx + ix1 - 1);
     }
     if(ix2 + 1 >= nx){
       p0[3] = *(ap + (iy1 - 1) * nx + 1);
       p1[3] = *(ap + iy1 * nx + 1);
       p2[3] = *(ap + iy2 * nx + 1);
       p3[3] = *(ap + (iy2 + 1) * nx + 1);
     }
     else {
       p0[3] = *(ap + (iy1 - 1) * nx + ix2 + 1);
       p1[3] = *(ap + iy1 * nx + ix2 + 1);
       p2[3] = *(ap + iy2 * nx + ix2 + 1);
       p3[3] = *(ap + (iy2 + 1) * nx + ix2 + 1);
     }
   }
   else{
       p0[0] = *(ap + (iy1 - 1) * nx + ix1 - 1);
       p1[0] = *(ap + iy1 * nx + ix1 - 1);
       p2[0] = *(ap + iy2 * nx + ix1 - 1);
       p3[0] = *(ap + (iy2 + 1) * nx + ix1 - 1);
       p0[3] = *(ap + (iy1 - 1) * nx + ix2 + 1);
       p1[3] = *(ap + iy1 * nx + ix2 + 1);
       p2[3] = *(ap + iy2 * nx + ix2 + 1);
       p3[3] = *(ap + (iy2 + 1) * nx + ix2 + 1);
   }

   p0[1] = *(ap + (iy1 - 1) * nx + ix1);
   p0[2] = *(ap + (iy1 - 1) * nx + ix2);
   p1[1] = *(ap + iy1 * nx + ix1);
   p1[2] = *(ap + iy1 * nx + ix2);
   p2[1] = *(ap + iy2 * nx + ix1);
   p2[2] = *(ap + iy2 * nx + ix2);
   p3[1] = *(ap + (iy2 + 1) * nx + ix1);
   p3[2] = *(ap + (iy2 + 1) * nx + ix2);

   if(p0[0] > ADD_CHECK || p0[1] > ADD_CHECK || p0[2] > ADD_CHECK || p0[3] > ADD_CHECK ) return ADD_UNDEF;
   else if(p1[0] > ADD_CHECK || p1[1] > ADD_CHECK || p1[2] > ADD_CHECK || p1[3] > ADD_CHECK ) return ADD_UNDEF;
   else if(p2[0] > ADD_CHECK || p2[1] > ADD_CHECK || p2[2] > ADD_CHECK || p2[3] > ADD_CHECK ) return ADD_UNDEF;
   else if(p3[0] > ADD_CHECK || p3[1] > ADD_CHECK || p3[2] > ADD_CHECK || p3[3] > ADD_CHECK ) return ADD_UNDEF;

   xd = (xx - *(xg + ix1)) / (*(xg + ix2) - *(xg + ix1));

   y3[0] = *(yg + iy1 - 1);
   y3[1] = *(yg + iy1);
   y3[2] = *(yg + iy2);
   y3[3] = *(yg + iy2 + 1);

   yd[0] = (yy - y3[0]) * (yy - y3[2]) * (yy - y3[3]) / ((y3[1] - y3[0]) * (y3[1] - y3[2]) * (y3[1] - y3[3]));
   yd[1] = (yy - y3[0]) * (yy - y3[1]) * (yy - y3[3]) / ((y3[2] - y3[0]) * (y3[2] - y3[1]) * (y3[2] - y3[3]));
   yd[2] = (yy - y3[0]) * (yy - y3[1]) * (yy - y3[2]) / ((y3[3] - y3[0]) * (y3[3] - y3[1]) * (y3[3] - y3[2]));   


   pp[0] = p0[1] + xd * (p0[2] - p0[1]);
   pp[1] = p1[0] + f2(xd) * (p1[1] - p1[0]) + f3(xd) * (p1[2] - p1[0]) + f4(xd) * (p1[3] - p1[0]);
   pp[2] = p2[0] + f2(xd) * (p2[1] - p2[0]) + f3(xd) * (p2[2] - p2[0]) + f4(xd) * (p2[3] - p2[0]);
   pp[3] = p3[1] + xd * (p3[2] - p3[1]);

   val = pp[0] + yd[0] * (pp[1] - pp[0]) + yd[1] * (pp[2] - pp[0]) + yd[2] * (pp[3] - pp[0]);
    
   return val;
}

