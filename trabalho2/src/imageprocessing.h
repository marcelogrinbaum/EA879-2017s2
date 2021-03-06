
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
float brilho_colunas(imagem *I, float n);
float brilho_linhas(imagem *I, float n);
float brilho_multithreads(imagem *I, float n, int num_threads);
float brilho_multiprocessos(imagem *I, float n, int num_processos);
void valor_maximo(imagem *I);
void teste(char *nome_do_arquivo, int n_de_testes);
#endif
