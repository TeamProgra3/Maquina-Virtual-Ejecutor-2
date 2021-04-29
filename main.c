#include <stdio.h>
#include <stdlib.h>
#include "operaciones.h"
#include "string.h"
void magia();
void traduce(int var,int** opA,int** opB,int RAM[],int REG[]){
    int aux,topA,topB;

    aux = (var>>24)&0xFF;
    if (aux == 0xFF){    //SIN OPERANDO
        cod = (var>>20) & 0xFFF;

    }else{
        aux = aux>>4;
        if (aux == 0xF){           // UN SOLO OPERANDO
            cod = (var>>24) & 0xFF;
            topA = (var>>22) & 0x3;
            switch (topA){
            case 0x0 :    //TIPO INMEDIATO - UN SOLO OPERANDO
                **opA = var & 0xFFFF;
                **opA=**opA<<16;
                **opA=**opA>>16;
                break;
            case 0x1 :    //TIPO REGISTRO - UN SOLO OPERANDO
                **opA = var & 0xFFFF;
                *opA = &(REG[**opA]);
                break;

            case 0x2 :    //TIPO DIRECTO - UN SOLO OPERANDO
                **opA = var & 0xFFFF;
                *opA = &RAM[REG[DS]+**opA];
                break;
            }
        }else{                     // DOS OPERANDOS
            cod = var>>28 & 0xF;
            topA = (var>>26) & 0x3;
            topB = (var>>24) & 0x3;
            switch (topA){
            case 0x0 :         //opA: TIPO INMEDIATO
                **opA = (var>>12) & 0xFFF;
                **opA=**opA<<20;
                **opA=**opA>>20;
                break;

            case 0x1 :         //opA: TIPO REGISTRO
                **opA = (var >> 12) & 0x00FFF;
                *opA = &(REG[**opA]);
                break;

            case 0x2 :                      //opA: TIPO DIRECTO
                **opA = (var>>12) & 0x00FFF;
                *opA = &RAM[REG[DS]+**opA];
                break;
            }

            switch (topB){
            case 0x0 :                      //opB: TIPO INMEDIATO
                **opB = var & 0xFFF;
                **opB=**opB<<20;
                **opB=**opB>>20;
                break;

            case 0x1 :                      //opB: TIPO REGISTRO
                **opB = var  & 0x00FFF;
                 *opB = &(REG[**opB]);
                break;

            case 0x2 :                      //opB: TIPO INMEDIATO
                **opB = var & 0xFFF;
                *opB = &RAM[REG[DS]+**opB];
                break;
            }
        }
    }
}

void Ejecucion(int RAM[], int REG[]){
    int *opA,*opB,*auxA,*auxB;

    auxA = (int*)malloc(sizeof(int));
    auxB = (int*)malloc(sizeof(int));
    cargainstrucciones();
    while(REG[IP] >= 0 && REG[IP] < REG[DS]){  //IP >= 0 && IP < DS
        opA=auxA;opB=auxB;
        traduce(RAM[REG[IP]],&opA,&opB,RAM,REG);    //traduce(inst[IP])
        INST[cod](opA,opB,REG,RAM);
        REG[IP]++;                           //IP++
        if(sysactivado)
            muestra(opA,opB,REG,RAM);
    }
    free(auxA);free(auxB);
}
void leeArch(char nombreArch[50],int RAM[],int REG[]){
    int aux;FILE*arch;
    REG[DS] = 0;
    arch = fopen(nombreArch,"rb");
    if (arch != NULL){
        while (fread(&aux,sizeof(int),1,arch)==1){
            RAM[REG[DS]] = aux;
            (REG[DS])++;
        }
        fclose(arch);
        Ejecucion(RAM,REG);
    }else
        printf("ERROR: Archivo no encontrado");
}
void creaArch(){
    FILE*arch;
    arch = fopen("arch.bin","wb");
    int aux = 0xF000000F;
    fwrite(&aux,sizeof(int),1,arch);
    aux = 0x1800500A;
    fwrite(&aux,sizeof(int),1,arch);
    fclose(arch);
}

int main(int cantArg,char* argsMain[]){
    magia();
    int REG[16] = {0};//16 registros (IP, AC, AX,BX...etc)
    int RAM[8191];//8192 celdas de 4 bytes
    mnemonicos();
    flagb=flagc=flagd=0;
    if ((cantArg >=3 && !strcmp(argsMain[2],"-b")) || ((cantArg >=4 && !strcmp(argsMain[3],"-b"))) || (cantArg >=5 && !strcmp(argsMain[4],"-b")))
        flagb= 1;
    if ((cantArg >=3 && !strcmp(argsMain[2],"-c")) || ((cantArg >=4 && !strcmp(argsMain[3],"-c"))) || (cantArg >=5 && !strcmp(argsMain[4],"-c"))){
        flagc= 1;
        system("cls");
    }
    if ((cantArg >=3 && !strcmp(argsMain[2],"-d")) || ((cantArg >=4 && !strcmp(argsMain[3],"-d"))) || (cantArg >=5 && !strcmp(argsMain[4],"-d"))){
        flagd= 1;
    }
    if (cantArg >= 2) {
        leeArch(argsMain[1], RAM, REG);
    } else {
        printf("ERROR: Debe indicar el archivo a ejecutar");
        return -1;
    }
    return 0;
}



void magia(){
    printf("-----------------------VERSION 1.2------------------------\n");
}
