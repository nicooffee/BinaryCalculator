/*--------------------------------------------------------------------------------------------------------------------**/
/************************************************************************************************************************
    Autores        : NICOLAS IGNACIO HONORATO DROGUETT.
	Proposito      : Librería para tratar números binarios.
    Fecha          : Santiago de Chile, 24 de mayo de 2019
	Compilador C   : gcc (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0
************************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "binary.h"

/*******************************************************************************/
/**********************************OPERACIONES**********************************/
/**---- Funciones Operación:
 * 
 * Todas las funciones del bloque operaciones son las que se utilizan fuera de
 * la librería para tratar las variables struct Bin.
 * 
 * Cada una de estas funciones (a excepción del AND, OR y minus) dividen en
 * casos sus operaciones en base a si los binarios están en complemento 2.
 * 
 * Todas las funciones para tratar las string que están en binario 
 * (sum, sub, mult, divv) trabajan en base a números binarios positivos. Las
 * funciones de este bloque se encargan de cumplir esto.
 * 
 *----*/


/**
 * addBinary:
 * 
 * Función que recibe dos binarios y retorna el resultado de la suma de estos.
 * 
 * De ser alguno de los dos NULL, se retorna NULL.
 */
struct Bin *addBinary(struct Bin *b1,struct Bin *b2){
    char *b=NULL;
    struct Bin *res=NULL,*aux,*aux1,*aux2;
    if(b1==NULL || b2==NULL) return NULL;
    if(b1->num==NULL || b2->num==NULL) return NULL;
    if(b1->c2 ^ b2->c2){
        if(b1->c2){
            aux1 = minusBinary(b1);
            res = subBinary(b2,aux1);
            freeBinary(aux1);
        }
        else{
            aux2 = minusBinary(b2);
            res = subBinary(b1,aux2);
            freeBinary(aux2);
        }
    }
    else{
        if(b1->c2){
            aux1 = minusBinary(b1);
            aux2 = minusBinary(b2);
            b = sum(aux1->num,aux2->num);
            freeBinary(aux1);
            freeBinary(aux2);
            aux = createBinary(b);
            free(b);
            res = minusBinary(aux);
            freeBinary(aux);
        }
        else{
            b = sum(b1->num,b2->num);
            res = createBinary(b);
            free(b);
        }
    }
    return res;
}





/**
 * subBinary:
 * 
 * Función que recibe dos binarios y retorna el resultado de la resta de estos.
 * 
 * De ser alguno de los dos NULL, se retorna NULL.
 */
struct Bin *subBinary(struct Bin *b1,struct Bin *b2){
    int c;
    char *b;
    struct Bin *res=NULL,*aux,*aux1,*aux2;
    if(b1==NULL || b2==NULL) return NULL;
    if(b1->num==NULL || b2->num==NULL) return NULL;
    if(b1->c2 ^ b2->c2){
        if(b1->c2){
            aux1 = minusBinary(b1);
            b = sum(aux1->num,b2->num);
            freeBinary(aux1);
            aux = createBinary(b);
            res = minusBinary(aux);
            freeBinary(aux);
        }
        else{
            aux2 = minusBinary(b2);
            res = addBinary(b1,aux2);
            freeBinary(aux2);
        }
    }
    else{
        if(b1->c2){
            b = sub(b1->num,b2->num);
            aux = createBinary(b);
            aux->c2=1;
            res = copyBinary(aux);
            if(bincmp(b1->num,b2->num)>0){
                res = minusBinary(aux);
                res->c2 = 0;
            }
        }
        else{
            b = sub(b1->num,b2->num);
            aux = createBinary(b);
            if(bincmp(b1->num,b2->num)<0)
                aux->c2 = 1;
            res = copyBinary(aux);
        }
        free(aux);
        free(b);
    }
    return res;
}





/**
 * multBinary:
 * 
 * Función que recibe dos binarios y retorna el resultado de la 
 * mutliplicación de estos.
 * 
 * De ser alguno de los dos NULL, se retorna NULL.
 */
struct Bin *multBinary(struct Bin *b1,struct Bin *b2){
    char *b;
    struct Bin *res=NULL,*aux,*aux1,*aux2;
    if(b1==NULL || b2==NULL) return NULL;
    if(b1->num==NULL || b2->num==NULL) return NULL;
    if(b1->c2 ^ b2->c2){
        aux1 = b1->c2==1?minusBinary(b1):minusBinary(b2);
        aux2 = b1->c2==1?b2:b1;
        b = mult(aux1->num,aux2->num);
        freeBinary(aux1);
        aux = createBinary(b);
        free(b);
        res = minusBinary(aux);
        freeBinary(aux);
    }
    else{
        if(b1->c2){
            aux1 = minusBinary(b1);
            aux2 = minusBinary(b2);
            b = mult(aux1->num,aux2->num);
            res= createBinary(b);
            free(b);
        }
        else{
            b = mult(b1->num,b2->num);
            res = createBinary(b);
            free(b);
        }
    }
    return res;
}





/**
 * divBinary:
 * 
 * Función que recibe dos binarios y retorna el resultado de la 
 * división de estos.
 * 
 * De ser alguno de los dos NULL, se retorna NULL.
 */
struct Bin *divBinary(struct Bin *b1,struct Bin *b2){
    char *b,*r;
    struct Bin *res=NULL,*aux,*aux1,*aux2;
    if(b1==NULL || b2==NULL) return NULL;
    if(b1->num==NULL || b2->num==NULL) return NULL;
    if(bincmp(b2->num,"0\0")==0) return NULL;
    if(bincmp(b1->num,"0\0")==0) return createBinary("0\0");
    if(b1->c2 ^ b2->c2){
        aux1 = b1->c2==1?minusBinary(b1):minusBinary(b2);
        aux2 = b1->c2==1?b2:b1;
        b = divv(aux1->num,aux2->num,&r);
        freeBinary(aux1);
        aux = createBinary(b);
        free(b);
        res = minusBinary(aux);
        freeBinary(aux);
    }
    else{
        if(b1->c2){
            aux1 = minusBinary(b1);
            aux2 = minusBinary(b2);
            b = divv(aux1->num,aux2->num,&r);
            aux = createBinary(b);
            free(b);
            res = minusBinary(aux);
            freeBinary(aux);
        }
        else{
            b = divv(b1->num,b2->num,&r);
            res = createBinary(b);
            free(b);
        }
    }
    free(r);
    return res;
}





/**
 * modBinary:
 * 
 * Función que recibe dos binarios y retorna el resultado de aplicar
 * b1 modulo b2.
 * 
 * De ser alguno de los dos NULL, se retorna NULL.
 */
struct Bin *modBinary(struct Bin *b1,struct Bin *b2){
    char *b,*r;
    struct Bin *res=NULL,*aux,*aux1,*aux2;
    if(b1==NULL || b2==NULL) return NULL;
    if(b1->num==NULL || b2->num==NULL) return NULL;
    if(bincmp(b2->num,"0\0")==0) return NULL;
    if(bincmp(b1->num,"0\0")==0) return createBinary("0\0");
    if(b1->c2 ^ b2->c2){
        aux1 = b1->c2==1?minusBinary(b1):minusBinary(b2);
        aux2 = b1->c2==1?b2:b1;
        b = divv(aux1->num,aux2->num,&r);
        freeBinary(aux1);
        res = createBinary(r);
    }
    else{
        aux1 = b1->c2?minusBinary(b1):b1;
        aux2 = b1->c2?minusBinary(b2):b2;
        b = divv(aux1->num,aux2->num,&r);
        res = createBinary(r);
        if(b1->c2){
            freeBinary(aux1);
            freeBinary(aux2);
        }
    }
    free(b);
    return res;
}





/**
 * andBinary:
 * 
 * Función que recibe dos binarios y retorna el resultado de
 * aplicar la operación lógica AND a b1 y b2.
 * 
 * De ser alguno de los dos NULL, se retorna NULL.
 */
struct Bin *andBinary(struct Bin *b1,struct Bin *b2){
    char *b;
    struct Bin *res;
    if(b1==NULL || b2==NULL) return NULL;
    if(b1->num==NULL || b2->num==NULL) return NULL;
    b = andBin(b1->num,b2->num);
    res = createBinary(b);
    free(b);
    return res;
}





/**
 * orBinary:
 * 
 * Función que recibe dos binarios y retorna el resultado de
 * aplicar la operación lógica OR a b1 y b2.
 * 
 * De ser alguno de los dos NULL, se retorna NULL.
 */
struct Bin *orBinary(struct Bin *b1,struct Bin *b2){
    char *b;
    struct Bin *res;
    if(b1==NULL || b2==NULL) return NULL;
    if(b1->num==NULL || b2->num==NULL) return NULL;
    b = orBin(b1->num,b2->num);
    res = createBinary(b);
    free(b);
    return res;
}





/**
 * minusBinary:
 * 
 * Función que recibe un binario y retorna su negación aritmética
 * en complemento a 2.
 * 
 * Si el binario tiene valor NULL, se retorna NULL.
 */
struct Bin *minusBinary(struct Bin *b1){
    char *b,*aux;
    struct Bin *res;
    int n;
    if(b1==NULL || b1->num==NULL) return NULL;
    if(bincmp(b1->num,"0\0")==0) return copyBinary(b1);
    n = getMinPow(b1->num);
    aux = addPow2Bits(b1->num,n);
    b = twosC(aux);
    free(aux);
    res = createBinary(b);
    res->c2 = -(b1->c2-1);
    free(b);
    return res;
}


/********************************FIN OPERACIONES********************************/

/**********************************SUBFUNCIONES*********************************/
/**----
 * En este bloque se encuentran las funciones que manejan las string que están
 * dentro de los struct Bin.
 * 
 * Todas estas funciones carecen de información con respecto al signo del número,
 * es por esto que dentro de las funciones del bloque operaciones se comprueba si
 * el número está en complemento 2 o no.
 *----*/


/**
 * onesC:
 * 
 * Función que retorna el complemento a 1 de la string b.
 */
char *onesC(char *b){
    int i=0;
    char *bc=NULL;
    bc = (char*) calloc(strlen(b)+1,sizeof(char));
    for(;i<strlen(b);i++)
        bc[i] = b[i]=='0'?'1':'0';
    bc[i] = '\0';
    return bc;
}


/**
 * twosC:
 * 
 * Función que retorna el complemento a 2 de la string b.
 */
char *twosC(char *b){
    char *aux;
    char *bc;
    aux = onesC(b);
    bc = sum(aux,"01\0");
    free(aux);
    return bc;
}

/**
 * sum:
 * 
 * Función que retorna la string resultante de sumar b1 y b2.
 */
char *sum(char *b1,char *b2){
    char *buf = (char*) calloc(MAX_BUF,sizeof(char));
    char *res = NULL;
    int cb1,cb2;
    int r = 0;
    int i,j,k=0;
    for(i=strlen(b1)-1,j=strlen(b2)-1;i>=0 || j>=0 || r!=0;i--,j--){
        cb1 = i>=0? b1[i]-48:0;
        cb2 = j>=0? b2[j]-48:0;
        buf[k++] = (char) (cb1 ^ cb2 ^ r)+48;
        r = (cb1+cb2+r)>1?1:0; 
    }
    buf[k] = '\0';
    res = (char *) calloc(strlen(buf)+1,sizeof(char));
    res[k] = '\0';
    for (i=0;i<strlen(buf);i++) res[i] = buf[(strlen(buf)-1)-i];
    free(buf);
    return res;
}

/**
 * sub:
 * 
 * Función que retorna la string resultante de restar b1 y b2.
 * 
 * Para realizar esto, la función utiliza el complemento a 2
 * del binario b2 y luego lo suma a b1.
 * 
 * El resultado final es la resta de b1 y b2.
 * 
 */
char *sub(char *b1,char *b2){
    char *res,*aux,*auxB1,*auxB2,*auxB2c;
    int n;
    n = strlen(b1)>strlen(b2)?getMinPow(b1):getMinPow(b2);
    auxB1 = addPow2Bits(b1,n);
    auxB2 = addPow2Bits(b2,n);
    auxB2c = twosC(auxB2);
    free(auxB2);
    aux = sum(auxB1,auxB2c);
    free(auxB1);
    free(auxB2c);
    res = strlen(aux)>pow(2,n)?delLeftZeros(&aux[1]):delLeftZeros(aux);
    free(aux);
    return res;
}

/**
 * mult:
 * 
 * Función que retorna el resultado de multiplicar b1 y b2.
 * 
 * La función utiliza el método clásico de multiplicación (bit a bit). La
 * razón del por qué no se utiliza un método más eficiente como por
 * ejemplo, Karatsuba, es porque para implementarlo se requiere mucho
 * más espacio auxiliar en comparación con el método utilizado.
 */
char *mult(char *b1,char *b2){
    char *acum,*auxAcum,*mult;
    char m[MAX_BUF] = "\0";
    char *res;
    int i,j;
    acum = (char*) calloc(MAX_BUF,sizeof(char));
    strcpy(acum,"0\0");
    for(i=strlen(b2)-1;i>=0;i--){
        if(b2[i]!='0'){
            mult = (char*) calloc(MAX_BUF,sizeof(char));
            strcpy(mult,b1);
            strcat(mult,m);
            auxAcum = acum;
            acum = sum(acum,mult);
            free(auxAcum);
            free(mult);
        }
        strcat(m,"0\0");
    }
    auxAcum = delLeftZeros(acum);
    free(acum);
    res = (char *) calloc(strlen(auxAcum)+1,sizeof(char));
    strcpy(res,auxAcum);
    free(auxAcum);
    return res;
}




/**
 * divv:
 * 
 * Función que retorna el resultado de dividir b1 y b2.
 * 
 * La función realiza una división entera, donde el resultado se
 * guarda en la variable Q. El resto de la división se guarda en
 * el parámetro remainder. 
 */
char *divv(char *b1,char *b2, char **remainder){
    char Q[MAX_BUF];
    char R[MAX_BUF] = "0\0";
    char z[] = "0\0";
    char *aux;
    int i;
    if(bincmp("0\0",b2)==0) return NULL;
    Q[strlen(b1)] = '\0';
    for(i=0;i<strlen(b1);i++) Q[i] = '0';
    for(i=0;i<strlen(b1);i++){
        strcat(R,z);
        R[strlen(R)-1] = b1[i];
        if(bincmp(R,b2)>=0){
            aux = sub(R,b2);
            strcpy(R,aux);
            free(aux);
            Q[i] = '1';
        }
    }
    *remainder = (char *) calloc(strlen(R)+1,sizeof(char));
    strcpy(*remainder,R);
    return delLeftZeros(Q);
}




/**
 * bincmp:
 * 
 * Función que retorna el resultado de comparar los tamaños de
 * b1 y b2. 
 * Si b1<b2 -> bincmp(b1,b2)<0
 * Si b1>b2 -> bincmp(b1,b2)>0
 * Si b1=b2 -> bincmp(b1,b2)=0
 */
int bincmp(char *b1, char *b2){
    int i,j,m=1;
    int d=0;
    for(i=strlen(b1)-1,j=strlen(b2)-1;i>=0 || j>=0;i--,j--,m*=2)
        d += ((i>=0?b1[i]-48:0)*m)-((j>=0?b2[j]-48:0)*m);
    return d;
}




/**
 * getMinPow:
 * 
 * La siguiente función retorna la cantidad de bits mínima en potencia
 * de 2 que requiere b para ser representado.
 * 
 * Si b: 11011 entonces la cantidad mínima es 2^3 = 8. Se retorna el
 * valor 3.
 */
int getMinPow(char *b){
    int l;
    int n = 1;
    if(b == NULL) return -1;
    l = strlen(b);
    while(l > pow(2,n))
        n+=1;
    return n;
}




/**
 * addPow2Bits:
 * 
 * Función que retorna una string equivalente a b, con 2^n-len(b) bits
 * añadidos a la izquierda.
 * 
 * Si b: 10111 y n=3 entonces la string retornada es 00010111.
 */
char *addPow2Bits(char *b,int n){
    int L = pow(2,n);
    int Lb = strlen(b);
    char *nb = (char *) calloc(pow(2,n)+1,sizeof(char));
    int i=0;
    for(;i<pow(2,n);i++)
        nb[i] = i>=(L-Lb)?b[i-(L-Lb)]:'0';
    nb[i] = '\0';
    return nb;
}




/**
 * delLeftZeros:
 * 
 * Función que retorna el equivalente al parámetro b, sin los posibles
 * ceros que posea a la izquierda.
 * 
 * Si b = 00101, entonces la string retornada es 101.
 */
char *delLeftZeros(char *b){
    char *z = (char*) calloc(2,sizeof(char));
    char *aux = strchr(b,'1');
    char *r =NULL;
    strcpy(z,"0\0");
    if (aux==NULL) return z;
    free(z);
    r = (char *) calloc(strlen(aux)+1,sizeof(char));
    strcpy(r,aux);
    return r;
}



/**
 * andBin:
 * 
 * Función que retorna el resultado de aplicar la operación AND a los
 * parámetros b1 y b2. La operación se realiza bit a bit.
 */
char *andBin(char *b1,char *b2){
    char *b;
    int len;
    int i,j,k;
    len = strlen(b1)>strlen(b2)?strlen(b1):strlen(b2);
    b = (char *) calloc(len+1,sizeof(char));
    b[len] = '\0';
    for(i=strlen(b1)-1,j=strlen(b2)-1,k=len-1;k>=0;i--,j--,k--)
        b[k] = (char)((i>=0?b1[i]-48:0)&(j>=0?b2[j]-48:0))+48;
    return b;
}




/**
 * orBin:
 * 
 * Función que retorna el resultado de aplicar la operación OR a los
 * parámetros b1 y b2. La operación se realiza bit a bit.
 */
char *orBin(char *b1,char *b2){
    char *b;
    int len;
    int i,j,k;
    len = strlen(b1)>strlen(b2)?strlen(b1):strlen(b2);
    b = (char *) calloc(len+1,sizeof(char));
    b[len] = '\0';
    for(i=strlen(b1)-1,j=strlen(b2)-1,k=len-1;k>=0;i--,j--,k--)
        b[k] = (char)((i>=0?b1[i]-48:0)|(j>=0?b2[j]-48:0))+48;
    return b;
}



/********************************FIN SUBFUNCIONES*******************************/
/*******************************************************************************/
/******************************CREACION Y EXTRAS********************************/
/**
 * isBinary:
 * 
 * Función que comprueba si la string c es equivalente a una expresión binaria.
 * Esto es, si c pertenece a [01]+
 */
int isBinary(char *c){
    int i=0;
    for(;i<strlen(c);i++)
        if(c[i]!='0' && c[i]!='1')
            return 0;
    return 1;
}




/**
 * createBinary:
 * 
 * Función que crea un struct Bin a partir de la string c.
 * 
 * La función comprueba si c es un binario. Se asume que la string c no está
 * escrita en complemento 2.
 */
struct Bin *createBinary(char *c){
    struct Bin *bin=NULL;
    if(c==NULL || !isBinary(c))
        return NULL;
    bin = (struct Bin *) calloc(1,sizeof(struct Bin));
    bin->num = (char*) calloc(strlen(c)+1,sizeof(char));
    bin->c2 = 0;
    strcpy(bin->num,c);
    return bin;
}



/**
 * copyBinary:
 * 
 * Función que retorna una copia del parámetro b.
 */
struct Bin *copyBinary(struct Bin *b){
    struct Bin *bcpy=NULL;
    if(b==NULL) return NULL;
    bcpy = (struct Bin *) calloc(1,sizeof(struct Bin));
    bcpy->num = (char*) calloc(strlen(b->num)+1,sizeof(char));
    bcpy->c2 = b->c2;
    strcpy(bcpy->num,b->num);
    return bcpy;
}



/**
 * formatBin:
 * 
 * Función que retorna un formato para el binario enviado
 * por parámetro.
 * 
 * Si 1011 no está en complemento 2 se retorna: +1011
 * Si 1011 está en complemento 2 se retorna: -1011
 */
char *formatBin(struct Bin *bin){
    struct Bin *aux;
    char buf[MAX_BUF];
    char *sf=NULL;
    char *c=NULL;
    int i = 0, f = 0;
    if(bin==NULL || bin->num==NULL) strcpy(buf,"(null)\0");
    else{
        buf[0] = bin->c2?'-':'+';
        aux = bin->c2?minusBinary(bin):bin;
        c = strchr(aux->num,'1');
        strcpy(&buf[1],c==NULL?aux->num:c);
        if(bin->c2) freeBinary(aux);
    }
    sf = (char *) calloc(strlen(buf)+1,sizeof(char));
    strcpy(sf,buf);
    return sf;
}



/**
 * freeBinary:
 * 
 * Función que libera el espacio utilizado por el parámetro b. 
 */
void freeBinary(struct Bin *b){
    if (b==NULL) return;
    free(b->num);
    if (b->num == NULL) return;
    free(b);
}


/****************************FIN CREACION Y EXTRAS******************************/
/*******************************************************************************/
