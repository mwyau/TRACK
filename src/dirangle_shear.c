#include <Stdio.h>
#include <stdlib.h>
#include <Math.h>
#include "splice.h"
#include "vec.h"
#include "m_values.h"

/* function to determine rotation angle to align with shear vector */ 

double dirangle_shear(struct tot_tr *altr, double *cn, double *sn, int pt_id, int ndsmth, int ifx, int ify)
{
   int i=0;
   int n2, st=0, en=0;
   int narot=0;
   
   float vxm=0.0, vym=0.0;

   double aa=0.0, arot=0.0, norm=0.0;
   double dp=0.0;
   double xx, yy;
   double s1, s2, c1, c2;

   struct fet_pt_tr *atr=NULL;

   VEC vsdir={0.0, 0.0, 0.0};
   VEC vshr={0.0, 0.0, 0.0};
   VEC vx={0.0, 0.0, 0.0}, vy={0.0, 0.0, 0.0};
   VEC pt1, vtmp, vt1, vt2, vt3, vn;

   n2 = ndsmth / 2;
   
   atr = altr->trpt + pt_id;

   if(ndsmth % 2) {st = pt_id - n2; en = pt_id + n2;}
   else {st = pt_id - n2; en = pt_id + n2 - 1;}

   if(st < 0) st = 0;
   if(en > altr->num - 1) en = altr->num - 1;
   if(st > en) st = en;

   for(i = st; i <= en; i++){
       atr = altr->trpt + i;
   
       xx = atr->xf * FP_PI;
       yy = FP_PI2 - atr->yf * FP_PI;
       if(yy < 0.) yy = 0.0;

       sincos(xx, &s1, &c1);
       sincos(yy, &s2, &c2);
       vsdir.x = -c1 * c2;
       vsdir.y = -s1 * c2;
       vsdir.z = s2;

/* determine shear vector */

       vxm = *(atr->add_fld + ifx);
       vym = *(atr->add_fld + ify);

       vx.x = -s1;
       vx.y = c1;
   
       vy.x = vsdir.x;
       vy.y = vsdir.y;
       vy.z = vsdir.z;
   
       mulv(&vx, &vx, vxm);
       mulv(&vy, &vy, vym);
       addv(&vx, &vy, &vshr);
       norm = sqrt(dotp(&vshr, &vshr));
       normv(&vshr, norm);
   
       if(norm <= 0.0) continue;

/* compute rotation angle */

       dp = dotp(&vsdir, &vshr);
       if(dp > 1.0) dp = 1.0;
       else if(dp < -1.0) dp = -1.0;
       aa = acos(dp);

/* compute which side of tvec that vsdir is for anticlockwise rotation */

       pt1.x = atr->pp[0];
       pt1.y = atr->pp[1];
       pt1.z = atr->pp[2];
       crosp(&pt1, &vshr, &vtmp);

       norm = dotp(&vtmp, &vsdir);
       if(norm < 0.0) aa *= -1.0;
       sincos(aa, &s1, &c1);
   
/* rotate standard direction for check */
       norm = dotp(&vsdir, &pt1);
       crosp(&vsdir, &pt1, &vt1);
       mulv(&vt1, &vt1, s1);
       mulv(&pt1, &vt2, ((1.0 - c1) * norm));
       mulv(&vsdir, &vt3, c1);
       addv(&vt1, &vt2, &vn);
       addv(&vn, &vt3, &vn);   

       if(fabs(dotp(&vn, &vshr) - 1.0) > 1.0e-4){
          printf("****ERROR****, orientating radial grid to shear direction incorrect, dotp=%e > 1.0e-4.\n\n", fabs(dotp(&vn, &vshr) - 1.0));
          exit(1);
       }
       
       arot += aa;
       ++narot;       
       
   }
   
   if(narot == 0) return ADD_UNDEF;

   arot /= narot;

   sincos(arot, sn, cn);

   return arot;
   
}
