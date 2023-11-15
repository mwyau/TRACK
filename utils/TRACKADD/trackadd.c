#include <stdio.h>
#include <stdlib.h>
#include <splice.h>
#include <string.h>
#include "mem_er.h"

/* program to combine common tracks from two different files */

struct tot_tr *read_tracks(FILE *, int *, int *, int *, int , float *, float * , float ** , int * );
void meantrd(FILE * , struct tot_tr * , int , int , int , int , float , float );
int toverlap_tradd(struct tot_tr * , struct tot_tr * , long int , long int , int * , int * );
int toverlap(struct tot_tr * , struct tot_tr * , long int * , long int * );

int noheader=0;

extern int nfld, nff;
extern int *nfwpos;

void main()
{
   int i=0, j=0;
   int trnum1=0, trnum2=0;
   int gpr1=0, gpr2=0;
   int ipr1=0, ipr2=0;
   int nff1=0, nff2=0;
   int nfld1=0, nfld2=0; 
   int *nfwpos1=NULL, *nfwpos2=NULL;
   int iadd=0, iaddf=0;
   long int is1=0, is2=0;
   int it1=0, it2=0;
   int num=0;

   float alat1=0.0, alat2=0.0, alng1=0.0, alng2=0.0; 

   FILE *fin1=NULL, *fin2=NULL;
   FILE *fout=NULL;

   char ftr1[MAXCHR], ftr2[MAXCHR];
   char filout[MAXCHR];

   struct tot_tr *tracks1=NULL, *tracks2=NULL, *atr1=NULL, *atr2=NULL;
   struct fet_pt_tr *fp1=NULL, *fp2=NULL;

   printf("What is the first track file, second file will be added to the first?\n\n");
   scanf("%s", ftr1);
   printf("****WARNING****, tracks in both files must be in the same order.\n\n");

   fin1 = fopen(ftr1, "r");
   if(!fin1){
      printf("***ERROR***, unable to open file %s for 'r'\n\n", ftr1);
      exit(1);
   }

   tracks1 = read_tracks(fin1, &trnum1, &gpr1, &ipr1, 's', &alat1, &alng1, NULL, NULL);

   fclose(fin1);

   strncpy(filout, ftr1, MAXCHR);
   strcat(filout, ".tradd");

   nff1 = nff + 1;
   nfld1 = nfld + 1;
   nfwpos1 = (int *)calloc(nff1, sizeof(int));
   mem_er((nfwpos1 == NULL) ? 0 : 1, nff1 * sizeof(int));
   memcpy(nfwpos1, nfwpos, nff*sizeof(int));
   *(nfwpos1 + nff1 - 1) = 0;

   printf("What is the second track file?\n\n");
   scanf("%s", ftr2);

   fin2 = fopen(ftr2, "r");
   if(!fin2){
      printf("***ERROR***, unable to open file %s for 'r'\n\n", ftr2);
      exit(1);
   }

   tracks2 = read_tracks(fin2, &trnum2, &gpr2, &ipr2, 's', &alat2, &alng2, NULL, NULL);

   fclose(fin2);

   nff2 = nff;
   nfld2 = nfld;
   nfwpos2 = (int *)calloc(nff2, sizeof(int));
   mem_er((nfwpos2 == NULL) ? 0 : 1, nff2 * sizeof(int));
   memcpy(nfwpos2, nfwpos, nff2*sizeof(int));

/* check track files are consisent */

   if(trnum1 != trnum2 || gpr1 != gpr2 || ipr1 != ipr2 || alat1 != alat2   || alng1 != alng2){
      printf("****ERROR****, incompatability between track files.\n\n");
      exit(1);
  }

  printf("What additinal field from track set 2 should be added to track set 1, input 0 for default?\n\n");
  scanf("%d", &iadd);
  if(iadd < 0 || iadd > nff2){
    printf("****ERROR****,incorrect value for additional field, %d\n\n", iadd);
  }

  if(iadd){
     iaddf = 0;
     for(i=0; i < iadd; i++){
       if(*(nfwpos2 + i)) iaddf += 3;
       else iaddf += 1;
    }
    --iaddf;
  }

  for(i=0; i < trnum1; i++){
     atr1 = tracks1 + i;

     for(j=0; j < atr1->num; j++){
       fp1 = atr1->trpt + j;
       fp1->add_fld = (float *)realloc_n(fp1->add_fld, nfld1*sizeof(float)); 
       fp1->add_fld[nfld1 - 1] = ADD_UNDEF;
     }

     atr2 = tracks2 + i; 

     toverlap(atr1, atr2, &is1, &is2);

     num = toverlap_tradd(atr1, atr2, is1, is2, &it1, &it2);

     for(j=0; j < num; j++){
        fp1 = atr1->trpt + it1 + j;
        fp2 = atr2->trpt + it2 + j;
        if(iadd){
           fp1->add_fld[nfld1 - 1] = fp2->add_fld[iaddf];
        } 
        else {
           fp1->add_fld[nfld1 - 1] = fp2->zf;  
        }
     }

  }

  nff = nff1;
  nfld = nfld1;
  nfwpos = nfwpos1;

  fout=fopen(filout, "w");
  if(!fout){
     printf("****ERROR****, can't open file %s\n", filout);
     exit(1);
  }

  meantrd(fout, tracks1, trnum1, 's', gpr1, ipr1, alat1, alng1);  

  fclose(fout);

  free(nfwpos1);
  free(nfwpos2);

  return;
}
