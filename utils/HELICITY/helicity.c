#include <Stdio.h>
#include <stdlib.h>
#include <Math.h>
#include <sys/types.h>
#include "splice.h"
#include "vec.h"
#include "mem_er.h"
#include "m_values.h"
#include "st_obj.h"
#include "st_fo.h"
#include "geo_values.h"

/* program to compute the helicity */

#define  GTOL  1.0e-4

struct tot_tr *read_tracks(FILE *, int *, int *, int *, int , float *, float * , float ** , int * );
FILE *open_file(char * , int * , long int * , int * , int * , int * , int * , int * , int * , int * , int * , int * , int * , int * , int *);
double dirangle(struct tot_tr * , double * , double * , int , int , int , int );
void setup_rotmatrix(double , double , double , double , VEC * );
void setup_radvec(VEC * , VEC * , VEC * , VEC * , VEC * , VEC * , VEC * , VEC * , double , double , int , int , int , int );
void find_last(struct fet_pt_tr * , struct feature_pts * , int * , int * , int , int );
double fvec(VEC * , VEC * , VEC * , VEC * );
void convert_track(struct tot_tr * , int , int , int );
double prop_speed(struct tot_tr * , int );
double vert_integ(double * , int , int );
void sincos(double , double * , double * );

extern float sum_per;
extern int iper_num;
extern int nfld, nff;
extern int *nfwpos;

int main(void )
{
  int i, j, k, m;
  
  int gpr=0, ipr=0;
  int trnum=0;
  
  int irdim=0;
  int ndsmth=0, ifcnt=0, itpadd=0, iprojd=0;
  int irtrn=0, irnr=0, irnth=0, irnf=0;
  int ifdir=0, idir=0, ifdirp=0;
  int igsmp_typ=0;
  int nxy=0;
  int iwwnd=0;
  int nll1=0, nll2=0;

  int ishx=0, ishy=0;
  
  int *ifncnt=NULL, *ifncntp=NULL;
  
  long int iptnum=0;
  
/*  off_t place1=0, place2=0, blklen=0; */ 
  

  float alat=0.0, alng=0.0;
  float rr=0.0;
  
  float *sradU=NULL, *sradV=NULL;
  float *sradUg=NULL, *sradVg=NULL;
  float *slng=NULL, *slat=NULL;
  float *slng2=NULL, *slat2=NULL;
  
  double norm=0.0, arot=0.0; 
  double s1=0.0, s2=0.0, c1=0.0, c2=0.0;
  double xx=0.0, yy=0.0;
  double ss1=0.0, cc1=0.0;
  
  double uu=0.0, vv=0.0;
  double uug=0.0, vvg=0.0;

  double pdirl=0.0, tstp=0.0;
  double ang=0.0;
      
  double **hellev=NULL;
  double *varr=NULL;
  
  float *helicity=NULL;
  
  float *wtan=NULL, *wrad=NULL;
  float *abuf=NULL; 
  
  VEC ptt={0.0, 0.0, 0.0};
  VEC vel={0.0, 0.0, 0.0}, vx={0.0, 0.0, 0.0}, vy={0.0, 0.0, 0.0};

  VEC pt1, vtmp;
  VEC vt1, vt2, vt3, vn;

  VEC *vecg=NULL, *vt=NULL;
  VEC rot[3]={{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
  VEC vsdir={0.0, 0.0, 0.0}, tvec={0.0, 0.0, 0.0};
  VEC *newv=NULL, *etht=NULL, *ephi=NULL, *erad=NULL, *etan=NULL;
  VEC *erdd=NULL, *etnn=NULL;

  FILE *fin=NULL;
  FILE *fradU=NULL, *fradV=NULL;
  FILE *fradUg=NULL, *fradVg=NULL;
  FILE *fhl=NULL, *ftan=NULL, *frad=NULL;
  FILE *fhel=NULL;

  char filin[MAXCHR];
  char filrad[MAXCHR];

  struct tot_tr *tracks=NULL, *atr=NULL;
  struct fet_pt_tr *fp=NULL;
  struct feature_pts fpt;

  printf("What is the track file to read?\n\n");
  scanf("%s", filin);

  fin = fopen(filin, "r");
  if(!fin){
      printf("***ERROR***, unable to open file %s for 'r'\n\n", filin);
      exit(1);
  }

  tracks = read_tracks(fin, &trnum, &gpr, &ipr, 's', &alat, &alng, NULL, NULL);
 
  fclose(fin);

/* read U file */

  printf("What is the filename for the sampled U field?\n\n");
  scanf("%s", filrad);

  fradU = open_file(filrad, &irtrn, &iptnum, &irnth, &irnr, &irnf, &idir, &ndsmth, &ifcnt, &itpadd, &iprojd, &ifdir, &igsmp_typ, &ishx, &ishy);
  
  if(irtrn != trnum){
     printf("****ERROR****, track file and field data not compatable: different numbers of tracks.\n\n");
     exit(1);
  }
  
  if(!idir){
    printf("****ERROR****, no storm rotation of data for U field.\n\n");
    exit(1);
  }
 
  irdim = irnr * irnth;
  
  sradU = (float *)calloc(irdim, sizeof(float));
  mem_er((sradU == NULL) ? 0 : 1, irdim * sizeof(float));
  slng = (float *)calloc(irnth, sizeof(float));
  mem_er((slng == NULL) ? 0 : 1, irnth * sizeof(float));
  slat = (float *)calloc(irnr, sizeof(float));
  mem_er((slat == NULL) ? 0 : 1, irnr * sizeof(float));
  
  fread(slng, irnth*sizeof(float), 1, fradU);
  fscanf(fradU, "%*c");
  fread(slat, irnr*sizeof(float), 1, fradU);
  fscanf(fradU, "%*c");

/* read V file */

  printf("What is the filename for the sampled V field?\n\n");
  scanf("%s", filrad);

  fradV = open_file(filrad, &irtrn, &iptnum, &irnth, &irnr, &irnf, &idir, &ndsmth, &ifcnt, &itpadd, &iprojd, &ifdir, &igsmp_typ, &ishx, &ishy);
  
  if(irtrn != trnum){
     printf("****ERROR****, track file and field data not compatable: different numbers of tracks.\n\n");
     exit(1);
  }
  
  if(!idir){
    printf("****ERROR****, no storm rotation of data for V field.\n\n");
    exit(1);
  } 
 
  sradV = (float *)calloc(irdim, sizeof(float));
  mem_er((sradV == NULL) ? 0 : 1, irdim * sizeof(float))
  slng2 = (float *)calloc(irnth, sizeof(float));
  mem_er((slng2 == NULL) ? 0 : 1, irnth * sizeof(float));
  slat2 = (float *)calloc(irnr, sizeof(float));
  mem_er((slat2 == NULL) ? 0 : 1, irnr * sizeof(float));

  fread(slng2, irnth*sizeof(float), 1, fradV);
  fscanf(fradV, "%*c");
  fread(slat2, irnr*sizeof(float), 1, fradV);
  fscanf(fradV, "%*c");  
  
  for(i=0; i < irnr; i++){
      if(fabs(*(slat + i) - *(slat2 + i)) > GTOL){
          printf("****ERROR****, grids do not match in radial direction\n\n");
          exit(1);
      }
  }

  for(i=0; i < irnth; i++){
      if(fabs(*(slng + i) - *(slng2 + i)) > GTOL){
          printf("****ERROR****, grids do not match in tangential direction\n\n");
          exit(1);
      }
  }
  
/* read Ugrad file */
  
  printf("What is the filename for the sampled Ugrad field?\n\n");
  scanf("%s", filrad);

  fradUg = open_file(filrad, &irtrn, &iptnum, &irnth, &irnr, &irnf, &idir, &ndsmth, &ifcnt, &itpadd, &iprojd, &ifdir, &igsmp_typ, &ishx, &ishy);
  
  if(irtrn != trnum){
     printf("****ERROR****, track file and field data not compatable: different numbers of tracks.\n\n");
     exit(1);
  }
  
  if(!idir){
    printf("****ERROR****, no storm rotation of data for Ug field.\n\n");
    exit(1);
 }
  
  sradUg = (float *)calloc(irdim, sizeof(float));
  mem_er((sradUg == NULL) ? 0 : 1, irdim * sizeof(float))

  fread(slng2, irnth*sizeof(float), 1, fradUg);
  fscanf(fradUg, "%*c");
  fread(slat2, irnr*sizeof(float), 1, fradUg);
  fscanf(fradUg, "%*c");  
  
  for(i=0; i < irnr; i++){
      if(fabs(*(slat + i) - *(slat2 + i)) > GTOL){
          printf("****ERROR****, grids do not match in radial direction\n\n");
          exit(1);
      }
  }

  for(i=0; i < irnth; i++){
      if(fabs(*(slng + i) - *(slng2 + i)) > GTOL){
          printf("****ERROR****, grids do not match in tangential direction\n\n");
          exit(1);
      }
  }  
  
  /* read Vgrad file */
  
  printf("What is the filename for the sampled Vgrad field?\n\n");
  scanf("%s", filrad);

  fradVg = open_file(filrad, &irtrn, &iptnum, &irnth, &irnr, &irnf, &idir, &ndsmth, &ifcnt, &itpadd, &iprojd, &ifdir, &igsmp_typ, &ishx, &ishy);
  
  if(irtrn != trnum){
     printf("****ERROR****, track file and field data not compatable: different numbers of tracks.\n\n");
     exit(1);
  }
  
  if(!idir){
    printf("****ERROR****, no storm rotation of data for Vg field.\n\n");
    exit(1);
 }
  
  sradVg = (float *)calloc(irdim, sizeof(float));
  mem_er((sradVg == NULL) ? 0 : 1, irdim * sizeof(float))

  fread(slng2, irnth*sizeof(float), 1, fradVg);
  fscanf(fradVg, "%*c");
  fread(slat2, irnr*sizeof(float), 1, fradVg);
  fscanf(fradVg, "%*c");  
  
  for(i=0; i < irnr; i++){
      if(fabs(*(slat + i) - *(slat2 + i)) > GTOL){
          printf("****ERROR****, grids do not match in radial direction\n\n");
          exit(1);
      }
  }

  for(i=0; i < irnth; i++){
      if(fabs(*(slng + i) - *(slng2 + i)) > GTOL){
          printf("****ERROR****, grids do not match in tangential direction\n\n");
          exit(1);
      }
  }  
  
  if(!idir || !ndsmth){
     printf("****ERROR****, fields must be sampled with storm rotation to propagation direction.\n\n");
     exit(1);
  }
  
  if(idir == 2){
     printf("****ERROR****, rotation should not be relative to anything other than direction. \n\n");
     exit(1);  
  }
  
  
  if(ifdir){
     if(*(nfwpos + ifdir - 1)){
       ifdirp = 0;
       for(j=0; j < ifdir - 1; j++){
           if(*(nfwpos + j)) ifdirp += 3;
           else ifdirp += 1;
       }
     }
     else {
	printf("****ERROR****, addional field does not have positional information for dirction finding, exiting.\n\n");
        exit(1);
     }  
     
  }
  
  printf("What is the time step in seconds for speeds in m/s?\n\n");
  scanf("%lf", &tstp);
  
  convert_track(tracks, trnum, ifdir, ifdirp);
  
/* setup coordinate vectors */
  
/* note, etht, ephi, erad and etan double as arrays for use with the rotated lat-lng sampling grid.
 etht == yy, ephi == xx (full grid); erad == yy, etan == xx (sampled grid).                          */


   vecg = (VEC *)calloc(irdim, sizeof(VEC));
   mem_er((vecg == NULL) ? 0 : 1, irdim*sizeof(VEC));

   newv = (VEC *)calloc(irdim, sizeof(VEC));
   mem_er((newv == NULL) ? 0 : 1, irdim*sizeof(VEC));

   etht = (VEC *)calloc(irdim, sizeof(VEC));
   mem_er((etht == NULL) ? 0 : 1, irdim*sizeof(VEC));

   ephi = (VEC *)calloc(irdim, sizeof(VEC));
   mem_er((ephi == NULL) ? 0 : 1, irdim*sizeof(VEC));

   erad = (VEC *)calloc(irdim, sizeof(VEC));
   mem_er((erad == NULL) ? 0 : 1, irdim*sizeof(VEC));

   etan = (VEC *)calloc(irdim, sizeof(VEC));
   mem_er((etan == NULL) ? 0 : 1, irdim*sizeof(VEC));
	     	     
   for(i=0; i < irnr; i++){
      rr = FP_PI2 - *(slat + i) * FP_PI;
      if(rr < 0.0) rr = 0.0;
      sincos(rr, &s1, &c1);
      for(j=0; j < irnth; j++){
          sincos(*(slng + j) * FP_PI, &s2, &c2);
          vt = vecg + i * irnth + j;
          vt->x = s1 * c2;
          vt->y = s1 * s2;
          vt->z = c1; 
       }
   }
	     
	     
   if(!igsmp_typ){
	     
      erdd = (VEC *)calloc(irdim, sizeof(VEC));
      mem_er((erdd == NULL) ? 0 : 1, irdim*sizeof(VEC));

      etnn = (VEC *)calloc(irdim, sizeof(VEC));
      mem_er((etnn == NULL) ? 0 : 1, irdim*sizeof(VEC));

      for(i=0; i < irdim; i++){
          etnn[i].x= -vecg[i].y;
          etnn[i].y = vecg[i].x;
          norm = sqrt(dotp(&etnn[i], &etnn[i]));
          normv(&etnn[i], norm);
  
          crosp(&etnn[i], &vecg[i], &erdd[i]);
          norm = sqrt(dotp(&erdd[i], &erdd[i]));
          normv(&erdd[i], norm);   
      }
       
   }
	     
   else {
      if(irnr != irnth){
         printf("****ERROR****, only lat-long grids the same dimension in both directions currently allowed.\n\n");
         exit(1);
      }
      nxy = irnr;

   }
   
/* Initialise arrays for helicity */

   hellev = (double **)calloc(irnf, sizeof(double *));
   mem_er((hellev == NULL) ? 0 : 1, irnf*sizeof(double *));
   for(i=0; i < irnf; i++){
      *(hellev + i) = (double *)calloc(irdim, sizeof(double));
      mem_er((*(hellev + i) == NULL) ? 0 : 1, irdim*sizeof(double));
   }
   
   helicity = (float *)calloc(irdim, sizeof(float));
   mem_er((helicity == NULL) ? 0 : 1, irdim*sizeof(float));  
   
   abuf = (float *)calloc(irdim, sizeof(float));
   mem_er((abuf == NULL) ? 0 : 1, irdim*sizeof(float));
   
   varr = (double *)calloc(irnf, sizeof(double));
   mem_er((varr == NULL) ? 0 : 1, irnf*sizeof(double));
   
/* openfile for writing helicity level data */

   fhl = fopen("helicity_lev.dat", "w");
   fprintf(fhl, "%6d %10ld %6d %6d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n", trnum, iptnum, irnth, irnr, irnf, idir, ndsmth, ifcnt, itpadd, iprojd, ifdir, igsmp_typ, ishx, ishy);

   fwrite(slng, irnth * sizeof(float), 1, fhl);
   fprintf(fhl, "\n");
   fwrite(slat, irnr * sizeof(float), 1, fhl);
   fprintf(fhl, "\n"); 
   
/* openfile for writing helicity data */  

   fhel = fopen("helicity.dat", "w");
   fprintf(fhel, "%6d %10ld %6d %6d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n", trnum, iptnum, irnth, irnr, 1, idir, ndsmth, ifcnt, itpadd, iprojd, ifdir, igsmp_typ, ishx, ishy);

   fwrite(slng, irnth * sizeof(float), 1, fhel);
   fprintf(fhel, "\n");
   fwrite(slat, irnr * sizeof(float), 1, fhel);
   fprintf(fhel, "\n");
   
/* Initialise arrays for wind components if needed  */

   printf("Do you want to  save the wind components, 'y' or 'n'\n\n");
   scanf("\n");
   if(getchar() == 'y'){

      iwwnd = 1;

      wtan = (float *)calloc(irdim, sizeof(float));
      mem_er((wtan == NULL) ? 0 : 1, irdim*sizeof(float));
      wrad = (float *)calloc(irdim, sizeof(float));
      mem_er((wrad == NULL) ? 0 : 1, irdim*sizeof(float));
      
      ftan = fopen("wind_tan.dat", "w");
      fprintf(ftan, "%6d %10ld %6d %6d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n", trnum, iptnum, irnth, irnr, irnf, idir, ndsmth, ifcnt, itpadd, iprojd, ifdir, igsmp_typ, ishx, ishy);

      fwrite(slng, irnth * sizeof(float), 1, ftan);
      fprintf(ftan, "\n");
      fwrite(slat, irnr * sizeof(float), 1, ftan);
      fprintf(ftan, "\n");
      
      frad = fopen("wind_rad.dat", "w");
      fprintf(frad, "%6d %10ld %6d %6d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n", trnum, iptnum, irnth, irnr, irnf, idir, ndsmth, ifcnt, itpadd, iprojd, ifdir, igsmp_typ, ishx, ishy);

      fwrite(slng, irnth * sizeof(float), 1, frad);
      fprintf(frad, "\n");
      fwrite(slat, irnr * sizeof(float), 1, frad);
      fprintf(frad, "\n");      
   
   }
   
   printf("There are %d levels, specify the levels between which you want to restrict the vertical integration, l1, l2?\r\n"
          "i.e. first NL levels will be used.                                                                           \n\n", irnf);
   scanf("%d %d", &nll1, &nll2);
   if((nll1 < 1 || nll1 > irnf) || (nll2 < 1 || nll2 > irnf)){
       printf("****ERROR****, incorrect levels identifiers specified, exiting.\n\n");
       exit(1);
   }
   
/* calculate data block size */

/*   place1 = ftello(fradU);
   fread(sradU, irdim*sizeof(float), 1, fradU);
   fscanf(fradU, "%*c");
   place2 = ftello(fradU);
   blklen = place2 - place1; 
   fseeko(fradU, place1, SEEK_SET); */
   
/* main processing, looping over tracks */
  
   for(i=0; i < trnum; i++){
       atr = tracks + i;
       
       for(j=0; j < atr->num; j++){

           fp = atr->trpt + j;
	   
	   xx = fp->xf * FP_PI;
           yy = FP_PI2 - fp->yf * FP_PI;
           if(yy < 0.) yy = 0.0;
           sincos(xx, &s1, &c1);
           sincos(yy, &s2, &c2);

           pt1.x = fp->pp[0];
           pt1.y = fp->pp[1];
           pt1.z = fp->pp[2];	   
	    

           vsdir.x = -c1 * c2;
           vsdir.y = -s1 * c2;
           vsdir.z = s2;

/* compute system velocity for relative velocities */

           pdirl = prop_speed(atr, j) * EARTH_RADIUS_M / tstp;

/* calculate directional vector */

	   arot = dirangle(atr, &cc1, &ss1, j, ndsmth, ifdir, ifdirp);

/*           printf("%e %e\n", arot/FP_PI, pdirl); */

           norm = dotp(&vsdir, &pt1);
           crosp(&vsdir, &pt1, &vt1);
           mulv(&vt1, &vt1, ss1);
           mulv(&pt1, &vt2, ((1.0 - cc1) * norm));
           mulv(&vsdir, &vt3, cc1);
           addv(&vt1, &vt2, &vn);
           addv(&vn, &vt3, &tvec);
		
	   if(igsmp_typ){
              yy = -(FP_PI2 - yy);
              sincos(yy, &s2, &c2);		    
	   }
		
           setup_rotmatrix(s1, c1, s2, c2, rot);		
               
           if(!ifcnt)
              setup_radvec(rot, vecg, &pt1, newv, etht, ephi, erad, etan, ss1, cc1, irdim, idir, igsmp_typ, nxy);
	      
           for(m=0; m < irnf; m++){

               if(ifcnt){
                  find_last(fp, &fpt, ifncnt, ifncntp, m, itpadd);

                  xx = (fpt.x).xy * FP_PI;
                  yy = FP_PI2 - (fpt.y).xy * FP_PI;

/* determine sampling point at this level */

                  if(iprojd){

                     sincos(xx, &s1, &c1);
                     sincos(yy, &s2, &c2);

                     ptt.x = c1 * s2;
                     ptt.y = s1 * s2;
                     ptt.z = c2;

                     fvec(&pt1, &tvec, &ptt, &vn);

                     yy = acos(vn.z);
                     xx = atan2(vn.y, vn.x);
                     if(xx < 0.0) xx = FPI2 + xx;

                  }

                  sincos(xx, &s1, &c1);
                  sincos(yy, &s2, &c2);

                  pt1.x = c1 * s2;
                  pt1.y = s1 * s2;
                  pt1.z = c2;
		      
	          if(igsmp_typ){
	   	     yy = -(FP_PI2 - yy);
                     sincos(yy, &s2, &c2);		    
	          }
		      
	          setup_rotmatrix(s1, c1, s2, c2, rot);

                  setup_radvec(rot, vecg, &pt1, newv, etht, ephi, erad, etan, ss1, cc1, irdim, idir, igsmp_typ, nxy);

               }	   
	       
               fread(sradU, irdim*sizeof(float), 1, fradU);
               fscanf(fradU, "%*c");	
	   
               fread(sradV, irdim*sizeof(float), 1, fradV);
               fscanf(fradV, "%*c");   
	   
               fread(sradUg, irdim*sizeof(float), 1, fradUg);
               fscanf(fradUg, "%*c");	
	   
               fread(sradVg, irdim*sizeof(float), 1, fradVg);
               fscanf(fradVg, "%*c");	       
	       
               for(k=0; k < irdim; k++){
                   uu = *(sradU + k);
                   vv = *(sradV + k);
		   
		   uug = *(sradUg + k);
		   vvg = *(sradVg + k);		   

                   if(uu > ADD_CHECK || vv > ADD_CHECK) continue;

                   mulv(&ephi[k], &vx, uu);
		       
		   if(!igsmp_typ) {
		      mulv(&etht[k], &vy, (-vv)); 
		   }
		   else {
		      mulv(&etht[k], &vy, vv);
		   }
			  
                   addv(&vx, &vy, &vel);

                   uu = dotp(&etan[k], &vel);
                   vv = dotp(&erad[k], &vel);
		   
                   if(uug > ADD_CHECK || vvg > ADD_CHECK) continue;

                   mulv(&ephi[k], &vx, uug);
		       
		   if(!igsmp_typ) {
		      mulv(&etht[k], &vy, (-vvg)); 
		   }
		   else {
		      mulv(&etht[k], &vy, vvg);
		   }
			  
                   addv(&vx, &vy, &vel);

                   uug = dotp(&etan[k], &vel);
                   vvg = dotp(&erad[k], &vel);	

                   if(!igsmp_typ){
                      ang = sqrt(1.0 - vecg[k].x * vecg[k].x);
                      ang = acos(ang);
                      if(vecg[k].x < 0.0) ang = -ang;
                      sincos(ang, &s1, &c1);
                      vtmp.x = -c1;
                      vtmp.y = 0.0;
                      vtmp.z = s1;
                      mulv(&vtmp, &vtmp, pdirl);
                      uu -= dotp(&vtmp, &etnn[k]);
                      vv -= dotp(&vtmp, &erdd[k]);
                   }
                   else {
                      vv -= pdirl;
                   } 
		                    
		   
		   *(*(hellev + m) + k) = -uu * vvg + vv * uug;
		   *(abuf + k) = *(*(hellev + m) + k);
		   
		   if(iwwnd){
		       *(wtan + k) = uu;
                       *(wrad + k) = vv;
		   }	   
		   
		}

/* write data to file */

	        fwrite(abuf, irdim * sizeof(float), 1, fhl);
                fprintf(fhl, "\n");
		
		if(iwwnd){
	           fwrite(wtan, irdim * sizeof(float), 1, ftan);
                   fprintf(ftan, "\n");
	           fwrite(wrad, irdim * sizeof(float), 1, frad);
                   fprintf(frad, "\n");		   		
		}	       
	   
           }
	   
/* vertically integrate */

           for(k=0; k < irdim; k++){
	       for(m=0; m < irnf; m++){
	          *(varr + m) = *(*(hellev + m) + k);
	       }
	       *(helicity + k) = vert_integ(varr, nll1, nll2);
	   }
	   
	   fwrite(helicity, irdim * sizeof(float), 1, fhel);
           fprintf(fhel, "\n");
	   	   
       }  
   
   }
   
   fclose(fhl);
   fclose(fhel);
  
   if(iwwnd){

      free(wtan);
      free(wrad);
      
      fclose(ftan);   
      fclose(frad);  
      
   }
   
   free(slat); free(slng);
   free(slat2); free(slng2);
   free(sradU); free(sradV);
   free(sradUg); free(sradVg);  
   for(i=0; i < irnf; i++) free(*(hellev + i));
   free(hellev);
   free(helicity);
   free(abuf);
   free(varr);
   

   return 0;

}

/* open data file for read */

FILE *open_file(char *filrad, int *irtrn, long int *iptnum, int *irnth, int *irnr, int *irnf, int *idir, int *ndsmth, int *ifcnt, int *itpadd, int *iprojd, int *ifdir, int *igsmp_typ, int *ishx, int *ishy)
{

    int irtrn2=0;
    int irnth2=0, irnr2=0, irnf2=0;
    int idir2=0, ndsmth2=0, ifdir2=0;
    int ifcnt2=0, itpadd2=0, iprojd2=0;
    int igsmp_typ2=0;
    int ishx2=0, ishy2=0;

    long int iptnum2=0;

    static int ifrst=0;

    FILE *frad=NULL;
    char line[MAXCHR];

    frad = fopen(filrad, "r");
    if(!frad){
       printf("***ERROR***, unable to open file %s for 'r'\n\n", filrad);
       exit(1);
    }
    fgets(line, MAXCHR, frad);

    if(!ifrst){
       sscanf(line, "%d %ld %d %d %d %d %d %d %d %d %d %d %d %d", irtrn, iptnum, irnth, irnr, irnf, idir, ndsmth, ifcnt, itpadd, iprojd, ifdir, igsmp_typ, ishx, ishy);
       ifrst = 1;
    }
    else{
       sscanf(line, "%d %ld %d %d %d %d %d %d %d %d %d %d %d %d", &irtrn2, &iptnum2, &irnth2, &irnr2, &irnf2, &idir2, &ndsmth2, &ifcnt2, &itpadd2, &iprojd2, &ifdir2, &igsmp_typ2, &ishx2, &ishy2);

       if(*irtrn != irtrn2 || *iptnum != iptnum2 || *irnth != irnth2 ||
          *irnr != irnr2   || *irnf != irnf2 || *idir != idir2  || *ndsmth != ndsmth2 ||
          *ifcnt != ifcnt2 || *itpadd != itpadd2 || *iprojd != iprojd2 || *ifdir != ifdir2 ||
	  *igsmp_typ != igsmp_typ2 || *ishx != ishx2 || *ishy != ishy2){
           printf("****ERROR****, incompatability between radial field files.\n\n");
           exit(1);
       }
    }

    return frad;
}

/* determine rotation angle for alinging grid with system direction */

double dirangle(struct tot_tr *altr, double *cn, double *sn, int pt_id, int ndsmth, int ifdir, int ifdirp)
{
   int i=0;
   int n2, st=0, en=0;
   int narot=0;

   double aa=0.0, arot=0.0, norm=0.0;
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

       aa = acos(dotp(&vsdir, &tvec));

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

       if(fabs(dotp(&vn, &tvec) - 1.0) > 1.0e-6){
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

/* function sets up the rotation matrix */

void setup_rotmatrix(double s1, double c1, double s2, double c2, VEC *rot)
{

    rot[0].x = c1 * c2;
    rot[0].y = -s1;
    rot[0].z = c1 * s2;
    rot[1].x = s1 * c2;
    rot[1].y = c1;
    rot[1].z = s1 * s2;
    rot[2].x = -s2;
    rot[2].z = c2;

    return;
}

/* setup local radial vectors */

void setup_radvec(VEC *rot, VEC *vecg, VEC *pt1, VEC *newv, VEC *etht, VEC *ephi, VEC *erad, VEC *etan, double ss1, double cc1, int irdim, int idir, int igsmp_typ, int nll)
{

    int i=0, j=0;
    int i0=0, i1=0, i2=0;

    VEC vt1, vt2, vt3, vn;

    double norm=0.0;

    if(!igsmp_typ){

       for(i=0; i < irdim; i++){

           newv[i].x = dotp(&rot[0], &vecg[i]);
           newv[i].y = dotp(&rot[1], &vecg[i]);
           newv[i].z = dotp(&rot[2], &vecg[i]);

           if(idir){
              norm = dotp(&newv[i], pt1);
              crosp(&newv[i], pt1, &vt1);
              mulv(&vt1, &vt1, ss1);
              mulv(pt1, &vt2, ((1.0 - cc1) * norm));
              mulv(&newv[i], &vt3, cc1);
              addv(&vt1, &vt2, &vn);
              addv(&vn, &vt3, &newv[i]);
           }

           ephi[i].x = -(newv[i].y);
           ephi[i].y = newv[i].x;
           ephi[i].z = 0.0;
           norm = sqrt(dotp(&ephi[i], &ephi[i]));
           normv(&ephi[i], norm);
                   
           crosp(&ephi[i], &newv[i], &etht[i]);
           norm = sqrt(dotp(&etht[i], &etht[i]));
           normv(&etht[i], norm);

           crosp(pt1, &newv[i], &etan[i]);
           norm = sqrt(dotp(&etan[i], &etan[i]));
           normv(&etan[i], norm);

           crosp(&etan[i], &newv[i], &erad[i]);
           norm = sqrt(dotp(&erad[i], &erad[i]));
           normv(&erad[i], norm);   

       }
    }
    
    else {
    
       for(i=0; i < nll; i++){
       
           for(j=0; j < nll; j++){
	       i0 = i * nll + j;
               newv[i0].x = dotp(&rot[0], &vecg[i0]);
               newv[i0].y = dotp(&rot[1], &vecg[i0]);
               newv[i0].z = dotp(&rot[2], &vecg[i0]);

               if(idir){
                  norm = dotp(&newv[i0], pt1);
                  crosp(&newv[i0], pt1, &vt1);
                  mulv(&vt1, &vt1, ss1);
                  mulv(pt1, &vt2, ((1.0 - cc1) * norm));
                  mulv(&newv[i0], &vt3, cc1);
                  addv(&vt1, &vt2, &vn);
                  addv(&vn, &vt3, &newv[i0]);
               }

               ephi[i0].x = -(newv[i0].y);
               ephi[i0].y = newv[i0].x;
               ephi[i0].z = 0.0;
               norm = sqrt(dotp(&ephi[i0], &ephi[i0]));
               normv(&ephi[i0], norm);
                   
               crosp(&newv[i0], &ephi[i0], &etht[i0]);
               norm = sqrt(dotp(&etht[i0], &etht[i0]));
               normv(&etht[i0], norm);		       
	       	   
	    }     
  
       }
       
       for(j=0; j < nll; j++){
           i1 = j;
	   i2 = (nll - 1) * nll + j;
	   crosp(&newv[i2], &newv[i1], &etan[j]);
           norm = sqrt(dotp(&etan[j], &etan[j]));
           normv(&etan[j], norm);

           for(i=0; i < nll; i++){
	       i0 = i * nll + j;
	       etan[i0].x = etan[j].x;
	       etan[i0].y = etan[j].y; 
	       etan[i0].z = etan[j].z;
	       
	       crosp(&newv[i0], &etan[i0], &erad[i0]);
               norm = sqrt(dotp(&erad[0], &erad[0]));
               normv(&erad[0], norm);
	   }
       }
    
    }

    return;
} 

/* if a missing point is found find equivelent point at previous levels */

void find_last(struct fet_pt_tr *atr, struct feature_pts *fpt, int *ifncnt, int *ifncntp, int ic, int itpadd)
{

   int i, j, ii;
   int nmnf=0;
   int nxf=0;

   if(*(ifncnt + ic)){
     (fpt->x).xy = *(atr->add_fld + *(ifncntp + ic));
     (fpt->y).xy = *(atr->add_fld + *(ifncntp + ic) + 1);

     if((fpt->x).xy > ADD_CHECK){

       if(itpadd && ic > 0){
          nmnf = *(ifncntp + ic);
          ii = *(ifncnt + ic) - 2;

          nxf = 0;
          for(i=ii; i >= 0; i--){
              if(*(nfwpos + i)) {
                 nmnf -= 3;
                 nxf = 0;
                 for(j=ic-1; j>=0; j--) {
                    if(i+1 == *(ifncnt + j)) nxf = 1;
                 }

                 if(*(atr->add_fld + nmnf) > ADD_CHECK || !nxf) continue;

                 break;
              }
              else --nmnf;            
          }

          if(nxf){
            (fpt->x).xy = *(atr->add_fld + nmnf);
            (fpt->y).xy = *(atr->add_fld + nmnf + 1);
          }
          else {
            (fpt->x).xy = atr->xf;
            (fpt->y).xy = atr->yf;
          }

        }
        else {
          (fpt->x).xy = atr->xf;
          (fpt->y).xy = atr->yf;
        }
     }
   }
   else {
     (fpt->x).xy = atr->xf;
     (fpt->y).xy = atr->yf;
   }

   return;
}

double fvec(VEC *a, VEC *b, VEC *c, VEC *r)
{

    VEC an, bn;

    double aa, bb, dotab;
    double norm=0.0;

    dotab = dotp(a, b);
    aa = dotp(a, c) - dotp(b, c) * dotab;
    bb = dotp(b, c) - dotp(a, c) * dotab;

    mulv(a, &an, aa);
    mulv(b, &bn, bb);

    addv(&an, &bn, r);

    norm = sqrt(dotp(r, r));
    normv(r, norm);

    return dotab;

}

/* determine system propogation speed over three time steps */

double prop_speed(struct tot_tr *altr, int ifld)
{

    double spp=0.0;

    VEC ptt, pt0, pt1;

    struct fet_pt_tr *fpt=NULL, *fp0=NULL, *fp1=NULL;

    fpt = altr->trpt + ifld;
    ptt.x = fpt->pp[0];
    ptt.y = fpt->pp[1];
    ptt.z = fpt->pp[2];

    if(ifld+1 < altr->num && ifld - 1 >= 0){
      fp0 = fpt - 1;
      fp1 = fpt + 1;
      pt0.x = fp0->pp[0];
      pt0.y = fp0->pp[1];
      pt0.z = fp0->pp[2];
      pt1.x = fp1->pp[0];
      pt1.y = fp1->pp[1];
      pt1.z = fp1->pp[2];
      spp += acos(dotp(&ptt, &pt0));
      spp += acos(dotp(&ptt, &pt1));
      spp *= 0.5;
    }
    else if(ifld+1 < altr->num){
      fp1 = fpt + 1;
      pt1.x = fp1->pp[0];
      pt1.y = fp1->pp[1];
      pt1.z = fp1->pp[2];
      spp = acos(dotp(&ptt, &pt1));
    }
    else if(ifld - 1 >= 0){
      fp0 = fpt - 1;
      pt0.x = fp0->pp[0];
      pt0.y = fp0->pp[1];
      pt0.z = fp0->pp[2];
      spp = acos(dotp(&ptt, &pt0));
    }
    else {
      printf("****ERROR****, track point number inconsistency, exiting.\n\n");
      exit(1);
    }

    return spp;
}

/* calculate vertical integral */

double vert_integ(double *dat, int nl1, int nl2)
{
    int i=0;

    double vint=0.0;

    for(i=nl1-1; i < nl2-1; i++){
        if(*(dat + i) > ADD_CHECK || *(dat + i + 1) > ADD_CHECK) continue;
        vint += 0.5 * (*(dat + i) + *(dat + i + 1)); 
    }

    return vint;
}
