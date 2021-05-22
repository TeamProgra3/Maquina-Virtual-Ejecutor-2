#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
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
    INST[0xC]=SLEN;
    INST[0xD]=SMOV;
    INST[0xE]=SCMP;
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
    INST[0xFC]=PUSH;
    INST[0xFD]=POP;
    INST[0xFE]=CALL;
    INST[0xFF0]=RET;
    INST[0xFF1]=STOP;
    srand(time(0));
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
    MNEM[0xC]="SLEN";
    MNEM[0xD]="SMOV";
    MNEM[0xE]="SCMP";
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
    MNEM[0xFC]="PUSH";
    MNEM[0xFD]="POP";
    MNEM[0xFE]="CALL";
    MNEM[0xFF0]="RET";
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

void SLEN(int *a,int *b,int REG[],int RAM[]) {
    int i=0;
    while (*(b+i) != 0x0)
        i++;
    (*a)=i;
}

void SMOV(int *a,int *b,int REG[],int RAM[]) {
    int i=0;
    while (*(b+i) != 0x0){
        *(a+i)=*(b+i);
        i++;
    }
    *(a+i) = 0x0;
}

void SCMP(int *a,int *b,int REG[],int RAM[]) {
    int i=0,aux=0;
    aux = *a - *b;
    while((*(a+i) != 0x0 || *(b+i) != 0x0) && aux == 0){
        aux = *(a+i) - *(b+i);
        i++;
    }
    cargarCC(&aux,REG);
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
    int offset,registro,codaux, op1, op2, tipoOp1, tipoOp2, existeOp2 = 0;
    char auxOffset[50],aux1[50], aux2[50];
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
                sprintf(aux2, ", [%d] ", op2);
            else if (tipoOp2 == 1){
                if (op2 >=10 && op2 <= 15)
                    sprintf(aux2, ", %cX", op2 + 55);
                else if (op2 == 9)
                    sprintf(aux2, ", AC");
                else if (op2 == 6)
                    sprintf(aux2, ", SP");
                else if (op2 == 7)
                    sprintf(aux2, ", BP");
            }
            else if (tipoOp2 == 0)
                    sprintf(aux2, ", %d ", op2);
            else{//tipo op3
                offset = (op2>>4) & 0xFF;
                offset <<= 24;
                offset >>= 24;
                registro = op2 & 0xF;
                if (registro >= 10 && registro <=15)
                    sprintf(aux2, ", [%cX", op2 + 55);
                else if ( registro == 7)
                    sprintf(aux2, ", [BP");
                else if ( registro == 6)
                    sprintf(aux2, ", [SP");
                sprintf(auxOffset, "%d", offset);
                if (offset > 0){
                     strcat(aux2, "+");
                     strcat(aux2, auxOffset);
                }
                else if (offset < 0){
                    strcat(aux2, auxOffset);
                }
                strcat(aux2, "]");
            }
        }
        if (tipoOp1 == 2)
            sprintf(aux1, " [%d]", op1);
        else if (tipoOp1 == 1){
            if (op1 >=10 && op1 <= 15)
                sprintf(aux1, " %cX", op1 + 55);
            else if (op1 == 9)
                sprintf(aux1, " AC");
            else if (op1 == 6)
                sprintf(aux1, " SP");
            else if (op1 == 7)
                sprintf(aux1, " BP");
            }
        else if (tipoOp1 == 0)
                sprintf(aux1, " %d ", op1);
        else{//tipo op3
                offset = (op1>>4) & 0xFF;
                offset <<= 24;
                offset >>= 24;
                registro = op1 & 0xF;
                if (registro >= 10 && registro <=15)
                    sprintf(aux1, " [%cX", op2 + 55);
                else if ( registro == 7)
                    sprintf(aux1, " [BP");
                else if ( registro == 6)
                    sprintf(aux1, " [SP");
                sprintf(auxOffset, "%d", offset);
                if (offset > 0){
                     strcat(aux1, "+");
                     strcat(aux1, auxOffset);
                }
                else if (offset < 0){
                    strcat(aux1, auxOffset);
                }
                strcat(aux1, "]");
            }
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
    int DSL,i=0,DXL,DXH,ESL,SSL,CSL,DIR;
    int ant,act,antUtilizado,actUtilizado;
    char aux,c;
    int Eseg[ REG[ES]>>16 ];
    DSL = REG[DS] & 0xFFFF;
    DXL = REG[DX] & 0xFFFF;
    DXH = (REG[DX] >> 16)& 0xFFFF;
    ESL = REG[ES] & 0xFFFF;
    SSL = REG[SS] & 0xFFFF;
    CSL = REG[CS] & 0xFFFF;
    switch(DXH){
    case 0:
        DIR = DXL + DSL;
    break;
    case 1:
        DIR = DXL + ESL;
    break;
    case 2:
        DIR = DXL + SSL;
    break;
    case 3:
        DIR = DXL + CSL;
    break;
    }


    switch(*a) {
    case 1:
       //arranco en DX+SD (segmento de datos)
        if (((REG[AX]>>8) &0x1 ) == 0x1){
            if (!((REG[AX]>>11) == 0x1))
                printf("\n[%04i]:",DXL+i);
        scanf(" %c",&aux);
        while (aux != '\n'){
            RAM[DSL+DXL+i] = aux;
            scanf("%c",&aux);
            i++;
        }
        }else{
            for( i=0; i<=REG[CX]-1; i++){//CX veces
                if (!((REG[AX]>>11) == 0x1))
                    printf("\n[%04i]:",DXL+i);
                switch(REG[AX]&0xF){            //La operacion depende del valor en AX
                case(0x1):
                    scanf("%i",&RAM[DSL+DXL+i]);
                    break;
                case(0x4):
                    scanf("%o",&RAM[DSL+DXL+i]);
                    break;
                case(0x8):
                    scanf("%x",&RAM[DSL+DXL+i]);
                    break;
                }
            }
            printf("\n");
        }
        break;
    case 2:

        for( i=0; i<=REG[CX]-1; i++) {
            if (!((REG[AX]>>11)&0xF))            //veifica %800
                printf("[%04i]:",DSL+DXL+i);
            if (REG[AX]&0x1)
                printf("%i ",RAM[DSL+DXL+i]);
            if (REG[AX]&0x4)
                printf("%o ",RAM[DSL+DXL+i]);
            if (REG[AX]&0x8)
                printf("%x ",RAM[DSL+DXL+i]);
            if (REG[AX]&0x10)
                printf("%c ",(char)RAM[DSL+DXL+i]);
            if (!((REG[AX]>>8)&0x1))
                printf("\n");
        }
        break;
    case 3: //(STRING READ)
        if (!((REG[AX]>>11)&0xF))
            printf("[%04i]:",DIR);
        scanf("%c",&c);
        while (c != '\n' && i < REG[CX]-1){
            RAM[DIR+i]=c;
            i++;
            scanf("%c",&c);
        }
        RAM[DIR+i]=0; //\0 o 0?
        break;
    case 4: //(STRING WRITE)

        while (RAM[DIR+i] != 0 && i < REG[CX]-1){
            if (!((REG[AX]>>11)&0xF))
                printf("[%04i]:",DIR+i);
            printf("%c",RAM[DIR+i]);
            i++;
        }
        if (!((REG[AX]>>8)&0x1))
            printf("\n");
        break;
    case 5: //Requiere en CX la cantidad de celdas que se solicitan y devuelve en DX un puntero a la primer celda para su uso dentro de ES
        //int ESeg[ REG[ES]>>16 ];
        //Inicializo listas, segmento extra y posiciono la RAM
        if(REG[HP]==0xFFFFFFFF){
            for(i=0;i<=REG[ES]>>16;i++)
                Eseg[i]=(REG[ES]&0x0000FFFF)+i;
            cargaHL(&RAM[ Eseg[0] ],(REG[ES]>>16)-1,0x0000);//pongo en la parte alta de ES[0] el tama�o del extra segment -1
            REG[HP]=0x0000FFFF;//HPH=0;HPL=-1
        }
        if( (REG[HP]&0xFFFF)==0xFFFF){ //Si la lista de ocupados esta vacia
            cargaHL(&RAM[ Eseg[REG[CX]+1] ],(RAM[Eseg[0]]>>16)-REG[CX]-1,Eseg[REG[CX]+1]);
            cargaHL(&RAM[Eseg[0]],REG[CX],0x0000);
            cargaHL(&REG[HP],REG[CX]+1,0x0000);
            REG[DX]=(REG[HP]&0xFFFF)+1;
        }else{
            act=REG[HP]>>16;
            while((RAM[Eseg[act]]&0x0000FFFF)!=REG[HP]>>16 && (RAM[Eseg[act]]>>16) < REG[CX]){ //mientras NO este en el ultimo header Y el HPH tenga un tama�o menor al requerido
                ant=act;
                act=RAM[Eseg[act]]&0x0000FFFF; //pongo en actual el puntero al siguiente nodo header de HPH
            }
            actUtilizado=REG[HP]&0x0000FFFF;//con la segunda lista(la de ocupados) voy recorriendo hasta alcanzar la de libres
            while( actUtilizado<act && (RAM[Eseg[actUtilizado]]&0x0000FFFF)!=REG[HP]>>16){
                antUtilizado=actUtilizado;//3 [ 5  13 ]
                actUtilizado=RAM[Eseg[actUtilizado]]&0x0000FFFF;//13[ 4  3 ]
            }
            if((RAM[Eseg[act]]>>16) == REG[CX]){
                cargaHL(&RAM[Eseg[antUtilizado]] , RAM[Eseg[antUtilizado]]>>16 ,RAM[Eseg[act]]);//1� nodo utilizados
                cargaHL(&RAM[Eseg[act]]          , REG[CX]                     ,actUtilizado);//2� nodo utilizados
                cargaHL(&RAM[Eseg[ant]]          , RAM[Eseg[ant]]>>16          ,RAM[Eseg[act]]&0x0000FFFF);//conecto el nodo anterior con el siguiente de actual para HPH

                if(REG[HP]>>16==RAM[Eseg[act]])
                    cargaHL(&REG[HP]             , RAM[Eseg[act]]&0x0000FFFF   ,act);
                else
                    cargaHL(&REG[HP]             , REG[HP]>>16                 ,act);
            }else{
                if((RAM[Eseg[act]]>>16) > REG[CX]){
                    cargaHL(&RAM[Eseg[antUtilizado]]  ,Eseg[antUtilizado]>>16          ,RAM[Eseg[act]]);
                    cargaHL(&RAM[Eseg[act]]           ,REG[CX]                         ,actUtilizado);
                    cargaHL(&RAM[Eseg[ant]]           ,Eseg[ant]>>16                   ,act+REG[CX]);
                    cargaHL(&RAM[Eseg[act+REG[CX]+1]] ,actUtilizado-(act+REG[CX]+1)-1  ,RAM[Eseg[act]]&0x0000FFFF);
                    if(REG[HP]>>16==RAM[Eseg[act]])
                        cargaHL(&REG[HP]              ,(REG[HP]>>16)+REG[CX]           ,act);
                    else
                        cargaHL(&REG[HP]              , REG[HP]>>16                    ,act);
                }
            }
            if((RAM[Eseg[act]]&0x0000FFFF)!=REG[HP]>>16 && (RAM[Eseg[act]]>>16)<REG[CX])
                printf("Espacio insuficiente en memoria dinamica");
            else
                cargaHL(&REG[DX],RAM[Eseg[act]]>>16,act+1);
        }
        break;
    case 6: //libera la memoria indicada en DX
        break;
    case 7:
        system("cls");
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
    int contInstrucciones,SPL,BPL,ESH,ESL,SSH,SSL,CSL,DSL,CSH,DSH,i,j=0;
    char salida[50] = {' '};
    printf("\n");
    SPL = REG[SP] & 0xFFFF;
    BPL = REG[BP] & 0xFFFF;
    DSH = (REG[DS] >> 16) & 0xFFFF;
    DSL = REG[DS] & 0xFFFF;
    ESH = (REG[ES] >> 16) & 0xFFFF;
    ESL = REG[ES] & 0xFFFF;
    SSH = (REG[SS] >> 16) & 0xFFFF;
    SSL = REG[SS] & 0xFFFF;
    CSH = (REG[CS] >> 16) & 0xFFFF;
    CSL = REG[CS] & 0xFFFF;
    while((RAM[contInstrucciones]>128 || RAM[contInstrucciones]<0) && contInstrucciones<CSH)
         contInstrucciones++;
    if (REG[IP]<contInstrucciones) {
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
        if(contInstrucciones-i>5)
            for(i=REG[IP]+1; i<=REG[IP]+4; i++) {
                RecuperaString(RAM[i],salida);
                printf("[%04i]:%02X %02X %02X %02X %i: %s \n",i,(RAM[i]>>24)&0xFF,(RAM[i]>>16)&0xFF,(RAM[i]>>8)&0xFF,(RAM[i]>>0)&0xFF,j++,salida);
            } else
            for(i=REG[IP]+1; i<contInstrucciones; i++) {
                RecuperaString(RAM[i],salida);
                printf("[%04i]:%02X %02X %02X %02X %i: %s \n",i,(RAM[i]>>24)&0xFF,(RAM[i]>>16)&0xFF,(RAM[i]>>8)&0xFF,(RAM[i]>>0)&0xFF,j++,salida);
            }
        printf("\n Registros: \n");
        printf("DS = %X --> DSH = %d | DSL = %d ||| ",REG[DS], DSH,DSL);
        printf("ES = %X --> ESH = %d | ESL = %d \n",REG[ES], ESH,ESL);
        printf("SS = %X --> SSH = %d | SSL = %d ||| ",REG[SS], SSH,SSL);
        printf("CS = %X --> CSH = %d | CSL = %d \n",REG[CS], CSH,CSL);
        printf("IP = \t %i | CC- = \t %d | CC0 = \t %d\n",REG[IP],(REG[CC]>>31)&1,(REG[CC])&1);
        printf("HP = \t %d | AC = \t %d | AX = \t %d | BX = \t %d \n",REG[HP],REG[AC],REG[AX],REG[BX]);
        printf("CX = \t %d | DX = \t %d | EX = \t %d | FX = \t %d \n",REG[CX],REG[DX],REG[14],REG[15]);
        printf("SPL = \t %d (Abs: %d) | BPL = \t %d (Abs: %d) \n",SPL,SPL+SSL,BPL,BPL+SSL);
        if (SPL != SSH){
            printf("== STACK: [%d",RAM[SPL + SSL]);
            for (int k = SPL + SSL+1; k < SSH + SSL;k++) {
                printf(", %d",RAM[k]);
            }
            printf("] ==\n");
        }
        barrab(RAM, REG);
    }
}
void barrab(int RAM[],int REG[]) {

    int d1,d2;
    int j=0,k=0;
    char c[10],aux[10],aux2[10];

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
                aux[j]=c[j];
                j++;  //guardo en aux el primer decimal
            }
            aux[j] = '\0';
            d1=anytoint(aux,NULL);
            if(c[j]=='\0')
                printf("[%04i]: %i\n",d1,RAM[d1]); //estoy en el caso donde el usuario ingreso UN SOLO DECIMAL
            else {
                k=0;
                while(c[j]!='\0') {
                    //aux[k++]=c[j++];
                    aux2[k]=c[j];                 //guardo en aux el segundo decimal
                    j++;
                    k++;                //guardo en aux el segundo decimal
                }
                d2=anytoint(aux2,NULL);
                for(j=d1; j<=d2; j++)
                    printf("[%04i]: %i\n",j,RAM[j]);
            }
            printf("\n[%04i] cmd:",REG[IP]);gets(c);
        }
    }
}

void cargaHL(int *a,int H,int L ) {// H=High L=Low
    int aux;
	aux=(H<<16)&(0xFFFF0000);
	*a=aux+L;
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
    REG[AC] = (((*a)<<16)&0xFFFF0000)|(REG[AC]&0xFFFF);
}

void LDL(int *a,int *b,int REG[],int RAM[]) {
    REG[AC] = ((*a)&0xFFFF)|(REG[AC]&0xFFFF0000);
}

void RND(int *a,int *b,int REG[],int RAM[]) {
    REG[AC]=(rand())%(*a);
}

void NOT(int *a,int *b,int REG[],int RAM[]) {
    *a=~(*a);
    cargarCC(a,REG);
}
void PUSH(int *a,int *b,int REG[],int RAM[]) {
    int SPL = REG[SP] & 0xFFFF;
    int SSL = REG[SS] & 0xFFFF;
    if (SPL == 0){
        printf("==================================\n");
        printf("STACK OVERFLOW! Instruccion nro: %d \n", REG[IP]+1);
        printf("SP = 0x%X || BP = 0x%X || SS = 0x%X\n", REG[SP], REG[BP], REG[SS]);
        printf("SPL = %d - Tamanio pila: %d\n", SPL, (REG[SS]>>16)&0xffff);
        printf("==================================\n");
        REG[IP]=REG[DS];
    } else{
        REG[SP]--;
        SPL = REG[SP] & 0xFFFF;
        RAM[SSL+SPL] = *a;
    }
}
void POP(int *a,int *b,int REG[],int RAM[]) {
    int SPL = REG[SP] & 0xFFFF;
    int SSL = REG[SS] & 0xFFFF;
    int SSH = REG[SS]>>16 & 0xFFFF;
     if (SPL == SSH){ //Celda que quiero es igual al tamaño
        printf("==================================\n");
        printf("STACK UNDERFLOW! Instruccion nro: %d \n", REG[IP]+1);
        printf("SP = 0x%X || BP = 0x%X || SS = 0x%X\n", REG[SP], REG[BP], REG[SS]);
        printf("SPL = %d - Tamanio pila: %d\n", SPL, (REG[SS]>>16)&0xffff);
        printf("==================================\n");
     } else {
         *a = RAM[SSL + SPL];
         REG[SP]++;
    }

}
void CALL(int *a,int *b,int REG[],int RAM[]) {
    int SPL = REG[SP] & 0xFFFF;
    int SSL = REG[SS] & 0xFFFF;
    if (SPL == 0){
        printf("==================================\n");
        printf("STACK OVERFLOW! Instruccion nro: %d \n", REG[IP]+1);
        printf("SP = 0x%X || BP = 0x%X || SS = 0x%X\n", REG[SP], REG[BP], REG[SS]);
        printf("SPL = %d - Tamanio pila: %d\n", SPL, (REG[SS]>>16)&0xffff);
        printf("==================================\n");
        REG[IP]=REG[DS];
    } else {
        REG[SP]--;
        SPL = REG[SP] & 0xFFFF;
        RAM[SSL+SPL] = REG[IP]+1;
        REG[IP] = (*a)-1; //Porque IP se incrementa solo al terminas la instruccion
    }


}
void RET(int *a,int *b,int REG[],int RAM[]) {
    int SPL = REG[SP] & 0xFFFF;
    int SSL = REG[SS] & 0xFFFF;
    int SSH = REG[SS]>>16 & 0xFFFF;
     if (SPL == SSH){ //Celda que quiero es igual al tamaño
        printf("==================================\n");
        printf("STACK UNDERFLOW! Instruccion nro: %d \n", REG[IP]+1);
        printf("SP = 0x%X || BP = 0x%X || SS = 0x%X\n", REG[SP], REG[BP], REG[SS]);
        printf("SPL = %d - Tamanio pila: %d\n", SPL, (REG[SS]>>16)&0xffff);
        printf("==================================\n");
     } else {
         REG[IP] = RAM[SSL + SPL]-1; //-1 porque IP se incrementa solo al terminar instruccion
         REG[SP]++;
    }

}
void STOP(int *a,int *b,int REG[],int RAM[]) {
    REG[IP]=REG[DS];
}
