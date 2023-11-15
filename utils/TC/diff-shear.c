#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <splice.h>
#include "mem_er.h"
#include "m_values.h"
#include "vec.h"

/* function to determine the angle between shear and direction along tracks */ 

struct tot_tr *read_tracks(FILE *, int *, int *, int *, int , float *, float * , float ** , int * );
void meantrd(FILE * , struct tot_tr * , int , int , int , int , float , float );
double dirangle(struct tot_tr * , double * , double * , int , int , int , int );
double dirangle_shear(struct tot_tr * , double * , double * , int , int , int , int );
void sincos(double , double * , double * );
void convert_track(struct tot_tr * , int , int , int );
float grwth(struct tot_tr * , int , int , int , int );


int noheader=0;

extern int nfld, nff;
extern int *nfwpos;

int main(void )
{
   int i=0, j=0;
   int trnum=0;
   int gpr=0, ipr=0;
   int ithw1=0, ithw2=0;
   int ith1=0, ith2=0;
   int ftype='s';
   int igr=0, igrwth=0;

   int ndsmth=0, gsmth=0;
   
   float alat=0.0, alng=0.0;
   float shm=0.0;
   
   double xx=0.0, yy=0.0;
   double ang=0.0;
   double s1, s2, c1, c2;
   double sn=0.0, cn=0.0;
   double norm=0.0, arot=0.0;
   double dp=0.0;

   VEC vsdir={0.0, 0.0, 0.0}, tvec={0.0, 0.0, 0.0}, svec={0.0, 0.0, 0.0};
   VEC pt1={0.0, 0.0, 0.0};
   VEC vt1, vt2, vt3, vn;
   VEC vtmp={0.0, 0.0, 0.0};

   FILE *fin=NULL, *fout=NULL;

   char ftrin[MAXCHR];
   
   struct tot_tr *tracks=NULL, *atr=NULL;
   struct fet_pt_tr *fp1=NULL ; 

   printf("What is the track file to read?\n\n");
   scanf("%s", ftrin);

   fin = fopen(ftrin, "r");
   if(!fin){
      printf("***ERROR***, unable to open file %s for 'r'\n\n", ftrin);
      exit(1);
   }

   tracks = read_tracks(fin, &trnum, &gpr, &ipr, 's', &alat, &alng, NULL, NULL);

   fclose(fin);

   convert_track(tracks, trnum, 0, 0);

   printf("Which two additional fields are required for the U and V of the shear?\n\n");
   scanf("%d %d", &ithw1, &ithw2);
   if((ithw1 <= 0 || ithw1 > nff) || (ithw2 <= 0 || ithw2 > nff)){
      printf("****ERROR****,incorrect values for additional fields, %d %d\n\n", ithw1, ithw2);
      exit(1);
   }
   

   ith1 = 0;
   for(i=0; i < ithw1; i++){
      if(*(nfwpos + i)) ith1 += 3;
      else ith1 += 1;
   }
   --ith1;
   
   ith2 = 0;
   for(i=0; i < ithw2; i++){
      if(*(nfwpos + i)) ith2 += 3;
      else ith2 += 1;
   }
   --ith2;  

   printf("Which intensity field should be used for tendency, use '0' for default?\n\n");
   scanf("%d", &igr);
   if(igr < 0 || igr > nff){
      printf("****ERROR****,incorrect value for additional field, %d\n\n", igr);
      exit(1);
   }

   if(igr){
       igrwth = 0;
       for(i=0; i < igr; i++){
         if(*(nfwpos + i)) igrwth += 3;
         else igrwth += 1;
      }
      --igrwth;
   }

   printf("Do you want to use the current point to determine system direction or average over several points\r\n"
          "to improve direction smoothness, input '1' for single point or 'n' the number of points to use.  \r\n"
          "This will depend on the track lifetime and minimum lifetime threshold.                           \n\n");
   scanf("%d", &ndsmth);
   if(ndsmth < 1){
      printf("****ERROR****, number of points cannot be zero or negative for direction smoothing, exiting.\n\n");
      exit(1); 
   }

   printf("Do you want to use the current point to determine growth rate or average over several points,\r\n"
          "input '1' for single point or 'n' the number of points to use.                               \n\n");
   scanf("%d", &gsmth);
   if(gsmth < 1){
      printf("****ERROR****, number of points cannot be zero or negative for growth rate smoothing, exiting.\n\n");
      exit(1);
   }
   
   nff += 3;
   nfld += 3;;
   
   nfwpos = (int *)realloc_n(nfwpos, nff*sizeof(int));
   mem_er((nfwpos == NULL) ? 0 : 1, nff * sizeof(int));
   *(nfwpos + nff - 1) = 0;
   *(nfwpos + nff - 2) = 0;
   *(nfwpos + nff - 3) = 0;
   
   for(i=0; i < trnum; i++){

       atr = tracks + i;
       
       for(j=0; j < atr->num; j++){
           fp1 = atr->trpt + j;
	   fp1->add_fld = (float *)realloc_n(fp1->add_fld, nfld * sizeof(float));
	   mem_er((fp1->add_fld == NULL) ? 0 : 1, nfld * sizeof(float));
       }


       for(j=0; j < atr->num; j++){
          fp1 = atr->trpt + j;

          xx = fp1->xf * FP_PI;
          yy = FP_PI2 - fp1->yf * FP_PI;

/* rotate standard vector (-1, 0, 0) */

          if(yy < 0.) yy = 0.0;
          sincos(xx, &s1, &c1);
          sincos(yy, &s2, &c2);
          vsdir.x = -c1 * c2;
          vsdir.y = -s1 * c2;
          vsdir.z = s2;

          arot = dirangle(atr, &cn, &sn, j, ndsmth, 0, 0);

          if(arot > ADD_CHECK) {fp1->add_fld[nfld-1] = ADD_UNDEF; continue;}

          pt1.x = fp1->pp[0];
          pt1.y = fp1->pp[1];
          pt1.z = fp1->pp[2];

          norm = dotp(&vsdir, &pt1);
          crosp(&vsdir, &pt1, &vt1);
          mulv(&vt1, &vt1, sn);
          mulv(&pt1, &vt2, ((1.0 - cn) * norm));
          mulv(&vsdir, &vt3, cn);
          addv(&vt1, &vt2, &vn);
          addv(&vn, &vt3, &tvec);

          arot = dirangle_shear(atr, &cn, &sn, j, ndsmth, ith1, ith2);

          norm = dotp(&vsdir, &pt1);
          crosp(&vsdir, &pt1, &vt1);
          mulv(&vt1, &vt1, sn);
          mulv(&pt1, &vt2, ((1.0 - cn) * norm));
          mulv(&vsdir, &vt3, cn);
          addv(&vt1, &vt2, &vn);
          addv(&vn, &vt3, &svec);

/* compute angle */
          dp = dotp(&svec, &tvec);
          if(dp > 1.0) dp = 1.0;
          else if(dp < -1.0) dp = -1.0;
          ang = acos(dp);

/* check which side of direction */
          crosp(&pt1, &tvec, &vtmp);
          norm = dotp(&vtmp, &svec);
          if(norm < 0.0) ang *= -1.0;

/* check for angle consistency */

          sincos(ang, &sn, &cn);
          norm = dotp(&svec, &pt1);
          crosp(&svec, &pt1, &vt1);
          mulv(&vt1, &vt1, sn);
          mulv(&pt1, &vt2, ((1.0 - cn) * norm));
          mulv(&svec, &vt3, cn);
          addv(&vt1, &vt2, &vn);
          addv(&vn, &vt3, &vn);

         if(fabs(dotp(&vn, &tvec) - 1.0) > 1.0e-4){
            printf("****ERROR****, incorrect alighnment of shear and direction vectors, dotp=%e > 1.0e-4.\n\n", fabs(dotp(&vn, &tvec) - 1.0));
            exit(1); 
          }

/*printf("%f\n", ang/FP_PI); 
printf("tvec %d %f %f %f\n", j, tvec.x, tvec.y, tvec.z);
printf("svec %d %f %f %f\n", j, svec.x, svec.y, svec.z);
printf("vn %d %f %f %f\n", j, vn.x, vn.y, vn.z);
printf("%f\n\n", dotp(&vn, &tvec)); */

          shm = sqrt(fp1->add_fld[ith1] * fp1->add_fld[ith1] + fp1->add_fld[ith2] * fp1->add_fld[ith2]);

          fp1->add_fld[nfld-3] = shm;
	  fp1->add_fld[nfld-2] = ang / FP_PI;
          fp1->add_fld[nfld-1] = grwth(atr, j, gsmth, igr, igrwth); 
	  
       }

   }
   
   fout = fopen("ff_trs.shear", "w");
   if(!fout){
      printf("***ERROR***, unable to open file %s for 'w'\n\n", "ff_trs.shear");
      exit(1);
   }

   meantrd(fout, tracks, trnum, ftype, gpr, ipr, alat, alng);

   fclose(fout);   

   
   for(i=0; i < trnum; i++){
      atr = tracks + i;
      if(nff){
         for(j=0; j < atr->num; j++) free((atr->trpt + j)->add_fld);
      }
      free(atr->trpt);
      
   }
   
   free(tracks);

   return 0;
}

double dirangle(struct tot_tr *altr, double *cn, double *sn, int pt_id, int ndsmth, int ifdir, int ifdirp)
{
   int i=0;
   int n2, st=0, en=0;
   int narot=0;

   double aa=0.0, arot=0.0, norm=0.0;
   double dp=0.0;
   double xx, yy;
   double s1, s2, c1, c2;

   struct fet_pt_tr *atr=NULL;

   VEC vsdir={0.0, 0.0, 0.0};
   VEC pt1, pt2, tvec, vtmp;
   VEC vt1, vt2, vt3, vn;

   n2 = ndsmth / 2;

   atr = altr->trpt + pt_id;

   if(altr->num <= 1){
      printf("****ERROR****, insufficient number of points in this track to determine direction.\n\n");
      exit(1);
   }

   if(ndsmth % 2) {st = pt_id - n2; en = pt_id + n2;}
   else {st = pt_id - n2; en = pt_id + n2 - 1;}

   if(st < 0) st = 0;
   if(en > altr->num - 2) en = altr->num - 2;
   if(st > en) st = en;

   for(i = st; i <= en; i++){
       atr = altr->trpt + i;

       if(ifdir){
          xx = *(atr->add_fld + ifdirp) * FP_PI;
          yy = FP_PI2 - *(atr->add_fld + ifdirp + 1) * FP_PI;
       }
       else {
          xx = atr->xf * FP_PI;
          yy = FP_PI2 - atr->yf * FP_PI;
       }
       if(yy < 0.) yy = 0.0;

       sincos(xx, &s1, &c1);
       sincos(yy, &s2, &c2);
       vsdir.x = -c1 * c2;
       vsdir.y = -s1 * c2;
       vsdir.z = s2;

       pt1.x = atr->pp[0];
       pt1.y = atr->pp[1];
       pt1.z = atr->pp[2];
       pt2.x = (atr+1)->pp[0];
       pt2.y = (atr+1)->pp[1];
       pt2.z = (atr+1)->pp[2];
       crosp(&pt1, &pt2, &vtmp);
       crosp(&vtmp, &pt1, &tvec);
       norm = sqrt(dotp(&tvec, &tvec));
       normv(&tvec, norm);

       if(norm <= 0.0) continue;

/* compute rotation angle */
       dp = dotp(&vsdir, &tvec);
       if(dp > 1.0) dp = 1.0;
       else if(dp < -1.0) dp = -1.0;
       aa = acos(dp);

/* compute which side of tvec that vsdir is for anticlockwise rotation */

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

       if(fabs(dotp(&vn, &tvec) - 1.0) > 1.0e-4){
         printf("****ERROR****, orientating radial grid to storm direction incorrect, dotp=%e > 1.0e-6.\n\n", fabs(dotp(&vn, &tvec) - 1.0));
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

float grwth(struct tot_tr *altr, int pt_id, int ndsmth, int ifdir, int ifdirp)
{
   int i=0;
   int n2, st=0, en=0;
   int nsamp=0;

   float grate=0.0;
   float fint1=0.0, fint2=0.0;

   struct fet_pt_tr *atr=NULL;

   n2 = ndsmth / 2;

   atr = altr->trpt + pt_id;

   if(altr->num <= 1){
      printf("****ERROR****, insufficient number of points in this track to determine growth rate.\n\n");
      exit(1);
   }

   if(ndsmth % 2) {st = pt_id - n2; en = pt_id + n2;}
   else {st = pt_id - n2; en = pt_id + n2 - 1;}

   if(st < 0) st = 0;
   if(en > altr->num - 2) en = altr->num - 2;
   if(st > en) st = en;

   for(i = st; i <= en; i++){

       atr = altr->trpt + i;

       if(ifdir){
          fint1 = *(atr->add_fld + ifdirp);
          fint2 = *((atr + 1)->add_fld + ifdirp);
       }
       else {
          fint1 = atr->zf;
          fint2 = (atr + 1)->zf;
       }

       if(fint1 > ADD_CHECK || fint2 > ADD_CHECK) continue;
 
       grate += fint2 - fint1;
       ++nsamp;
   }

   if(nsamp == 0){
      grate = ADD_UNDEF;
   }
   else {
      grate /= nsamp;
   }

   return grate;
}
