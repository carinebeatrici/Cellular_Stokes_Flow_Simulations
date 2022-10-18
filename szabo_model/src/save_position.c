#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "param.h"
#include "particle.h"

void  save_position(FILE *arquivo, particle *boid, int time_step, int n)
{
   int i;
   int count;
   count = 0;
   for(i=0; i<n; i++)
     {	
	if(boid[i].exist==1)
	  {     
	     count++;
	  }
     }
   fprintf(arquivo,"%d %d \n",time_step,count);
   for(i=0;i<n;i++)
     {
	if(boid[i].exist==1)
	  {
	     fprintf(arquivo,"%f %f %f %f %d %f %d\n",boid[i].x,boid[i].y,boid[i].vx,boid[i].vy,boid[i].box,boid[i].dreq, boid[i].id);
	  }
     }
   fflush(stdout);
}


float PolarOrderParameter( particle *boid,  int time, int n)
{
   float sum,sum_x,sum_y;
   FILE *file;
   float vr;
   int count;
   int count_last;
   file=fopen("data//phi.dat","a");
   if( !file )
     {	
	printf("unable to create the file in SaveParticlesBin\n");
//	return 1;
	sum = 0.0;
     }
   
   if( V1 == 0.0)
     sum = 0.0;
//     return 0.0;

   int i;
   sum_x = 0.0;
   sum_y = 0.0;
   count = 0;
   count_last = 0;
   for(i=0; i<n; i++)
     {
	if(boid[i].exist==1)
	  {
	     vr = sqrt(boid[i].vx*boid[i].vx+boid[i].vy*boid[i].vy);
	     if(vr > 0.0)
	       {
		  count++;
		  sum_x += boid[i].vx/vr;
		  sum_y += boid[i].vy/vr;
	       }
	     
	     //      printf("%f %f",senangle[GP[i].angle],cosangle[GP[i].angle]);
	     if(boid[i].x > (Lx-5)-(3*R_MAX) && boid[i].x < (Lx-5)-(2*R_MAX))count_last++;
	  }
	
     }
   sum=sqrt(sum_x*sum_x+sum_y*sum_y);
   sum/=(float)count;
   fprintf(file,"%d  %f  %f  %2.2f %d %d\n",time,sum,atan2(sum_y,sum_x),(float)(count/(Lx*Ly)),count, count_last);
   fclose(file);
   return sum;
}
	// 


float SolidLiquid( particle *boid, int time, int n, int *vizinhos_old, int *vizinhos, 
		   int *num_vizinhos, int *num_vizinhos_old, int *num_different)
{
   float Delta;
   int i, j, k;
   static int is_first = 1;
   int count_viz = 0;
   float distance = 0.0;
   int is_same = 0;
   FILE *file;
   file=fopen("data//delta.dat","a");
   
   if(is_first == 0)
     {
	for(i = 0; i<n; i++)
	  {     
	     count_viz = 0;
	     for(j = 0; j<n; j++)
	       {
		  distance = (boid[i].x - boid[j].x) * (boid[i].x - boid[j].x) 
		    + (boid[i].y - boid[j].y) * (boid[i].y - boid[j].y);
		  if(distance<(R_MAX*R_MAX))
		    {   
		       vizinhos[i*MAX_VIZ+count_viz] = j;
		       count_viz++;
		    }
	       }
	     num_vizinhos[i] = count_viz;
	  }
	Delta = 0.0;
	for(i = 0; i<n; i++)
	  {
	     num_different[i] = 0;
	     for(j = 0; j<num_vizinhos[i]; j++)
	       {
		  is_same = 0;
		  for(k = 0; k<num_vizinhos_old[i]; k++)
		    {
		       if(vizinhos[i*MAX_VIZ+j] == vizinhos_old[i*MAX_VIZ+k])
			 {
			    is_same = 1;
			 }            
		    }
		  if(is_same == 0)
		    {
		       num_different[i]++;
		    }
	       }
	     if(num_vizinhos[i] < num_vizinhos_old[i] )
	       {
		  num_different[i] += (num_vizinhos[i] - num_vizinhos_old[i]);
	       }
	     if(num_vizinhos_old[i] != 0)
	       {
		  Delta += (float)num_different[i]/(float)(num_vizinhos_old[i]);
	       }
	     else
	       {
		  Delta++;
	       }
	  }
	Delta /= n;
	for(i = 0; i<n; i++)
	  {
	     for(k = 0; k<MAX_VIZ; k++)
	       {
		  vizinhos_old[i*MAX_VIZ+k] = 0;
	       }
	     for(k = 0; k<num_vizinhos[i]; k++)
	       {
		  vizinhos_old[i*MAX_VIZ+k] = vizinhos[i*MAX_VIZ+k];
		  num_vizinhos_old[i] = num_vizinhos[i];
	       }
	  }

     }
   else
     {
	is_first = 0;
	for(i = 0; i<n; i++)
	  {
	    count_viz = 0;
	    for(j = 0; j<n; j++)
	       {
		  distance = (boid[i].x - boid[j].x) * (boid[i].x - boid[j].x) 
		           + (boid[i].y - boid[j].y) * (boid[i].y - boid[j].y);
		  if(distance<(R_MAX*R_MAX))
		    {
		       vizinhos_old[i*MAX_VIZ+count_viz] = j;
		       count_viz++;
		    }        
	       }
	     num_vizinhos_old[i] = count_viz;
	  }
	Delta = -1.0;
     }
   
   fprintf(file,"%d  %f \n",time, Delta);
   fclose(file);
//   printf("Delta: %f \n", Delta);
   return Delta;
}




void SaveImageDensity(particle *boid, const int box_for_line, const int box_for_row, 
		     int *imagem, float *vx_mean, float *vy_mean, float *phi_local, float *angle_local, int n)
{
   int i,caixas_x,caixas_y;
//   int *imagem;
//   int retorno = 0;
//   int total_box = box_for_line * box_for_row;
//   float *vx_mean, *vy_mean;
//   float *phi_local, *angle_local;
   FILE *file,*file2;
//   printf(" aqui 3.7.6 \n");
   file=fopen("data//image.dat","a");
   file2=fopen("data//localphi.dat","a");
   if( !file )
     {
	
	printf("unable to create the file in SaveParticlesBin\n");
	exit(0);
     }
  // printf(" aqui 3.7.7 \n");
  /* 
   retorno += posix_memalign((void**)&imagem, 16, total_box*sizeof(int));
   retorno += posix_memalign((void**)&vx_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&vy_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&phi_local, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&angle_local, 64, total_box*sizeof(float));
      
    if(retorno > 0)printf("Problema com a posix dentro da save positions \n");
   */
   for(i=0;i<box_for_line*box_for_row;i++)
     {
	imagem[i]=0;
	vx_mean[i]=0;
	vy_mean[i]=0;
	phi_local[i]=0.0;
	angle_local[i]=10.0;
     }
   
   //printf(" aqui 3.7.8 \n");
   for(i=0;i<n;i++)
     {
	caixas_x=(int)(boid[i].x/R_MAX);
	caixas_y=(int)((Ly-boid[i].y)/R_MAX);
	imagem[caixas_y*box_for_line+caixas_x]++;
	vx_mean[caixas_y*box_for_line+caixas_x]+=boid[i].vx;
	vy_mean[caixas_y*box_for_line+caixas_x]+=boid[i].vy;
     }
   //printf(" aqui 3.7.9 \n");
   for(i=0;i<box_for_line*box_for_row;i++)
     {	
	if(imagem[i]>0)phi_local[i]=sqrt((vx_mean[i]*vx_mean[i])+(vy_mean[i]*vy_mean[i]))/imagem[i];
	if(imagem[i]>0)angle_local[i]=atan2(vy_mean[i],vx_mean[i]);
     }
   for(i=0;i<box_for_line*box_for_row;i++)	fprintf(file,"%d ",imagem[i]);
   for(i=0;i<box_for_line*box_for_row;i++)	fprintf(file2," %f ",phi_local[i]);
   //printf(" aqui 3.7.10 \n");
   for(i=0;i<box_for_line*box_for_row;i++)fprintf(file2," %f ",angle_local[i]);
   fprintf(file," \n ");
   fprintf(file2," \n ");
   fflush(stdout);
   fclose(file);
   fclose(file2);
 /*  free(imagem);
   free(vx_mean);
   free(vy_mean);
   free(phi_local);
   free(angle_local);
  */ 
}
