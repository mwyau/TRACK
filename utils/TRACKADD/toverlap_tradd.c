#include <stdio.h>
#include <stdlib.h>
#include "splice.h"

/* Compute track overlap in time */

int toverlap_tradd(struct tot_tr *tr1, struct tot_tr *tr2, long int is1, long int is2, int *it1, int *it2)
{

    int i; 

    int it1s=0, it1e=0, it2s=0, it2e=0;
    int num=0, numt=0;

    struct fet_pt_tr *fp1=NULL, *fp2=NULL;

/* find position along track for start and end times */

    if(tr1->time && tr2->time){

       for(i=0; i < tr1->num; i++){

           fp1 = tr1->trpt + i;

           if(fp1->time == is1) it1s = i;
           if(fp1->time == is2) it1e = i;

       }



       for(i=0; i < tr2->num; i++){

           fp2 = tr2->trpt + i;

           if(fp2->time == is1) it2s = i;
           if(fp2->time == is2) it2e = i;

       }


    }

    else {

       for(i=0; i < tr1->num; i++){

           fp1 = tr1->trpt + i;

           if(fp1->fr_id == is1) it1s = i;
           if(fp1->fr_id == is2) it1e = i;

       }



       for(i=0; i < tr2->num; i++){

           fp2 = tr2->trpt + i;

           if(fp2->fr_id == is1) it2s = i;
           if(fp2->fr_id == is2) it2e = i;

       }

    }

    numt = it1e - it1s + 1;
    num = it2e - it2s + 1;
    *it1 = it1s;
    *it2 = it2s;

    if(num != numt){
       printf("***ERROR***, track section lengths dont match for TRACK_ID's %d %d\n\n", tr1->trid, tr2->trid);
       exit(1);
    }

    return num;

}
