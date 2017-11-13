
%{
#include <stdlib.h>
#include <string.h>
void yyerror(char *);

#include "y.tab.h"
%}



%%

([0-9]*[.])?[0-9]+ {
  yylval.fval = atof(yytext);
  printf("Achei um FLOAT %f\n", yylval.fval);
  return FLOAT;
}

\[ {
  printf("Abriu\n");
  return ABRE;
}

\] {
  printf("Fechou\n");
  return FECHA;
}

[a-z]  {
        yylval.ival = yytext[0] - 'a';
        return VAR;
       }

\" { return ASPA; }

[a-zA-Z0-9\.]+  {
        for (int i = 0; i < yyleng; i++ )
          yylval.strval[i] = yytext[i];

        yylval.strval[yyleng] = '\0';
        printf ("Achei nome: %s\n", yylval.strval);
        return STRING;
       }

[=]     {
          return IGUAL;
        }

[*] {
  printf("Achei um VEZES\n");
  return VEZES;
}

[/] {
  printf("Achei um DIV\n");
  return DIV;
}

\n   {
         return EOL;
       }

[ \t]+ ;

.     {printf("**%s**\n", yytext);
yyerror("Token nao reconhecido!\n");}

%%

int  yywrap() {
  return 1;
}
