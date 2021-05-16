#include <stdio.h>
#include <stdlib.h>
#include "operaciones.h"
#include "string.h"
void magia();
void traduce(int var,int** opA,int** opB,int RAM[],int REG[]){
    int celdafinal,segmento,registro,offset,DSL,aux,topA,topB;

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
                DSL = REG[DS] & 0xFF;
                *opA = &RAM[DSL+**opA];
                break;
            case 0x3 :    //Indirecto
                offset = (var>>4) & 0xFF;
                registro = var & 0xF;
                //Tenemos el registro pedido y el offset, ahora hay que sacar la parte alta de 
                //ese registro, que me indica respecto a que segmento de la memoria se trabaja
                segmento = (REG[registro] >> 16) & 0xFFFF;
                //La parte alta de segmento indica el tamanio asignado a ese segmento
                //La parte baja contiene la celda donde comienza ese segmento
                
                //Luego la celda a devolver es:
                            //parte baja segmento + contenido registro + offset
                celdafinal = (REG[segmento] & 0xFFFF) + REG[registro] + offset;
                if (celdafinal  < (REG[segmento]>>16) & 0xFFFF )
                    *opA = &RAM[celdafinal];
                else
                    **opA = -1;
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
                DSL = REG[DS] & 0xFF;
                *opA = &RAM[DSL+**opA];
                break;
            case 0x3 :    //Indirecto
                offset = (var>>4) & 0xFF;
                registro = var & 0xF;
                //Tenemos el registro pedido y el offset, ahora hay que sacar la parte alta de 
                //ese registro, que me indica respecto a que segmento de la memoria se trabaja
                segmento = (REG[registro] >> 16) & 0xFFFF;
                //La parte alta de segmento indica el tamanio asignado a ese segmento
                //La parte baja contiene la celda donde comienza ese segmento
                
                //Luego la celda a devolver es:
                            //parte baja segmento + contenido registro + offset
                celdafinal = (REG[segmento] & 0xFFFF) + REG[registro] + offset;
                if (celdafinal  < (REG[segmento]>>16) & 0xFFFF )
                    *opA = &RAM[celdafinal];
                else
                    **opA = -1;
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
                DSL = REG[DS] & 0xFF;
                *opB = &RAM[DSL+**opB];
                break;
            case 0x3 :    //Indirecto
                offset = (var>>4) & 0xFF;
                registro = var & 0xF;
                //Tenemos el registro pedido y el offset, ahora hay que sacar la parte alta de 
                //ese registro, que me indica respecto a que segmento de la memoria se trabaja
                segmento = (REG[registro] >> 16) & 0xFFFF;
                //La parte alta de segmento indica el tamanio asignado a ese segmento
                //La parte baja contiene la celda donde comienza ese segmento
                
                //Luego la celda a devolver es:
                            //parte baja segmento + contenido registro + offset
                celdafinal = (REG[segmento] & 0xFFFF) + REG[registro] + offset;
                if (celdafinal  < (REG[segmento]>>16) & 0xFFFF )
                    *opB = &RAM[celdafinal];
                else
                    **opB = -1;
                break;
            }
        }
    }
}

void Ejecucion(int RAM[], int REG[]){
    int *opA,*opB,*auxA,*auxB;
    int CSH,DSH;
    auxA = (int*)malloc(sizeof(int));
    auxB = (int*)malloc(sizeof(int));
    cargainstrucciones();
    DSH = (REG[DS] >> 16) & 0xFFFF;
    CSH = (REG[CS] >> 16) & 0xFFFF;
    while(REG[IP] >= 0 && REG[IP] < CSH){  //IP >= 0 && IP < DS
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
    int cont,aux;FILE*arch;
    int tamanoDS, tamanoES, tamanoSS,tamanoCS;
    REG[DS] = 0;
    arch = fopen(nombreArch,"rb");
    //arch = fopen("C:/Users/Augusto/Documents/Facultad/Arquitectura/MaquinaVirtual/Maquina-Virtual-Ejecutor-2/prueba.bin", "rb");
    if (arch != NULL){
        fread(&aux, sizeof(int), 1, arch);
        if (aux == 0x4D563231){
            fread(&tamanoDS, sizeof(int), 1, arch);
            fread(&tamanoES, sizeof(int), 1, arch);
            fread(&tamanoSS, sizeof(int), 1, arch);
            fread(&tamanoCS, sizeof(int), 1, arch);

            if (tamanoES+tamanoCS+tamanoDS+tamanoSS<=8192){
                REG[CS] = (tamanoCS) << 16;  //
                REG[DS] = (tamanoDS << 16) | (tamanoCS); 
                REG[ES] = (tamanoES << 16) | (tamanoCS+tamanoDS);
                REG[SS] = (tamanoSS << 16) | (tamanoES+tamanoCS+tamanoDS);
                cont = 0;
                while (fread(&aux,sizeof(int),1,arch)==1){ //Carga instrucciones y constantes string en CS
                    RAM[cont] = aux;
                    cont++;
                }
                if(cont<=tamanoCS)
                    Ejecucion(RAM, REG);
                else
                    printf("OVERFLOW [CS]: La cantidad de instrucciones supera las establecidas como maximo en el header\n");
                }
        }
        else 
            printf("La cabecera del archivo no es valida (Se esperaba MV21)\n");
        fclose(arch);
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
    REG[4] = -1;
    int RAM[8192];//8192 celdas de 4 bytes
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
    printf("--------------------------------VERSION 2.1---------------------------------\n");
    printf("-----Jamon: Lee header y se cargan los registros DS,EX,SS,CS------\n");
    printf("--Jamon: Operadores indirectos funcionando, falta testeo todavia no olvidar--\n");
    printf("---------------Pendiente: Memoria dinamica // Erorres de la pila--------------\n");
    printf("----Pendiente: Codificar nuevas instrucciones // Errores acceso de memoria----\n");
    printf("-----------------------------------------------------------------------------\n\n");
}
