
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

#include "imageprocessing.h"

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

void brilho_(imagem *I, float n){ 

  struct timeval rt0, rt1, drt;
  
  gettimeofday(&rt0, NULL);

  printf("Multiplicando todods os pixels por %f\n", n);
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
  printf(RESET "Tempo: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);
}

void brilho_multithreads(imagem *I, float n){ 

  struct timeval rt0, rt1, drt;

  pthread_t t1, t2, t3;
    
  gettimeofday(&rt0, NULL);

  printf("Multiplicando todods os pixels por %f\n", n);
  
  void* linhas1(void *arg) {
     for (int i=0; i<(I->height)/3; i++) {
       for (int j=0; j<I->width; j++) {
        int idx;
        idx = i + (j*I->height);
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
    
  void* linhas2(void *arg) { 
     for (int i=((I->height)/3)+1; i<2*(I->height)/3; i++) {
       for (int j=0; j<I->width; j++) {
        int idx;
        idx = i + (j*I->height);
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

   void* linhas3(void *arg) {
     for (int i=(2*(I->height)/3)+1; i<I->height; i++) {
       for (int j=0; j<I->width; j++) {
        int idx;
        idx = i + (j*I->height);
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


  pthread_create(&t1, NULL, linhas1, NULL);  
  pthread_create(&t2, NULL, linhas2, NULL);
  pthread_create(&t3, NULL, linhas3, NULL); 
  pthread_join (t1, NULL);
  pthread_join (t2, NULL);  
  pthread_join (t3, NULL);  

  gettimeofday(&rt1, NULL);
    
  timersub(&rt1, &rt0, &drt);  
  printf(GREEN "Tempo: %ld.%06ld segundos\n" RESET, drt.tv_sec, drt.tv_usec);
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
