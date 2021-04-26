#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<windows.h>
#include "operaciones.h"

//Si hay tiempo estaria piola meter ambos vectores en uno solo
void cargainstrucciones() {
    INST[0x0]=MOV;
    INST[0x1]=ADD;
    INST[0x2]=SUB;
    INST[0x3]=SWAP;
    INST[0x4]=MUL;
    INST[0x5]=DIV;
    INST[0x6]=CMP;
    INST[0x7]=SHL;
    INST[0x8]=SHR;
    INST[0x9]=AND;
    INST[0xA]=OR;
    INST[0xB]=XOR;
    INST[0xF0]=SYS;
    INST[0xF1]=JMP;
    INST[0xF2]=JZ;
    INST[0xF3]=JP;
    INST[0xF4]=JN;
    INST[0xF5]=JNZ;
    INST[0xF6]=JNP;
    INST[0xF7]=JNN;
    INST[0xF8]=LDL;
    INST[0xF9]=LDH;
    INST[0xFA]=RND;
    INST[0xFB]=NOT;
    INST[0xFF1]=STOP;
}
void mnemonicos() {
    MNEM[0x0]="MOV";
    MNEM[0x1]="ADD";
    MNEM[0x2]="SUB";
    MNEM[0x3]="SWAP";
    MNEM[0x4]="MUL";
    MNEM[0x5]="DIV";
    MNEM[0x6]="CMP";
    MNEM[0x7]="SHL";
    MNEM[0x8]="SHR";
    MNEM[0x9]="AND";
    MNEM[0xA]="OR";
    MNEM[0xB]="XOR";
    MNEM[0xF0]="SYS";
    MNEM[0xF1]="JMP";
    MNEM[0xF2]="JZ";
    MNEM[0xF3]="JP";
    MNEM[0xF4]="JN";
    MNEM[0xF5]="JNZ";
    MNEM[0xF6]="JNP";
    MNEM[0xF7]="JNN";
    MNEM[0xF8]="LDL";
    MNEM[0xF9]="LDH";
    MNEM[0xFA]="RND";
    MNEM[0xFB]="NOT";
    MNEM[0xFF1]="STOP";
}


void cargarCC(int *a,int REG[]) {
    if(*a==0)
        REG[CC]=0x00000001;//0b00000000000000000000000000000001
    else if(*a<0)
        REG[CC]=0x80000000;//0b10000000000000000000000000000000
    else
        REG[CC]=0x0;//0b00000000000000000000000000000000
}

void MOV(int *a,int *b,int REG[],int RAM[]) {
    *a=*b;
}

void ADD(int *a,int *b,int REG[],int RAM[]) {
    *a+=*b;
    cargarCC(a,REG);
}

void SUB(int *a,int *b,int REG[],int RAM[]) {
    *a-=*b;
    cargarCC(a,REG);
}

void MUL(int *a,int *b,int REG[],int RAM[]) {
    *a*=*b;
    cargarCC(a,REG);
}

void DIV(int *a,int *b,int REG[],int RAM[]) {
    REG[AC]=*a%*b;
    *a/=(int)*b;
    cargarCC(a,REG);
}
void SWAP(int *a,int *b,int REG[],int RAM[]) {
    int aux=*a;
    *a=*b;
    *b=aux;
}

void CMP(int *a,int *b,int REG[],int RAM[]) {
    int aux=0;
    aux=*a-*b;
    cargarCC(&aux,REG);
}
void AND(int *a,int *b,int REG[],int RAM[]) {
    *a=(*a)&(*b);
    cargarCC(a,REG);
}
void OR(int *a,int *b,int REG[],int RAM[]) {
    *a=(*a)|(*b);
    cargarCC(a,REG);
}
void XOR(int *a,int *b,int REG[],int RAM[]) {
    *a=(*a)^(*b);
    cargarCC(a,REG);
}
void SHL(int *a,int *b,int REG[],int RAM[]) {
    *a=*a<<*b;
    cargarCC(a,REG);
}
void SHR(int *a,int *b,int REG[],int RAM[]) {
    *a=*a>>*b;
    cargarCC(a,REG);
}

void RecuperaString(int cod, char salida[50]) {
    int codaux, op1, op2, tipoOp1, tipoOp2, existeOp2 = 0;
    char aux1[50], aux2[50];
    codaux = ((cod >> 24) & 0xFF);
    if (codaux != 0xFF) {
        if (((cod >> 28) & 0xF)  == 0xF) {  //Un solo operando
            codaux = ((cod >> 24) & 0xFF);
            strcpy(salida, MNEM[codaux]);
            tipoOp1 = (cod >> 22) & 0x3;
            op1 = cod & 0xFFF;
            op1 = op1 << 16;
            op1 = op1 >> 16;

        } else {  //Dos operandos
            codaux = ((cod >> 28) & 0xF);
            strcpy(salida, MNEM[codaux]);
            existeOp2 = 1;
            tipoOp1 = (cod >> 26) & 0x3;
            tipoOp2 = (cod >> 24) & 0x3;
            op1 = (cod >> 12) & 0xFFF;
            op1 = op1 << 20;
            op1 = op1 >> 20;

            op2 = cod & 0xFFF;
            op2 = op2 << 20;
            op2 = op2 >> 20;

            if (tipoOp2 == 2)
                sprintf(aux2, " [%d] ", op2);
            else if (tipoOp2 == 1)
                if (op2 != 9)
                    sprintf(aux2, " %cX", op2 + 55);
                else
                    sprintf(aux2, " AC");
            else
                sprintf(aux2, " %d ", op2);
        }
        if (tipoOp1 == 2)
            sprintf(aux1, " [%d]", op1);
        else if (tipoOp1 == 1)
            if (op1 != 9)
                sprintf(aux1, " %cX", op1 + 55);
            else
                sprintf(aux1, " AC");
        else
            sprintf(aux1, " %d ", op1);

        strcat(salida, aux1);
        if (existeOp2)
            strcat(salida, aux2);
    } else { //Sin operandos
        codaux = (cod >> 20) & 0xFFF;
        strcpy(salida, MNEM[codaux]);
    }

}

int anytoint(char *s, char **out) {
    char *BASES = {"********@*#*****%"};
    int base = 10;
    char *bp = strchr(BASES, *s);
    if (bp != NULL) {
        base = bp - BASES;
        ++s;
    }
    return strtol(s, out, base);
}

void SYS(int *a,int *b,int REG[],int RAM[]) {
    int i=0;
    char aux;

    switch(*a) {
    case 1:
       //arranco en DX+SD (segmento de datos)
        if (((REG[AX]>>8) &0x1 ) == 0x1){
            if (!((REG[AX]>>11) == 0x1))
                printf("\n[%04i]:",REG[DX]+i);

        scanf(" %c",&aux);
        while (aux != '\n'){
            RAM[REG[DS] +REG[DX]+i] = aux;
            scanf("%c",&aux);
            i++;
        }
        }else{
            for( i=0; i<=REG[CX]-1; i++){//CX veces
                if (!((REG[AX]>>11) == 0x1))
                    printf("\n[%04i]:",REG[DX]+i);
                switch(REG[AX]&0xF){            //La operacion depende del valor en AX
                case(0x1):
                    scanf("%i",&RAM[REG[DS]+REG[DX]+i]);
                    break;
                case(0x4):
                    scanf("%o",&RAM[REG[DS]+REG[DX]+i]);
                    break;
                case(0x8):
                    scanf("%x",&RAM[REG[DS]+REG[DX]+i]);
                    break;
                }
            }
            printf("\n");
        }
        break;
    case 2:
        for( i=0; i<=REG[12]-1; i++) {
            if (!((REG[10]>>11)&0xF))            //veifica %800
                printf("[%04i]:",REG[13]+i);
            if (REG[10]&0x1)
                printf("%i ",RAM[REG[0]+REG[13]+i]);
            if (REG[10]&0x4)
                printf("%o ",RAM[REG[0]+REG[13]+i]);
            if (REG[10]&0x8)
                printf("%x ",RAM[REG[0]+REG[13]+i]);
            if (REG[10]&0x10)
                printf("%c ",(char)RAM[REG[0]+REG[13]+i]);
            if (!((REG[10]>>8)&0x1))
                printf("\n");
        }
        break;
    case 15:
        if (flagb) {
            if (flagc)
                system("cls");//si es en otro SO es system("clear");
            if(flagd)
                sysactivado=1;
            if(!sysactivado)
                barrab(RAM,REG);
        }
        break;
    }
}

void muestra(int *a,int *b,int REG[],int RAM[]) {
    int i,j=0;
    char salida[50] = {' '};
    printf("\n");
    //printf("REG[IP]:%i\n",REG[IP]);
    if (REG[IP]<REG[DS]) {
           system("cls");
        if(REG[IP]<5)
            for(i=0; i<REG[IP]; i++) {
                RecuperaString(RAM[i],salida);
                printf("[%04i]:%02X %02X %02X %02X %i: %s \n",i,(RAM[i]>>24)&0xFF,(RAM[i]>>16)&0xFF,(RAM[i]>>8)&0xFF,(RAM[i]>>0)&0xFF,j++,salida);
            } else
            for(i=REG[IP]-5; i<REG[IP]; i++) {
                RecuperaString(RAM[i],salida);
                printf("[%04i]:%02X %02X %02X %02X %i: %s \n",i,(RAM[i]>>24)&0xFF,(RAM[i]>>16)&0xFF,(RAM[i]>>8)&0xFF,(RAM[i]>>0)&0xFF,j++,salida);
            }
        //printeo la actual con >
        RecuperaString(RAM[REG[IP]],salida);

        printf(">[%04i]:%02X %02X %02X %02X %i: %s \n",i,(RAM[i]>>24)&0xFF,(RAM[i]>>16)&0xFF,(RAM[i]>>8)&0xFF,(RAM[i]>>0)&0xFF,j++,salida);
        i  = REG[IP];
        if(REG[DS]-i>5)
            for(i=REG[IP]+1; i<=REG[IP]+4; i++) {
                RecuperaString(RAM[i],salida);
                printf("[%04i]:%02X %02X %02X %02X %i: %s \n",i,(RAM[i]>>24)&0xFF,(RAM[i]>>16)&0xFF,(RAM[i]>>8)&0xFF,(RAM[i]>>0)&0xFF,j++,salida);
            } else
            for(i=REG[IP]+1; i<REG[DS]; i++) {
                RecuperaString(RAM[i],salida);
                printf("[%04i]:%02X %02X %02X %02X %i: %s \n",i,(RAM[i]>>24)&0xFF,(RAM[i]>>16)&0xFF,(RAM[i]>>8)&0xFF,(RAM[i]>>0)&0xFF,j++,salida);
            }
        printf("Registros: \n");
        printf("DS = \t %i \n",REG[DS]);
        printf("\t IP = \t %i \n",REG[IP]);
        printf("CC = \t %d | AC = \t %d | AX = \t %d | BX = \t %d \n",REG[CC],REG[AC],REG[AX],REG[BX]);
        printf("CX = \t %d | DX = \t %d | EX = \t %d | FX = \t %d \n",REG[CX],REG[DX],REG[14],REG[15]);
        barrab(RAM,REG);
    }
}


void barrab(int RAM[],int REG[]) {

    int d1,d2;
    int j=0,k=0;
    char c[10],aux[10];

    printf("\n[%04i] cmd:",REG[IP]);
    fflush(stdin);
    gets(c);

    if(!strcmp(c,"\0"))
        sysactivado=0;
    else if(!strcmp(c,("p")))
        sysactivado = 1; //redundante
    else {
        while(strcmp(c,"\0") && strcmp(c,"p")){
            j=0;
            while(c[j]!='\0'&&c[j]!=' ') {
                aux[j]=c[j];                 //guardo en aux el primer decimal
                j++;
            }
            aux[j] = '\0';

            d1=anytoint(aux,NULL);
            if(c[j]=='\0')
                printf("[%04i]: %i\n",d1,RAM[d1+REG[DS]]); //estoy en el caso donde el usuario ingreso UN SOLO DECIMAL
            else {
                k=0;
                while(c[j]!='\0') {
                    aux[k]=c[j];                 //guardo en aux el segundo decimal
                    j++;
                    k++;
                }
                d2=anytoint(aux,NULL);
                for(j=d1; j<=d2; j++)

                    printf("[%04i]: %i\n",j,RAM[j+REG[DS]]);
            }
            printf("\n[%04i] cmd:",REG[IP]);gets(c);
        }
    }
}




void JMP(int *a,int *b,int REG[],int RAM[]) {

    REG[IP]=(*a)-1; //REG[IP] se incrementa +1 al terminar la instruccion
}                  //esto se aplica a TODOS los jump

void JP(int *a,int *b,int REG[],int RAM[]) {
    if (((REG[CC]>>31&0b1) ==0b0) && (REG[CC]&0b1) ==0b0)
        REG[IP]=*a-1;
}


void JN(int *a,int *b,int REG[],int RAM[]) {
    if (((REG[CC]>>31&0b1) ==0b1) && (REG[CC]&0b1) ==0b0)
        REG[IP]=*a-1;
}

void JZ(int *a,int *b,int REG[],int RAM[]) {
    if( ((REG[CC])&0b1)==0b1)
        REG[IP]=*a-1;
}

void JNZ(int *a,int *b,int REG[],int RAM[]) {
    if( ((REG[CC])&0b1)==0b0)
        REG[IP]=*a-1;
}

void JNP(int *a,int *b,int REG[],int RAM[]) {
    if (!((REG[CC]>>31&0b1)==0b0) || (REG[CC]&0b1) ==0b1)
        REG[IP]=*a-1;
}

void JNN(int *a,int *b,int REG[],int RAM[]) {
    if (!((REG[CC]>>31&0b1)==0b1) || (REG[CC]&0b1) ==0b1)
        REG[IP]=*a-1;
}

void LDH(int *a,int *b,int REG[],int RAM[]) {
    REG[AC] = (((*a)&0x00FF)<<8)|(REG[AC]&0x00FF);
}

void LDL(int *a,int *b,int REG[],int RAM[]) {
    REG[AC] = ((*a)&0x00FF)|(REG[AC]&0xFF00);
}

void RND(int *a,int *b,int REG[],int RAM[]) {
    srand(time(NULL));
    *a=*a+rand()%(*b+1-*a);
}

void NOT(int *a,int *b,int REG[],int RAM[]) {
    *a=~(*a);
    cargarCC(a,REG);
}

void STOP(int *a,int *b,int REG[],int RAM[]) {
    REG[IP]=REG[DS];
}

/*ANTIGUO CODIGO PARA LEER LAS INSTRUCCIONES DESDE UN .TXT


void cargainstruccion(instruccion v[]){

    FILE *arch;
    int aux,i=0;
    char c,instr[5];

    arch=fopen("instrucciones.txt","rt");
    while(!feof(arch)){

        fscanf(arch,"%c",&c);
        aux=0;
        while(c!=' '){
            instr[aux]=c;
            fscanf(arch,"%c",&c);
            aux++;
        }
        instr[aux]='\0';

        strcpy(v[i].inst,instr);
        //printf("%s",v[i].inst);
        fscanf(arch,"%i%i",&v[i].codigo,&v[i].operandos);
        //printf(" %x\t%i",v[i].codigo,v[i].operandos);
        i++;
    }
    fclose(arch);
}
*/
