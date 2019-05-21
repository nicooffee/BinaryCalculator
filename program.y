/*--------------------------------------------------------------------------------------------------------------------**/
/************************************************************************************************************************
    Autores        : NICOLAS IGNACIO HONORATO DROGUETT.
	Proposito      : Laboratorio 4 fundamentos de ciencia de la computación I.
    Programa       : Calculadora de números binarios.
    Fecha          : Santiago de Chile, 24 de mayo de 2019
    Compilador yacc: yacc - 1.9 20140715
    Compialdor lex : lex 2.6.0
	Compilador C   : gcc (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0
************************************************************************************************************************/



/**
 * Definiciones globales:
 * 
 * El programa utiliza dos librerías externas, y.tab.h, que
 * corresponde a la que utiliza el analizador léxico para
 * comunicarse con yacc, y binary.h, que es la que contiene las
 * funciones para manipular los números binarios.
 * 
 * El programa utiliza un struct Variable y un arreglo de estos
 * para almacenar las variables que cree que el usuario por medio
 * de la calculadora. Se definen cuatro funciones para manipular
 * esta estructura.
 * 
 * Se definen variables globales para el manejo de la calculadora,
 * gramática y errores.
 */
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>
    #include "binary.h"
    #include "y.tab.h"
    #define MAX_VARIABLE 100
    #define MAX_VARNAME 20
    #define MAX_SERROR 100
    #define LINE 50
    struct Variable{
        char *key;
        struct Bin *value;
    };
    struct Variable *buscarVariable(char *key);
    int insertarVariable(char *key, struct Bin *value);
    void printVariables();
    void freeVariable(struct Variable **v);

    int yylex();
    int yywrap();
    void yyerror (char *s);
    int error=0;
    int exe=1;
    int del=0;
    struct Variable **variable;
    int cantVariable = 0;
    char minusLine[LINE] = "|------------------------------------------|\0";
    char astLine[LINE] = "|****************************************|\0";
    char titulo[LINE] = "Calculadora Binaria\0";
    char sError[MAX_SERROR];
    char noBinError[MAX_SERROR] = "El numero no es binario.\0";
    char invCharError[MAX_SERROR] = "Caracter no valido.\0";
    char varNotDef[MAX_SERROR] = " :Variable no definida.\0";
    char opInvalida[MAX_SERROR] = " :Operacion invalida.\0";
    char syntaxError[MAX_SERROR] = "Syntax error.\0";
    char divCero[MAX_SERROR] = "Division por 0.\0";
%}
/**
 * Se define el tipo de yylval, que utiliza el analizador sintáctico
 * para enviar valores. 
 */
%union{
    char buf[20];
    struct Bin *bin;
}
//Regla inicial de la gramática.
%start program

/**
 * Se definen siete tokens con los que el analizador sintáctico
 * indicará a yacc qué regla se debe seguir al descomponer el
 * arbol sintáctico que genera la cadena ingresada.
 */
%token NUMERO VARIABLE DELETE EXIT INVCHARERROR NOBINERROR SINTAXERROR


//Prioridad de los operadores.
%right '='
%left '+' '-'
%left '*' '/' '%'
%left '|'
%left '&'
%left '('
%%
program:   
    program exp '\n' {
        char *b = formatBin($2.bin);
        printf("\033[6;0f\033[2K| R : %s%*c| |",b==NULL?"ERROR\0":b,(int) (strlen(astLine)-strlen(b)-7),' ');
        if(cantVariable>0)
            printVariables();
        printf("\033[5;0f\033[2K\r| Op :%*c| |\033[5;8f",(int)(strlen(astLine)-7),' ');
        free(b);
    }
    | program '\n' {
        printf("\033[6;0f|%*c| |\n",(int)(strlen(astLine)-2),' ');
        printf("\033[5;0f\033[2K\r| Op :%*c| |\033[5;8f",(int)(strlen(astLine)-7),' ');
    }
    | program DELETE '\n'{
        int i=0;
        printf("\033[H\r");
        for(i=0;i<cantVariable+12;i++)
            printf("\033[2K\033[1B\033[2K");
        for(i=0;i<cantVariable;i++)
            freeVariable(&variable[i]);
        cantVariable=0;
        error=0;
        del = 1;
        YYACCEPT;
    }
    | EXIT '\n' {exe=0;YYABORT;}
    | program error '\n'{
        error =1;
        YYACCEPT;
    }
    |
    ;

exp: 
    exp '+' exp   {
        $$.bin = addBinary($1.bin,$3.bin);
        freeBinary($1.bin);
        freeBinary($3.bin);
    }
    | exp '-' exp   {
        $$.bin = subBinary($1.bin,$3.bin);
        freeBinary($1.bin);
        freeBinary($3.bin);
    }
    | exp '*' exp   {
        $$.bin = multBinary($1.bin,$3.bin);
        freeBinary($1.bin);
        freeBinary($3.bin);
    }
    | exp '/' exp   {
        $$.bin = divBinary($1.bin,$3.bin);
        freeBinary($1.bin);
        freeBinary($3.bin);
        if($$.bin == NULL){
            strcpy(sError,divCero);
            YYERROR;
        }
    }
    | exp '%' exp   {
        $$.bin = modBinary($1.bin,$3.bin);
        freeBinary($1.bin);
        freeBinary($3.bin);
        if($$.bin == NULL){
            strcpy(sError,divCero);
            YYERROR;
        }
    }
    | exp '|' exp   {
        $$.bin = orBinary($1.bin,$3.bin);
        freeBinary($1.bin);
        freeBinary($3.bin);
    }
    | exp '&' exp   {
        $$.bin = andBinary($1.bin,$3.bin);
        freeBinary($1.bin);
        freeBinary($3.bin);
    }
    | VARIABLE '=' exp {
        if($3.bin !=NULL)
            insertarVariable($1.buf,$3.bin);
        $$.bin = copyBinary($3.bin);
    }
    | '(' exp ')'   {$$.bin = copyBinary($2.bin); freeBinary($2.bin);}
    | VARIABLE      {
        struct Variable *v=buscarVariable($1.buf);
        $$.bin = v==NULL?NULL:copyBinary(v->value);
        if ($$.bin ==NULL){
            strcpy(sError,$1.buf);
            strcat(sError,varNotDef);
            YYERROR;
        }
    }
    | '+' exp {$$.bin = $2.bin;}
    | '-' exp       {
        $$.bin = minusBinary($2.bin);
        freeBinary($2.bin);
    }
    | NUMERO      {$$ = $1;}
    | '*' exp {strcpy(sError,syntaxError); YYERROR;}
    | '/' exp {strcpy(sError,syntaxError); YYERROR;}
    | '%' exp {strcpy(sError,syntaxError); YYERROR;}
    | '&' exp {strcpy(sError,syntaxError); YYERROR;}
    | '|' exp {strcpy(sError,syntaxError); YYERROR;}
    | '-' {strcpy(sError,syntaxError); YYERROR;}
    | '+' {strcpy(sError,syntaxError); YYERROR;}
    | '*' {strcpy(sError,syntaxError); YYERROR;}
    | '/' {strcpy(sError,syntaxError); YYERROR;}
    | '%' {strcpy(sError,syntaxError); YYERROR;}
    | '&' {strcpy(sError,syntaxError); YYERROR;}
    | '|' {strcpy(sError,syntaxError); YYERROR;}
    | NUMERO '=' exp {strcpy(sError,syntaxError); YYERROR;}
    | INVCHARERROR {strcpy(sError,invCharError); YYERROR;}
    | SINTAXERROR {strcpy(sError,syntaxError); YYERROR;}
    | NOBINERROR {strcpy(sError,noBinError); YYERROR;}
    ;
%%



/**
 * main: 
 * 
 * En la función main se muestra la estructura de la calculadora.
 * 
 * El programa se ejecuta mientras la variable exe sea igual a 1. Si
 * lex retorna el token EXIT, esta variable se hace 0.
 * 
 * Antes de finalizar cada instancia de la calculadora, se libera
 * la memoria utilizada por las variables que el usuario almacene
 */

 // home: 0,0
 // cursor lect: 5,8
 // cursor var: 10,0
int main(){
    int i = 0;
    variable = (struct Variable**) calloc(MAX_VARIABLE,sizeof(struct Variable*));
    cantVariable=0;
    system("clear");
    printf("\033c\033[0;0H");
    while(exe){
        printf("\033[H\r");
        printf("%s\n",minusLine);
        printf("| %s %*c| |\n",titulo,(int)(strlen(astLine)-strlen(titulo)-4),' ');
        printf("| Operaciones:");printf("\033[32;1m + - * / %% & | -(n)%*c\033[0m| |\n",(int)(strlen(minusLine)-36),' ');
        printf("%s |\n",astLine);
        printf("| Op : %*c| |\n",(int)(strlen(astLine)-8),' ');
        if(!error || del)printf("|%*c| |\n",(int)(strlen(astLine)-2),' ');
        else printf("\033[2K| \033[31;1m %s\033[0m%*c| |\n",sError,(int)(strlen(astLine)-strlen(sError)-4),' ');
        printf("|%*c| |\n",(int)(strlen(astLine)-2),' ');
        printf("|%*c| |\n",(int)(strlen(astLine)-2),' ');
        printf("%s |\n",astLine); 
        if(cantVariable==0) printf("%s",minusLine);
        else printf("| Variables:%*c|",(int)(strlen(minusLine)-13),' ');
        printf("\033[5;8f");
        if(del) del=0;
        yyparse();
        if(!error){
            for(i=0;i<cantVariable;i++)
                freeVariable(&variable[i]);
            cantVariable=0;
        }
    }
    system("clear");
    free(variable);
    return 0;
}




/**
 * printVariables:
 * 
 * Esta función muestra el listado de variables según el formato
 * de la calculadora.
 */
void printVariables(){
    int i=0;
    char *b;
    printf("\033[10;0f\033[2K\r| Variables:%*c|\n",(int)(strlen(minusLine)-13),' ');
    for(i=0;i<cantVariable;i++){
        b = formatBin(variable[i]->value);
        printf("\033[2K\r| \033[35;1m%s\033[0m = %s%*c|\n",\
            variable[i]->key,\
            b,\
            (int) (strlen(minusLine)-strlen(variable[i]->key)-strlen(b)-6),\
            ' ');
        free(b);
    }
    printf("%s",minusLine);
}




/**
 * buscarVariable:
 * 
 * Función que recorre el arreglo de variables y busca a la
 * variable que tenga el nombre key. De no encontrarse se
 * retorna NULL
 */
struct Variable *buscarVariable(char *key){
    int i = 0;
    for(;i<cantVariable;i++)
        if(variable[i]!=NULL && strcmp(variable[i]->key,key) == 0)
            return variable[i];
    return NULL;
}





/**
 * insertarVariable:
 * 
 * Función que crea una variable en base a key y value para luego
 * almacenarla en el arreglo de variables. De existir anteriormente
 * la variable, se reemplaza el valor anterior por el contenido en
 * el parámetro value.
 */
int insertarVariable(char *key, struct Bin *value){
    struct Variable *v=NULL;
    int i=0,r=0;
    if (MAX_VARIABLE == cantVariable) return -1;
    v = (struct Variable *) calloc(1,sizeof(struct Variable));
    v->key = (char*) calloc(strlen(key)+1,sizeof(char));
    strcpy(v->key,key);
    v->value=value;
    for(i=0;i<cantVariable;i++){
        if(r==0 && strcmp(variable[i]->key,key)==0){
            freeBinary(variable[i]->value);
            free(variable[i]->key);
            free(variable[i]);
            variable[i] = v;
            r=1;
        }
    }
    if(r==0){
        variable[cantVariable] = v;
        cantVariable++;
    }
    return 1;
}





/**
 * freeVariable:
 * 
 * Función que libera el espacio utilizado por el parámetro v.
 */
void freeVariable(struct Variable **v){
    if (*v == NULL) return;
    free((*v)->key);
    freeBinary((*v)->value);
    free((*v));
    (*v) = NULL;
    return;
}






int yywrap(){
    return 1;
} 
void yyerror (char *s){
   fprintf (stderr, "%s\n", s);
}