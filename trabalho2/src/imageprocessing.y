%{
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

void yyerror(char *c);
int yylex(void);

%}
%union {
  char    strval[50];
  int     ival;
  float   fval;
}
%token <strval> STRING THREAD PROCESSO COL LIN TESTE
%token <ival> VAR IGUAL EOL ASPA VEZES DIV ABRE FECHA 
%token <fval> FLOAT 
%left SOMA VEZES

%%

PROGRAMA:
        PROGRAMA EXPRESSAO EOL
        |
        ;

EXPRESSAO:
    | STRING IGUAL STRING {
        printf("Copiando %s para %s\n", $3, $1);
        imagem I = abrir_imagem($3);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($1, &I);
        liberar_imagem(&I);
    }

    | STRING IGUAL STRING VEZES FLOAT COL{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        brilho_colunas(&I,$5);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }
    
    | STRING IGUAL STRING VEZES FLOAT LIN{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        brilho_linhas(&I,$5);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }

    | STRING IGUAL STRING VEZES FLOAT THREAD FLOAT{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        brilho_multithreads(&I,$5,$7);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }

    | STRING IGUAL STRING VEZES FLOAT PROCESSO FLOAT{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        brilho_multiprocessos(&I,$5,$7);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }

    | STRING IGUAL STRING DIV FLOAT COL{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        $5 = 1/$5;
        brilho_colunas(&I,$5);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }

    | STRING IGUAL STRING DIV FLOAT LIN{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        $5 = 1/$5;
        brilho_linhas(&I,$5);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }

    | STRING IGUAL STRING DIV FLOAT THREAD FLOAT{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        $5 = 1/$5;
        brilho_multithreads(&I,$5,$7);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }

    | STRING IGUAL STRING DIV FLOAT PROCESSO FLOAT{
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        $5 = 1/$5;
        brilho_multiprocessos(&I,$5,$7);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }

    | ABRE STRING FECHA {
        printf("Abrindo imagem %s\n", $2);
        imagem I = abrir_imagem($2);
        valor_maximo(&I);
    }

    | TESTE {
		
		imagem I = abrir_imagem("demo.jpg");
		int media = 0;  		
  		printf("Colunas:\n");
  		for(int i=0; i<10;i++){
    		media += brilho_colunas(&I,2);
  		}
		media=media/10; 		
		if(media<100000) 		
 			printf(CYAN "Média colunas: 0.0%d\n" RESET,media);
		else
			printf(CYAN "Média colunas: 0.%d\n" RESET,media);		
  		
  		
  	   media = 0;
  		I = abrir_imagem("demo.jpg");
  		printf("Linhas:\n");
  		for(int i=0; i<10;i++){
    		media += brilho_linhas(&I,2);
  		}
  		media=media/10; 		
		if(media<100000) 		
 			printf(CYAN "Média linhas: 0.0%d\n" RESET,media);
		else
			printf(CYAN "Média linhas: 0.%d\n" RESET,media);
 		
 		
  		I = abrir_imagem("demo.jpg");
  		printf("Threads:\n");
  		for(int i=1; i<9;i++){
  			media=0;
    		printf("Número de threads: %d\n", i);
    		for(int j=0; j<10; j++)
      		media += brilho_multithreads(&I,2,i);
      	media=media/10; 		
			if(media<100000) 			
 				printf(CYAN "Média usando %d threads: 0.0%d\n" RESET,i,media);
 			else
 				printf(CYAN "Média usando %d threads: 0.%d\n" RESET,i,media);
  		}
  
  		I = abrir_imagem("demo.jpg");
  		printf("Processos:\n");
  		for(int i=1; i<9;i++){
  			media=0;
    		printf("Número de processos: %d\n", i);
    		for(int j=0; j<10; j++)
      		media += brilho_multiprocessos(&I,2,i);
        	media=media/10; 		
			if(media<100000) 			
 				printf(CYAN "Média usando %d processos: 0.0%d\n" RESET,i,media);
 			else
 				printf(CYAN "Média usando %d processos: 0.%d\n" RESET,i,media);
  		}    
    }
    
    
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
  printf("INSTRUÇÕES\n");
  printf("Adicionar os singuintes indicadores após a instrução do brilho:\n");
  printf("-t n para n threads\n");
  printf("-p n para n processos\n");
  printf("-l para varrer a imagem por linhs\n");
  printf("-c para varrer a imagem por colunas\n");
  printf("EXEMPLO: teste.jpg=demo.jpg*2 -p 2 para utilizar dois processos\n");

  FreeImage_Initialise(0);
  yyparse();
  return 0;

}
