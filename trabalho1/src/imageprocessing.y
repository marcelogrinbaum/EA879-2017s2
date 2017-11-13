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
%token <strval> STRING
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

    | STRING IGUAL STRING VEZES FLOAT {
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        brilho(&I,$5);
        printf("Salvando imagem em %s\n", $1);  
        salvar_imagem($1, &I);
    }
    
    | STRING IGUAL STRING DIV FLOAT {
        printf("Abrindo imagem %s\n", $3);
        imagem I = abrir_imagem($3);
        $5 = 1/$5;
        brilho(&I,$5);
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
  FreeImage_Initialise(0);
  yyparse();
  return 0;

}
