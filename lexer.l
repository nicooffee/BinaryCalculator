%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <math.h>
    #include "binary.h"
    #include "y.tab.h"
    extern YYSTYPE yylval;
%}

%%
("EXIT"|"exit")/.*      {return EXIT;}
"DEL".*      {return DELETE;}
[0-9]+      {
    if(!isBinary(yytext)) return NOBINERROR;
    yylval.bin = createBinary(yytext); 
    return NUMERO;
}
([a-zA-Z][0-9]*)+   {
    strcpy(yylval.buf,yytext); 
    return VARIABLE;
}
([a-zA-Z]+" "+[a-zA-Z]+)+ |
([0-9]" "*[a-zA-Z])+ |
[+][*/%|&=]+ |
[-][*/%|&=]+ |
[=][*/%|&=]+       return SINTAXERROR;
[-+*/%|&=]         return *yytext;
[\r\n]            {return *yytext;}
[ \t]             ;
[()]              {return *yytext;}
.                return INVCHARERROR;

%% 
 