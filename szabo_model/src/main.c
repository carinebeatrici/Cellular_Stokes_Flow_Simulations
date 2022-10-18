/* Modelo de Szabo
 * Inicio 13/09/2021
 * 
 * Baseado no modelo de Vicsek
 * Inicio: 28/07/2016
 *  
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TWO_PI 6.283185307

#include "param.h"
#include "particle.h"

long int  initialize(particle *boid, int n);
void  save_position(FILE *arquivo, particle *boid, int time_step, int n);  
void  calculate_distance(particle *boid, float *sum_fx, float *sum_fy,
			 const int box_for_line,
			 const int box_for_row,  neighbor_box *neighbor_box_list, 
			 int *box_list, int n);
int update_position(particle *boid, float *sum_fx, float *sum_fy, int n, int last_id);
void calc_box(particle *boid, const int box_for_line, int n);
void SaveImageDensity(particle *boid, const int box_for_line, const int box_for_row,
		     int *imagem, float *vx_mean, float *vy_mean, float *phi_local, float *angle_local, int n);
float PolarOrderParameter(particle *boid, int time, int n);
float SolidLiquid( particle *boid, int time, int n, int *vizinhos_old, int *vizinhos, 
		   int *num_vizinhos, int *num_vizinhos_old, int *num_different);
void neighbor_box_calc(neighbor_box *neighbor_box_list, const int box_for_line, const int box_for_row);

int main (void)
{
   int n, delta_n;
   int i;
   int count, count_last;
   float *sum_fx = NULL, *sum_fy = NULL; 
   //float angle;
   particle *boid  = NULL;
   int *box_list  = NULL;
   int retorno = 0, retorno2 = 0;
   float tempo_init = 0 , tempo_calc_box = 0 , tempo_distance = 0 , tempo_file = 0, tempo_outro = 0, tempo_total = 0;
   clock_t clock_init, clock_end;
   clock_init = clock();
   long int time_step=0, tnp=0, time_out=0;
   const int box_for_line =  (int)(Lx/R_MAX);
   const int box_for_row  =  (int)(Ly/R_MAX);
   const int total_box = box_for_line * box_for_row;
   int *imagem; 
   float *vx_mean, *vy_mean;
   float *phi_local, *angle_local;
   float phi = 0.0;
   srand48(time(0));
   neighbor_box *neighbor_box_list = NULL;
   FILE *arquivo_saida;
   float max_x;
   int ja_foi = 0;
   float solid_liquid = 0.0;
   
   int *vizinhos_old     = NULL;
   int *vizinhos         = NULL;
   int *num_vizinhos     = NULL;
   int *num_vizinhos_old = NULL;
   int *num_different    = NULL;
   
   int last_id;

   // Allocating stuff   
   retorno +=  posix_memalign((void**)&box_list, 16, total_box*sizeof(int));
   if(retorno > 0)printf("Problema com a posix dentro da distancia \n");
   retorno += posix_memalign((void**)&boid, 64, N*sizeof(particle));
   if (retorno > 0) printf("Erro com a alocacao da posix boid \n");
   retorno += posix_memalign((void**)&sum_fx, 64, N*sizeof(float));
   if (retorno > 0) printf("Erro com a alocacao da posix sum_fx \n");
   retorno += posix_memalign((void**)&sum_fy, 64, N*sizeof(float));
   if (retorno > 0) printf("Erro com a alocacao da posix sum_fy \n");
   retorno += posix_memalign((void**)&neighbor_box_list, 64,  (total_box*sizeof(neighbor_box)));
   if (retorno > 0) printf("Erro com a alocacao da posix \n");
   
   posix_memalign((void**)&vizinhos_old, 16, MAX_VIZ*N*sizeof(int));
   posix_memalign((void**)&vizinhos, 16, MAX_VIZ*N*sizeof(int));
   posix_memalign((void**)&num_vizinhos, 16, N*sizeof(int));
   posix_memalign((void**)&num_vizinhos_old, 16, N*sizeof(int));
   posix_memalign((void**)&num_different, 16, N*sizeof(int));
   
   
   if(LOAD==0)
     {
	retorno2 += system("rm -f data//phi.dat");
	retorno2 += system("rm -f data//posicoes.dat");
	retorno2 += system("rm -f data//image.dat");
	retorno2 += system("rm -f data//localphi.dat");
	retorno2 += system("rm -f data//delta.dat");
     }
   if (retorno2 > 0) printf("Erro com a remoção dos arquivos \n");
 
   neighbor_box_calc(neighbor_box_list, box_for_line, box_for_row);
   retorno += posix_memalign((void**)&imagem, 16, total_box*sizeof(int));
   retorno += posix_memalign((void**)&vx_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&vy_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&phi_local, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&angle_local, 64, total_box*sizeof(float));
   
   if(SORTEIO==CANTO)
     {
       n = L_CANTO * Ly * RESEVOIR_DENSITY;
     }
   else if(SORTEIO == EVERYWHERE)
     {
	n = N0;
     }
   else
     {
       n = N;
     }
   last_id = n;
   time_step = initialize(boid, n);
   if (LOAD == 0)
     {
	printf("Numero inicial de boids........... %d ................................... OK\n",n);
     }
   else
     {
	n=0;
	for(i=0; i<N; i++)if(boid[i].exist==1) n++;
	printf("Numero inicial de boids........... %d ................................... OK\n",n);
     }
   

   tnp = time_step+pow(time_step,0.8);
   fflush(stdout);
   arquivo_saida = fopen("data//posicoes.dat","a");   
   fflush(stdout);

   clock_end = clock();
   tempo_init = clock_end - clock_init;
   clock_init = clock();
   calc_box(boid, box_for_line, n);
   clock_end = clock();
   tempo_calc_box = clock_end - clock_init;
   clock_init = clock();
   
   printf("Inicio da execução......................................................... OK\n");
   printf("Numero de particulas:.................. %d .............................. OK \n",N);
   printf("Tipo de sorteio: %d ........................................................ OK \n",SORTEIO);
   printf("Tau:..%f ................................................................. \n",   TAU);
   printf("F_adh:..%f ................................................................. \n",  F_ADH);
   printf("Velocidade: .... %f........................................... OK \n",V1);
   printf("Lx: ........ %d ........ Ly: ......... %d ........................... OK \n",Lx,Ly);
   if(LOAD == YES)
     {
	printf("Tempo: ........ %li ............. ............................................ OK \n",time_step);
     }
   
   if(LOAD == NO)
     {
	save_position(arquivo_saida, boid, time_step, n);
	phi = PolarOrderParameter(boid, time_step, n);
	solid_liquid = SolidLiquid(boid, time_step, n, vizinhos_old, vizinhos, 
				   num_vizinhos, num_vizinhos_old, num_different);
	printf("Salvando posicoes  t: %li phi: %f Delta: %f n: %d front: --- last: -- delta_n: -- \n",time_step,phi,solid_liquid,n);
     }
   
   clock_end = clock();
   tempo_file = clock_end - clock_init;
   clock_init = clock();  
   time_step = 1;
   if(SORTEIO == EVERYWHERE)ja_foi = 1;
   while (time_step < TIME_FINAL)
     {
	if(ja_foi == 0)
	  {
	     time_out ++; 
	     max_x =  boid[0].x;
	     for(i=1; i<n; i++)
	       {
		  if(boid[i].x > max_x)
		    max_x = boid[i].x;
	       }
	      count_last = 0;
	     for(i=0;i<n;i++)
	       {	  
		  if(boid[i].x > (Lx-5)-(3*R_MAX) && boid[i].x < (Lx-5)-(2*R_MAX))count_last++;
	       }
	     
	     
	     if(time_out % SNAPSHOT == 0)
	       printf("time step throw away: %li max x: %f barreira: %f particles: %d densidade end: %f\n",
		      time_out, max_x, L_CENTRO_X+(R_FRONT*R_ESFERA), n, count_last/(L_CANTO*Ly));
	     if(max_x >= L_CENTRO_X+(R_FRONT*R_ESFERA))
	     //if(count_last/(L_CANTO*Ly) > 0.2 || R_FRONT < 0)
	       ja_foi = 1;
	  }
	else
	  {
	     time_step++;
	  }	
	calc_box(boid, box_for_line, n);
	clock_end = clock();
	tempo_calc_box += clock_end - clock_init;
	clock_init = clock();
	calculate_distance(boid, sum_fx, sum_fy, box_for_line, box_for_row, neighbor_box_list, box_list, n);
	clock_end = clock();
	tempo_distance += clock_end - clock_init;
	clock_init = clock();
	delta_n = update_position(boid, sum_fx, sum_fy, n, last_id);
	if(delta_n>0)last_id += delta_n;
	if(n+delta_n <= N)
	  { 
	     n += delta_n;
	  }
	calc_box(boid, box_for_line, n);
	
	clock_end = clock();
	tempo_outro += clock_end - clock_init;
	clock_init = clock();
	if(SNAPSHOT==-1)
	  {
	     if(time_step>=tnp)
	       {
		  tnp=tnp+pow(time_step,0.8);
		  save_position(arquivo_saida, boid, time_step, n);
//		  SaveImageDensity(boid, box_for_line, box_for_row,  imagem, vx_mean, vy_mean, phi_local, angle_local, n);
		  phi = PolarOrderParameter(boid, time_step, n);
		  solid_liquid = SolidLiquid(boid, time_step, n, vizinhos_old, vizinhos, 
					     num_vizinhos, num_vizinhos_old, num_different);
		  printf("Salvando posicoes %li %f %d %f\n",time_step,phi,n,solid_liquid);
		  clock_end = clock();
		  tempo_file += clock_end - clock_init;
		  clock_init = clock();
		  
	       }
	  }
	else
	  {
	     if(time_step%SNAPSHOT==0)
	       {
		  save_position(arquivo_saida, boid, time_step, n);
//		  SaveImageDensity(boid, box_for_line, box_for_row,  imagem, vx_mean, vy_mean, phi_local, angle_local, n);
		  phi = PolarOrderParameter(boid, time_step, n);
		  solid_liquid = SolidLiquid(boid, time_step, n, vizinhos_old, vizinhos, 
					     num_vizinhos, num_vizinhos_old, num_different);
		  max_x =  boid[0].x;
		  for(i=1; i<n; i++)
		    {     
		       if(boid[i].x > max_x)
			 max_x = boid[i].x;
		    }
		  count = 0;
		  count_last = 0;
		  for(i=0;i<n;i++)
		    {
		       if(boid[i].exist==1)count++;
		       if(boid[i].x > (Lx-5)-(3*R_MAX) && boid[i].x < (Lx-5)-(2*R_MAX))count_last++;
		    }
		  
		  printf("Salvando posicoes  t: %li phi: %f Delta: %f n: %d front: %f last: %d delta_n: %d \n",time_step,phi,solid_liquid,n, max_x, count_last, delta_n);
		  clock_end = clock();
		  tempo_file += clock_end - clock_init;
		  clock_init = clock();
	       }
	  }
     }
   // LIBERATING MEMORY
   clock_end = clock();
   tempo_outro += clock_end - clock_init;
   tempo_total = tempo_init+tempo_calc_box+tempo_distance+tempo_file+tempo_outro;
   printf("tempo total %f s \n", tempo_total/((float)CLOCKS_PER_SEC));
   printf("tempo init  %f s \n", tempo_init/((float)CLOCKS_PER_SEC));
   printf("tempo box   %f s \n", tempo_calc_box/((float)CLOCKS_PER_SEC));
   printf("tempo file  %f s \n", tempo_file/((float)CLOCKS_PER_SEC));
   printf("tempo dist  %f s \n", tempo_distance/((float)CLOCKS_PER_SEC));
   printf("tempo other %f s \n", tempo_outro/((float)CLOCKS_PER_SEC));
   printf("Final da execução :D \n");
   return 0;
}

