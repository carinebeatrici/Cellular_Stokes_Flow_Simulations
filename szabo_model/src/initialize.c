#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "param.h"
#include "particle.h"

#define TWO_PI 6.283185307

long int initialize(particle *boid, int n)
{
   int i;  
   float angle;
   float angle_space;
   float radius;
   int raiosorteio=(int)((int)(Lx-8.)/2);
   long int time_step = 0, aux_time_step;
   int n_read;
   long int contador;
   FILE *arquivo_saida;
   if(SORTEIO==QUADRADO)
     {
	for(i=0;i<N;i++)
	  {
	     angle          = drand48();
	     angle         *= TWO_PI;
	     boid[i].theta  = angle; 
	     boid[i].vx     = cos(angle);
	     boid[i].vy     = sin(angle);
	     boid[i].x      = Lx/3. * drand48() + Lx/3.;
	     boid[i].y      = Ly/3. * drand48() + Ly/3.;
	     boid[i].box    = -1;	
	     boid[i].next   = -1;	
	     boid[i].v0     = V1;
	     boid[i].type   = 0;	
	     boid[i].exist  = 1;	
	     boid[i].dreq   = (DISPERSION * (drand48() - 0.5));
	     boid[i].id     = i;
	  }
     }
   else if(SORTEIO==CIRCULO)
     {
	for(i=0;i<N;i++)
	  {
	     angle          = drand48();
	     angle         *= TWO_PI;
	     angle_space    =  drand48();
	     angle_space   *= TWO_PI;
	     radius         = drand48();
	     radius         = sqrt(radius);
	     boid[i].theta  = angle; 
	     boid[i].vx     = cos(angle);
	     boid[i].vy     = sin(angle);
	     boid[i].x      = raiosorteio*radius*cos(angle_space) + Lx/2.;
	     boid[i].y      = raiosorteio*Ly/Lx*radius*sin(angle_space) + Ly/2.;
	     boid[i].box    = -1;	
	     boid[i].next   = -1;	
	     boid[i].v0     = V1;
	     boid[i].type   = 0;	
	     boid[i].exist  = 1;	
	     boid[i].dreq   =  (DISPERSION * (drand48() - 0.5));
	     boid[i].id     = i;
	  }
     }
   else if(SORTEIO==CANTO)
     {
	for(i=0;i<n;i++)
	  {
	     angle          = drand48();
	     angle         *= TWO_PI;
	     boid[i].theta  = angle; 
	     boid[i].vx     = cos(angle);
	     boid[i].vy     = sin(angle);
	     boid[i].x      = L_CANTO * drand48();
	     boid[i].y      = Ly * drand48();
	     boid[i].box    = -1;	
	     boid[i].next   = -1;	
	     boid[i].v0     = V1;
	     boid[i].type   = 0;	
	     boid[i].exist  = 1;
	     boid[i].dreq   =  (DISPERSION * (drand48() - 0.5));
	     boid[i].id     = i;
	  }	
     }
   else if(SORTEIO==EVERYWHERE)
     {
	for(i=0;i<N0;i++)
	  {   
	     angle          = drand48();
	     angle         *= TWO_PI;
	     boid[i].theta  = angle; 
	     boid[i].vx     = cos(angle);
	     boid[i].vy     = sin(angle);
	     boid[i].x      = (Lx-5) * drand48();
	     boid[i].y      = Ly * drand48();
	     radius         = sqrt((boid[i].x-L_CENTRO_X)*(boid[i].x-L_CENTRO_X) + 
				   (boid[i].y-L_CENTRO_Y)*(boid[i].y-L_CENTRO_Y));
	     if (OBSTACULO == YES)
	       {
		  while(radius <= R_ESFERA)
		    {
		       boid[i].x  = Lx * drand48();
		       boid[i].y  = Ly * drand48();
		       radius     = sqrt((boid[i].x-L_CENTRO_X)*(boid[i].x-L_CENTRO_X) + 
					 (boid[i].y-L_CENTRO_Y)*(boid[i].y-L_CENTRO_Y));
//		       printf("%f new radius: %f \n", R_ESFERA, radius);
		    }
	       }
	     boid[i].box    = -1;
	     boid[i].next   = -1;
	     boid[i].v0     = V1;
	     boid[i].type   = 0;
	     boid[i].exist  = 1;
	     boid[i].dreq   =  (DISPERSION * (drand48() - 0.5));
	     boid[i].id     = i;
	  }
	for(i = N0; i < N; i++)
	  {
	     boid[i].exist = 0;
	  }
     } 
   if(LOAD == 1)
     {
	for(i = 0; i < N; i++)boid[i].exist = 0;
	contador = 1;
	arquivo_saida=fopen("data//posicoes.dat","r");
	printf("Lendo arquivo...........posicoes.dat............................................... \n");
	while(fscanf(arquivo_saida,"%li %d",&aux_time_step,&n_read)!= EOF)
	  {
	     time_step = aux_time_step;
	     printf("tempo: %li .......... numero de particulas:  %d ............ linha do arquivo: %li .........\n",time_step,n_read,contador);
	     contador++;
	     n = n_read;
	     //	   printf("Lendo arquivo tempo final atual: %li \n",aux_time_step);
	     if(n_read!=n)printf("!!!!!!!!!!!!!!!!!!!!!!!Numero!!de!!particulas!!foi!!alterado!!!!!!!!!!\n");
	     for(i=0;i<n_read;i++)
	       {
		  contador++;
		  aux_time_step = fscanf(arquivo_saida,"%f %f %f %f %d %f %*d",&boid[i].x,&boid[i].y,&boid[i].vx,&boid[i].vy,&boid[i].box,&boid[i].dreq);
//		  printf("%f %f %f %f %d \n",boid[i].x,boid[i].y,boid[i].vx,boid[i].vy,boid[i].box);
		  if(aux_time_step==EOF)
		    {
		       printf("******Aviso*****deu*****EOF****na*leitura*das*coordenadas********\n");
		       printf("tempo: %li .......... numero de particulas:  %d ............ linha do arquivo: %li .........\n",time_step,n,contador);
		       return(time_step);
		    }
		  boid[i].exist = 1;
		  boid[i].v0    = V1;
		  boid[i].theta = atan2(boid[i].vy,boid[i].vx);
		  boid[i].id    = i;
//		  boid[i].dreq   = DISPERSION * drand48();
	       }
	  }
	
	if(arquivo_saida!=NULL)fclose(arquivo_saida); 
	printf("tempo: %li .......... numero de particulas:  %d ............ linha do arquivo: %li .........\n",time_step,n,contador);
     }

   return(time_step);
}
