
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "imageprocessing.h"
#include <sys/mman.h>
#include <FreeImage.h>


#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
*/

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
    printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
  return I;

}

void liberar_imagem(imagem *I) {
  free(I->r);
  free(I->g);
  free(I->b);
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = I->r[idx];
      color.rgbGreen = I->g[idx];
      color.rgbBlue = I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}

int brilho_colunas(imagem *I, float n){ 

  struct timeval rt0, rt1, drt;
  
  gettimeofday(&rt0, NULL);

  //printf("Multiplicando todods os pixels por %f\n", n);
  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;
      idx = i + (j*I->width);
      I->r[idx] = I->r[idx]*n;
      if (I->r[idx] > 255) {
        I->r[idx] = 255;
      }
      I->g[idx] = I->g[idx]*n;
      if (I->g[idx] > 255) {
        I->g[idx] = 255;
      }
      I->b[idx] = I->b[idx]*n;
      if (I->b[idx] > 255) {
        I->b[idx] = 255;
      }
    }
  }
  gettimeofday(&rt1, NULL);
    
  timersub(&rt1, &rt0, &drt);  
  //printf(GREEN "Tempo: %ld.%06ld segundos\n" RESET, drt.tv_sec, drt.tv_usec);
  return drt.tv_usec;
}

int brilho_linhas(imagem *I, float n){ 

  struct timeval rt0, rt1, drt;
  
  gettimeofday(&rt0, NULL);

  //printf("Multiplicando todods os pixels por %f\n", n);
  for (int i=0; i<I->height; i++) {
     for (int j=0; j<I->width; j++) {
      int idx;

      idx = j + (i*I->width);
      I->r[idx] = I->r[idx]*n;
      if (I->r[idx] > 255) {
        I->r[idx] = 255;
      }
      I->g[idx] = I->g[idx]*n;
      if (I->g[idx] > 255) {
        I->g[idx] = 255;
      }
      I->b[idx] = I->b[idx]*n;
      if (I->b[idx] > 255) {
        I->b[idx] = 255;
      }
    }
  }
  gettimeofday(&rt1, NULL);
    
  timersub(&rt1, &rt0, &drt);  
  //printf(GREEN "Tempo: %ld.%06ld segundos\n" RESET, drt.tv_sec, drt.tv_usec);
  return drt.tv_usec;
}

int brilho_multithreads(imagem *I, float n, int num_threads){ 

  struct timeval rt0, rt1, drt;

  pthread_t t[8];
    
  gettimeofday(&rt0, NULL);

  //printf("Multiplicando todods os pixels por %f\n", n);
  
  void* linhas(void *arg) {
    int* num = (int*)arg;
    int n_da_thread = (*num);
     for (int i=(n_da_thread*((I->height)/num_threads)); i<((n_da_thread+1)*((I->height)/num_threads)); i++) {
       for (int j=0; j<I->width; j++) {
        int idx;
        idx = j + (i*I->width);
        I->r[idx] = I->r[idx]*n;
        if (I->r[idx] > 255) {
          I->r[idx] = 255;
        }
        I->g[idx] = I->g[idx]*n;
        if (I->g[idx] > 255) {
          I->g[idx] = 255;
        }
        I->b[idx] = I->b[idx]*n;
        if (I->b[idx] > 255) {
          I->b[idx] = 255;
        }
      }
	 }  
  }
  
  int args[8];
  for(int i=0; i<num_threads; i++)
    args[i] = i;
  for(int i=0; i<num_threads; i++)
    pthread_create(&(t[i]), NULL, linhas, &(args[i])); 
  for(int i=0; i<num_threads; i++) 
    pthread_join(t[i], NULL);
  // int a=0, b=1;
  // pthread_create(&t[0], NULL, linhas, &a);  
  // pthread_create(&t[1], NULL, linhas, &b);
  // //pthread_create(&t3, NULL, linhas3, NULL); 
  // pthread_join (t2, NULL);  
  // //pthread_join (t3, NULL);  

  gettimeofday(&rt1, NULL);
    
  timersub(&rt1, &rt0, &drt);  
  //printf(GREEN "Tempo: %ld.%06ld segundos\n" RESET, drt.tv_sec, drt.tv_usec);
  return drt.tv_usec;
}

int brilho_multiprocessos(imagem *I, float n, int num_processos){

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANONYMOUS;
  float *r = (float*) mmap(NULL, sizeof(float)*(I->height)*(I->width), protection, visibility, 0, 0);
  float *g = (float*) mmap(NULL, sizeof(float)*(I->height)*(I->width), protection, visibility, 0, 0);
  float *b = (float*) mmap(NULL, sizeof(float)*(I->height)*(I->width), protection, visibility, 0, 0);

  void linhas(int n_do_processo) {
     for (int i=(n_do_processo*((I->height)/num_processos)); i<((n_do_processo+1)*((I->height)/num_processos)); i++) {
       for (int j=0; j<I->width; j++) {
        int idx;
        idx = j + (i*I->width);
        r[idx] = I->r[idx]*n;
        if (r[idx] > 255) {
          r[idx] = 255;
        }
        g[idx] = I->g[idx]*n;
        if (g[idx] > 255) {
          g[idx] = 255;
        }
        b[idx] = I->b[idx]*n;
        if (b[idx] > 255) {
          b[idx] = 255;
        }
      }
    }  
  }

  struct timeval rt0, rt1, drt;
    
  gettimeofday(&rt0, NULL);

  pid_t *pids = malloc(sizeof(pid_t)*num_processos);
  int *args = malloc(sizeof(int)*num_processos);;
  // for(int i=0; i<num_processos; i++)
  //   args[i] = i;
  for (int i=0; i<num_processos; i++){
    pids[i] = fork();
    if (pids[i] < 0){
      printf("Erro ao fazer fork\n");
      exit(EXIT_FAILURE);
    }else if (pids[i] == 0){ //Processo filho
      linhas(i);            
      exit (EXIT_SUCCESS);
    }
  }  

  int status;
  pid_t pid;
  int k=0;
  while (k < num_processos) {
    pid = wait(&status);
    k++; 
  }

  int x, y, idx;

  for (y=0; y<I->height; y++){
    for (x=0; x<I->width; x++){
      idx = x + y*(I->width);
      I->r[idx] = r[idx];
      I->g[idx] = g[idx];
      I->b[idx] = b[idx];
    }
  }

  free (pids);
  munmap (r, sizeof(float)*(I->height)*(I->width));
  munmap (g, sizeof(float)*(I->height)*(I->width));
  munmap (b, sizeof(float)*(I->height)*(I->width));  

  gettimeofday(&rt1, NULL);
    
  timersub(&rt1, &rt0, &drt);  
  //printf(GREEN "Tempo: %ld.%06ld segundos\n" RESET, drt.tv_sec, drt.tv_usec);
  return drt.tv_usec;
}

void teste(char *nome_do_arquivo, int n_de_testes){
	imagem I = abrir_imagem(nome_do_arquivo);
	int media = 0;  		
  	printf("Colunas:\n");
  	for(int i=0; i<n_de_testes;i++){
   	media += brilho_colunas(&I,2);
  	}
	media=media/n_de_testes; 		
	if(media<100000) 		
 		printf(GREEN "Média colunas: 0.0%d\n" RESET,media);
	else
		printf(GREEN "Média colunas: 0.%d\n" RESET,media);		
  	
  	
  	media = 0;
  	I = abrir_imagem(nome_do_arquivo);
  	printf("Linhas:\n");
  	for(int i=0; i<n_de_testes;i++){
   	media += brilho_linhas(&I,2);
  	}
  	media=media/n_de_testes; 		
	if(media<100000) 		
 		printf(GREEN "Média linhas: 0.0%d\n" RESET,media);
	else
		printf(GREEN "Média linhas: 0.%d\n" RESET,media);
 	
 	
  	I = abrir_imagem(nome_do_arquivo);
  	printf("Threads:\n");
  	for(int i=1; i<9;i++){
  		media=0;
   	printf("Número de threads: %d\n", i);
   	for(int j=0; j<n_de_testes; j++)
     		media += brilho_multithreads(&I,2,i);
     	media=media/n_de_testes; 		
		if(media<100000) 			
 			printf(GREEN "Média usando %d threads: 0.0%d\n" RESET,i,media);
 		else
 			printf(GREEN "Média usando %d threads: 0.%d\n" RESET,i,media);
  	}
  
  	I = abrir_imagem(nome_do_arquivo);
  	printf("Processos:\n");
  	for(int i=1; i<9;i++){
  		media=0;
   	printf("Número de processos: %d\n", i);
   	for(int j=0; j<n_de_testes; j++)
   		media += brilho_multiprocessos(&I,2,i);
      media=media/n_de_testes; 		
		if(media<100000) 			
 			printf(GREEN "Média usando %d processos: 0.0%d\n" RESET,i,media);
 		else
 			printf(GREEN "Média usando %d processos: 0.%d\n" RESET,i,media);
  	}
}

void valor_maximo(imagem *I){
  printf("Calculando o valor máximo...\n");
  double max = 0;
  double norma = 0;
  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;
      idx = i + (j*I->width);

      norma = sqrt(pow(I->r[idx],2)+pow(I->g[idx],2)+pow(I->b[idx],2)); 
      
      if(norma > max)
        max = norma;
    }
  }
  printf("O valor máximo é: %lf\n", norma);  
}


