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

    | TESTE FLOAT {
		  printf("Imagem grande:\n");    	  
    	  teste("demo.jpg", $2);
    	  
    
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
  printf("EXEMPLO: imagem.jpg=demo.jpg*2 -p 2 para utilizar dois processos\n");
  printf("Para testar o desempenho de cada caso digite teste\n");
  FreeImage_Initialise(0);
  yyparse();
  return 0;

}
