%{
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>

void yyerror(char *c);
int yylex(void);

%}
%union {
  char    strval[50];
  int     ival;
  float   fval;
}
%token <strval> STRING THREAD PROCESSO COL LIN
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
