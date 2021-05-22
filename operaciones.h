#include <stdio.h>
#include <stdlib.h>
#define DS 0
#define SS 1
#define ES 2
#define CS 3
#define HP 4
#define IP 5
#define SP 6
#define BP 7
#define CC 8
#define AC 9
#define AX 10
#define BX 11
#define CX 12
#define DX 13

int sysactivado;
int cod;
int flagb;
int flagc;
int flagd;
char *MNEM[4096];
void mnemonicos();
void (*INST[4082])(int *, int *,int [],int[]);
void cargainstrucciones();
void cargarCC(int *a,int REG[]);
void muestra(int *a,int *b,int REG[],int RAM[]);
void barrab(int RAM[],int REG[]);
void RecuperaString(int cod, char salida[50]);
int anytoint(char *s, char **out);
void cargaHL(int *a,int H,int L );

//Instrucciones con dos operandos

void MOV(int *a,int *b,int REG[],int RAM[]);
void ADD(int *a,int *b,int REG[],int RAM[]);
void SUB(int *a,int *b,int REG[],int RAM[]);
void MUL(int *a,int *b,int REG[],int RAM[]);
void DIV(int *a,int *b,int REG[],int RAM[]);
void SWAP(int *a,int *b,int REG[],int RAM[]);
void CMP(int *a,int *b,int REG[],int RAM[]);
void AND(int *a,int *b,int REG[],int RAM[]);
void OR(int *a,int *b,int REG[],int RAM[]);
void XOR(int *a,int *b,int REG[],int RAM[]);
void SLEN(int *a,int *b,int REG[],int RAM[]);
void SMOV(int *a,int *b,int REG[],int RAM[]);
void SCMP(int *a,int *b,int REG[],int RAM[]);
void SHL(int *a,int *b,int REG[],int RAM[]);
void SHR(int *a,int *b,int REG[],int RAM[]);

//Instrucciones con un operando

void SYS(int *a,int *b,int REG[],int RAM[]);
void JMP(int *a,int *b,int REG[],int RAM[]);
void JZ(int *a,int *b,int REG[],int RAM[]);
void JP(int *a,int *b,int REG[],int RAM[]);
void JN(int *a,int *b,int REG[],int RAM[]);
void JNZ(int *a,int *b,int REG[],int RAM[]);
void JNP(int *a,int *b,int REG[],int RAM[]);
void JNN(int *a,int *b,int REG[],int RAM[]);
void LDH(int *a,int *b,int REG[],int RAM[]);
void LDL(int *a,int *b,int REG[],int RAM[]);
void RND(int *a,int *b,int REG[],int RAM[]);
void NOT(int *a,int *b,int REG[],int RAM[]);
void PUSH(int *a,int *b,int REG[],int RAM[]);
void POP(int *a,int *b,int REG[],int RAM[]);
void CALL(int *a,int *b,int REG[],int RAM[]);

//Instruccion sin operando
void RET(int *a,int *b,int REG[],int RAM[]);
void STOP(int *a,int *b,int REG[],int RAM[]);
