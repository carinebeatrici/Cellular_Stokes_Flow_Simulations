#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "param.h"
#include "particle.h"

#define TWO_PI 6.283185307

float gaussrand();
//float alignment(particle *boid);

int   update_position(particle *boid, float *sum_fx, float *sum_fy, int n, int last_id)
{
   float angle;//movement_angle;
   float rand_angle; 
   int i,j;
   int sai_dir=0;
   float densidade_reservatorio;
   int delta_n;
   sai_dir = 0;
   densidade_reservatorio = 0.0;
   delta_n = 0;
   float nx, ny, vr;
   
   float distance_center;
   float dx,dy;
   float proj; 
   
   for(i=0;i<n;i++)
     {
	if(boid[i].exist==1)
	  {
	     boid[i].vx = V0*cos(boid[i].theta) + MU*sum_fx[i];
	     boid[i].vy = V0*sin(boid[i].theta) + MU*sum_fy[i];  
	     
	     if(OBSTACULO == YES)
	       {
		  dx = boid[i].x-L_CENTRO_X;
		  dy = boid[i].y-L_CENTRO_Y;
		  distance_center = ((dx*dx)+(dy*dy));
		  if(distance_center <= R_ESFERA*R_ESFERA)
		    {
		       //vr = sqrt(boid[i].vx * boid[i].vx + boid[i].vy * boid[i].vy);
		       proj = boid[i].vx * dx + boid[i].vy * dy;
		       if (proj <= 0.0)
			 {
			    boid[i].vx -= proj * dx/(distance_center);
			    boid[i].vy -= proj * dy/(distance_center);
			 }
		    }
	       }
	     
	     boid[i].x += boid[i].vx * DT;
	     boid[i].y += boid[i].vy * DT;
	     
	     nx = cos(boid[i].theta);
	     ny = sin(boid[i].theta);
	     
	     vr = sqrt(boid[i].vx * boid[i].vx + boid[i].vy * boid[i].vy);
	     
	     angle = nx*boid[i].vy/vr - ny*boid[i].vx/vr;
	     if(angle>1.0)
	       {
		  if(angle>1.1)printf("ANGLE OUT [%d] = %f \n",i,angle);
		  angle=0.9999;
	       }	     
	     else if(angle<-1.0)
	       {
		  if(angle<-1.1)printf("ANGLE OUT [%d] = %f \n",i,angle);
		  angle=-0.9999;
	       }
	     else if(isnan(angle))
	       {
		  printf("ANGLE OUT [%d] = %f \n",i,angle);
		  printf("boid[%d]   vx = %f  vy = %f \n",i,boid[i].vx,boid[i].vy);
		  printf("boid[%d]    x = %f   y = %f \n",i,boid[i].x,boid[i].y);
		  angle = 0.0; //(2.0*drand48())-1.0;
	       }
	     
	     rand_angle = GAUSSIAN * NOISE * gaussrand() + WHITE * NOISE * (drand48()-0.5);	     
	     boid[i].theta += (asin(angle)/TAU) * DT + rand_angle * sqrt(DT);

/*	     FILE *file;
	     file=fopen("data//noise.dat","a");
	     fprintf(file," %f \n", rand_angle);
	     fclose(file); */
	     
	     
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
		       if(boid[i].vy < 0.0) boid[i].vy = -boid[i].vy;
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
		       if(boid[i].vy > 0.0) boid[i].vy = -boid[i].vy;
		    }
		  if(boid[i].y > Ly-R_CORE)
		    {
		       boid[i].y  = 2*(Ly-R_CORE) - boid[i].y;
		       if(boid[i].vy > 0.0) boid[i].vy = -boid[i].vy;
		    }
		  if(boid[i].x < R_CORE)
		    {
		       boid[i].x  = R_CORE - boid[i].x;
		       boid[i].vx = -boid[i].vx;
		    }
		  if(boid[i].y < R_CORE)
		    {
		       boid[i].y =  R_CORE + boid[i].y;
		       if(boid[i].vy < 0.0) boid[i].vy = -boid[i].vy;
		    }
		  
	       }
	     else
	       {
		  while(boid[i].x < 0.0) boid[i].x += Lx;
		  while(boid[i].y < 0.0) boid[i].y += Ly;
		  while(boid[i].x > Lx)  boid[i].x -= Lx;
		  while(boid[i].y > Ly)  boid[i].y -= Ly;
	       }
	     
	     if( boid[i].x < L_CANTO)
	       { 
		  densidade_reservatorio += 1;
	       }
	     
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
	     boid[i].theta = angle;
	     boid[i].vx    = cos(angle);
	     boid[i].vy    = sin(angle);
	     boid[i].x     = L_CANTO * drand48();
	     boid[i].y     = Ly * drand48();
	     boid[i].box   = -1;
	     boid[i].next  = -1;
	     boid[i].v0    = V1;
	     boid[i].type  = 0;
	     boid[i].exist = 1;
	     boid[i].dreq  = DISPERSION * (drand48() - 0.5);
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
		       boid[i].theta = boid[j].theta;
		       boid[i].x     = boid[j].x;
		       boid[i].y     = boid[j].y;
		       boid[i].vx    = boid[j].vx;
		       boid[i].vy    = boid[j].vy;
		       boid[i].next  = -1;
		       boid[i].type  = boid[j].type;
		       boid[i].v0    = boid[j].v0;
		       boid[i].dreq  = boid[j].dreq;
		       boid[i].id    = boid[j].id;
		       // making i exist again
		       // erasing j
		       boid[i].exist = 1;
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





float gaussrand()
{
   static float value1, value2, std; 
   static int phase = 0;
   float result;
   if(phase == 0)
     {
	do
	  {
	     value1 = drand48(); 
	     value1 = 2 * value1 - 1; 
	     value2 = drand48(); 
	     value2 = 2 * value2 - 1;
	     std = value1 * value1 + value2 * value2;
	  }while(std >= 1 || std == 0);
	result = value1 * sqrt(-2 * log(std)/std);
     }
   else
     {
	result = value2 * sqrt(-2 * log(std)/std);
     }
   phase = 1 - phase;
   return result;
}

/*
float alignment(particle *boid)
{
   float align;
   
   



   return align;
}
*/
