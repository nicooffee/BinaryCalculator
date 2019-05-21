#define MAX_BUF 1000
/**
 * struct Bin:
 * 
 * num = [01]+
 * 
 * Se asume que la string enviada a createBinary
 * tiene el signo en la primera posición (para
 * complemento 2)
 * 
 */

struct Bin{
    char *num;
    int c2;
};

struct Bin *addBinary(struct Bin *b1,struct Bin *b2);
char *sum(char *b1,char *b2);

struct Bin *subBinary(struct Bin *b1,struct Bin *b2);
char *sub(char *b1,char *b2);

struct Bin *multBinary(struct Bin *b1,struct Bin *b2);
char *mult(char *b1,char *b2);

struct Bin *divBinary(struct Bin *b1,struct Bin *b2);
struct Bin *modBinary(struct Bin *b1,struct Bin *b2);
char *divv(char *b1,char *b2, char **remainder);
char *addPow2Bits(char *b,int n);
char *delLeftZeros(char *b);
int getMinPow(char *b);
int bincmp(char *b1, char *b2);

struct Bin *andBinary(struct Bin *b1,struct Bin *b2);
char *andBin(char *b1,char *b2);

struct Bin *orBinary(struct Bin *b1,struct Bin *b2);
char *orBin(char *b1,char *b2);

struct Bin *minusBinary(struct Bin *b1);


char *onesC(char *b);
char *twosC(char *b);


int isBinary(char *c);
struct Bin *createBinary(char *c);
struct Bin *copyBinary(struct Bin *b);
char *formatBin(struct Bin *bin);
void freeBinary(struct Bin *b);