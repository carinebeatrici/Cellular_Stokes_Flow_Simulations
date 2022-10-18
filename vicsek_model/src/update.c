#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "param.h"
#include "particle.h"

#define TWO_PI 6.283185307

int   update_position(particle *boid, float *sum_vx, float *sum_vy,
		      float *sum_fx, float *sum_fy, int n, int last_id)
{
   float angle;//movement_angle;
   int i,j;
   float modulo_deslocamento;
   float componente_x,componente_y;
//   int ent_esq=0, sai_dir=0, ent_dir=0;
   int sai_dir=0;
   float densidade_reservatorio;
   int delta_n;
   float new_x, new_y;
   //ent_esq = 0;
   sai_dir = 0;
   //ent_dir = 0;
   densidade_reservatorio = 0.0;
   delta_n = 0;
   for(i=0;i<n;i++)
     {
	if(boid[i].exist==1)
	  {
	     
	     angle =  TWO_PI * drand48();

	     componente_x =  sum_vx[i]+sum_fx[i]+ETA*cos(angle);
	     componente_y =  sum_vy[i]+sum_fy[i]+ETA*sin(angle);
	     
	     modulo_deslocamento = sqrt((componente_x*componente_x)+(componente_y*componente_y));
	     
	     boid[i].vx =  boid[i].v0 * componente_x/modulo_deslocamento;
	     boid[i].vy =  boid[i].v0 * componente_y/modulo_deslocamento;
	     
	     new_x = boid[i].x + boid[i].vx;
	     new_y = boid[i].y + boid[i].vy;
	     
	     
//	     if(i==0)printf("vx: %f vy: %f fx: %f fy: %f nx: %f ny: %f \n",sum_vx[i],sum_vy[i],sum_fx[i],sum_fy[i],ETA*cos(angle),ETA*sin(angle));
	     
	     //contagem das particulas que passaram ou não
	     /*if(SORTEIO == CANTO || SORTEIO == EVERYWHERE)
	       {
		  if(new_x >= Lx)ent_esq++;
		  if(boid[i].x<L_CANTO && new_x>L_CANTO) sai_dir++;
		  if(boid[i].x>L_CANTO && new_x<L_CANTO) ent_dir++;
	       }
	     */
	     boid[i].x = new_x; 
	     boid[i].y = new_y;
	     // condicoes de contorno
	     if(SORTEIO == CANTO || SORTEIO == EVERYWHERE)
	       {
		  while(boid[i].x < 0.0)
		    {
		       boid[i].x  = -boid[i].x;
		       boid[i].vx = -boid[i].vx;
		    }
		  while(boid[i].y < 0.0)
		    {
		       boid[i].y  = -boid[i].y;
		       boid[i].vy = -boid[i].vy;
		    }
		  if(KILL == NO)
		    {
		       while(boid[i].x > Lx)
			 {
			    boid[i].x -= Lx;
			 }
		    }
		  if(KILL == YES)
		    {
		       if(boid[i].x >= (Lx-5))
			 {
			    sai_dir++;
			    boid[i].exist = 0;
			 }
		    }
		  while(boid[i].y  >= Ly)
		    {
		       boid[i].y  =  2.0 * Ly - boid[i].y;
		       boid[i].vy =  -boid[i].vy;
		    }		
	       }
	     else
	       {
		  while(boid[i].x < 0.0) boid[i].x += Lx;
		  while(boid[i].y < 0.0) boid[i].y += Ly;
		  while(boid[i].x > Lx)  boid[i].x -= Lx;
		  while(boid[i].y > Ly)  boid[i].y -= Ly;
	       }
	     
	     //	boid[i].angle = movement_angle;
	     //	if(SORTEIO==CANTO)
	     //	  {
	     if( boid[i].x < L_CANTO)
	       { 
		  densidade_reservatorio += 1;
	       }
	     //	  }
	  }
     }
   
   if(SORTEIO == CANTO || SORTEIO == EVERYWHERE)
     {
	if(CREATION_PROBABILITY >= 1.0)
	  {   
	     delta_n = (int)CREATION_PROBABILITY;
	  }
	else
	  {
	     float create = drand48();
	     if(create < CREATION_PROBABILITY) delta_n = 1.0;
	  }
	
	
          //delta_n = (int) ( RESEVOIR_DENSITY*(L_CANTO*Ly) - densidade_reservatorio);   
	densidade_reservatorio /= (1.0 * L_CANTO * Ly);
	//       printf("sai_dir: %d ent_dir: %d ent_esq: %d  total: %d  dens. reser.: %f dn: %d n: %d \n",sai_dir, ent_dir, ent_esq, (sai_dir - ent_dir - ent_esq), densidade_reservatorio,delta_n,n);
     }
   else
     {
	delta_n = 0;
     }
   
   // creating new particles
   if(delta_n > 0 && n+delta_n <= N)
     {
	for(i=n; i<n+delta_n; i++)
	  {   
	     angle         = drand48()*TWO_PI;
	     boid[i].vx    = cos(angle);
	     boid[i].vy    = sin(angle);
	     boid[i].x     = L_CANTO * drand48();
	     boid[i].y     = Ly * drand48();
	     boid[i].box   = -1;
	     boid[i].next  = -1;
	     boid[i].v0    = V1;
	     boid[i].type  = 0;
	     boid[i].exist = 1;
	     boid[i].id    = last_id;
	     last_id++;
	  }
     }
   
      
   // cleaning the vector, coping the particles in the end to the empty earlier positions
   for(i=0; i<n+delta_n; i++)
     {   
	//find the first one without particle
	if(boid[i].exist == 0)
	  {
	     for(j = n+delta_n; j > i; --j)
	       {
		  //find the last one with a particle
		  if(boid[j].exist == 1)
		    {
		       //copying j to i
		       boid[i].x    = boid[j].x;
		       boid[i].y    = boid[j].y;
		       boid[i].vx   = boid[j].vx;
		       boid[i].vy   = boid[j].vy;
		       boid[i].next = -1;
		       boid[i].type = boid[j].type;
		       boid[i].v0   = boid[j].v0;
		       // making i exist again
		       // erasing j
		       boid[i].exist = 1;
		       boid[i].id    = boid[j].id;
		       boid[j].exist = 0;
		       j = i;
		    }
	       }
	  }
	
     }
   
   if(KILL == YES)
     {
	delta_n -= sai_dir;
     }
   
   return(delta_n);
}

