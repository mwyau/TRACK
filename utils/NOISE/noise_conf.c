#include <Stdio.h>
#include <stdlib.h>
#include <Math.h>
#include "mem_er.h"

#define  MAXPT  100000
#define  MAXCHR 100

void rednoise(double , double , double * , double * , double , int );
void corcoff1(float * , int , float * , float , int , int );

int main(int argc, char *argv[])
{
   int i=0;

   int npt=0;
   int nfrq=0;
   int ntype='w';

   float siglev=0.0;
   float corcof=0.0;
   float timser[MAXPT];
   float *freq=NULL;
   
   double *redn=NULL, *redc=NULL;
   double mean=0.0, var=0.0, var2=0.0;
   double diff=0.0;
   
   char buff[MAXCHR];
   
   FILE *fin=NULL;
   
   fin = fopen(argv[1], "r");
   if(!fin){
      printf("No such file exists\n");
      exit(1);
   }
   
   while(fgets(buff, MAXCHR, fin) != NULL) {
     sscanf(buff, "%e", timser + npt);
/*     printf("%f\n", timser[npt]); */
     ++npt;
   }
   
   fclose(fin);
   
   printf("How many frequencies is required?\n\n");
   scanf("%d", &nfrq);
   
   redn = (double *)calloc(nfrq, sizeof(double));
   mem_er((redn == NULL) ? 0 : 1, nfrq * sizeof(double));

   redc = (double *)calloc(nfrq, sizeof(double));
   mem_er((redc == NULL) ? 0 : 1, nfrq * sizeof(double));
   
   freq = (float *)calloc(nfrq, sizeof(float));
   mem_er((freq == NULL) ? 0 : 1, nfrq * sizeof(float));
   
   for(i=0; i < nfrq; i++){
       freq[i] = (float) i / nfrq;
   }
   
   printf("Do you want rednoise or whitenoise, 'r' or 'w'\n\n");
   scanf("\n");
   ntype=getchar();

   if(ntype != 'r' && ntype != 'w'){

      printf("****ERROR****, noise type unknown, exiting\n\n");
      exit(1);
   }  
   
   printf("What significance level do you require, i.e 0.05 is \r\n"
          "the 95%% convidence level, significant at 5%%.        \n\n");

   scanf("%f", &siglev);

   if(siglev < 0 || siglev > 0.1){

      printf("****WARNING****, the significance level is outside of the prescibed\r\n"
                 "                 range of (0, 0.1), using default value of 0.05    \n\n");
      siglev = 0.05;


   }
   
/* compute mean and variance */

   for(i=0; i < npt; i++) mean += timser[i];
   mean /= npt;
   
   for(i=0; i < npt; i++) {
       diff = timser[i] - mean;
       var += diff * diff;
   } 
   var /= (npt - 1);
   var2 = var;
   var = sqrt(var);
   
   printf("%f %f %f\n", mean, var, var2);
   
/* center and normalise time series */

   for(i=0; i < npt; i++){
       timser[i] = (timser[i] - mean) / var;
   }
   
   
   if(ntype == 'r') {

      corcoff1(timser, npt, &corcof, 0.0, 0, 0);
      
   }

   rednoise(1.0, corcof, redn, redc, (double)siglev, nfrq);


   for(i=0; i < nfrq; i++){
       printf("%f %f %f\n", *(freq + i), *(redn + i), *(redc + i));
   }
            
   
   free(redn);
   free(redc);

   return 0;
}
