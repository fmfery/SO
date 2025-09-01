#ifndef INTERPRETE_H
#define INTERPRETE_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int PC=0;
int Ax=0;
int Bx=0;
int Cx=0;
int Dx=0;

/*
    Comprueba que la cadena sea digitos
*/
int Numeros(char *numero){
    if(numero[0] == '\0') return 0;
    int i=0;
    for ( i = 0; numero[i] != '\0'; i++)
    {
        if (!isdigit(numero[i]))
        {
            return 0;
        }
        
    }
    return 1;

}
int registroValido(char *letrasM){
   return(strcmp(letrasM,"Ax")==0 || strcmp(letrasM,"Bx")==0 || strcmp(letrasM,"Cx")==0 || strcmp(letrasM,"Dx")==0);
         
}


int leer_instruccion(FILE *fp){
        char linea[256];
        char instruccion[5];   //va a leer lo primero
        char letrasM[3];
        char numeros[10];
        
    if(fgets(linea,sizeof(linea),fp)==NULL){ //lee la linea del archivo de la linea 
        return 1;
    }
    
        PC++; //
    linea[strcspn(linea,"\n")]='\0';

    if(linea == NULL){
        //si la linea no tiene nada no hace nada
        return 0;
    }
    int token;
                    //lee el formato de MOV Ax,5 
    token = sscanf(linea,"%s %[^,],%s",instruccion,letrasM,numeros);
    //printf("%s %s %s\n",instruccion,letrasM,numeros);
     
    if(token < 2){
        token=sscanf(linea,"%s %s",instruccion, letrasM);
        numeros[0]='\0';
       
    }
    if(token<1){
        printf("Error:Falta registro y/o valor\n");
        return 1;

    }
    
    if(strcmp(instruccion,"MOV")==0 || strcmp(instruccion,"ADD")==0 || strcmp(instruccion,"SUB")==0 || strcmp(instruccion,"MULT")==0 || strcmp(instruccion,"DIV")==0){
        
        if(token < 3){
        printf("___________________________\n");
        printf("%s %s ",instruccion,letrasM);
        
            printf("Error: Hace falta un valor\n");
            return 0;

        }
        
        if(!registroValido(letrasM)){
            printf("Error: registro no valido\n");
            return 1;
        } 
            
            
        if(!Numeros(numeros)){
            printf("Error: No es un numero\n");
            return 1;
        }
        if(strcmp(instruccion,"MOV")==0){
            int valor=0;
            valor=atoi(numeros); //va a ser de cadena a numeros
            if(strcmp(letrasM,"Ax")==0) Ax=valor;
            else if(strcmp(letrasM,"Bx")==0) Bx=valor;
            else if(strcmp(letrasM,"Cx")==0) Cx=valor;
            else if(strcmp(letrasM,"Dx")==0) Dx=valor;
        }
        else if(strcmp(instruccion,"ADD")==0){
            int valor=0;
            valor=atoi(numeros); //va a ser de cadena a numeros
            if(strcmp(letrasM,"Ax")==0) Ax+=valor;
            else if(strcmp(letrasM,"Bx")==0) Bx+=valor;
            else if(strcmp(letrasM,"Cx")==0) Cx+=valor;
            else if(strcmp(letrasM,"Dx")==0) Dx+=valor;
        }
        else if(strcmp(instruccion,"SUB")==0){
            int valor=0;
            valor=atoi(numeros); //va a ser de cadena a numeros
            if(strcmp(letrasM,"Ax")==0) Ax-=valor;
            else if(strcmp(letrasM,"Bx")==0) Bx-=valor;
            else if(strcmp(letrasM,"Cx")==0) Cx-=valor;
            else if(strcmp(letrasM,"Dx")==0) Dx-=valor;
         }
        else if(strcmp(instruccion,"MULT")==0){
            int valor=0;
            valor=atoi(numeros); //va a ser de cadena a numeros
            if(strcmp(letrasM,"Ax")==0) Ax*=valor;
            else if(strcmp(letrasM,"Bx")==0) Bx*=valor;
            else if(strcmp(letrasM,"Cx")==0) Cx*=valor;
            else if(strcmp(letrasM,"Dx")==0) Dx*=valor;
        }
         else if(strcmp(instruccion,"DIV")==0){
            int valor=0;
            valor=atoi(numeros); //va a ser de cadena a numeros
            if(valor==0){
                printf("Error:Divicion entre cero\n");
                return 0;
            }
            if(strcmp(letrasM,"Ax")==0) Ax/=valor;
            else if(strcmp(letrasM,"Bx")==0) Bx/=valor;
            else if(strcmp(letrasM,"Cx")==0) Cx/=valor;
            else if(strcmp(letrasM,"Dx")==0) Dx/=valor;
        }
    
        printf("___________________________\n");
        printf("%s %s ",instruccion,letrasM);
        printf("Correcto\n");

        printf(" Ax:  %d, Bx: %d, Cx: %d, Dx %d \n",Ax,Bx,Cx,Dx);
      
        }
    else if(strcmp(instruccion,"INC")==0 || strcmp(instruccion,"DEC")==0){
        
        if(token < 2){

        printf("___________________________\n");
        printf("%s ",instruccion);
            printf("Error: Hace falta un registro\n");
            return 0;

        }
        
        if(!registroValido(letrasM)){
        printf("%s %s",instruccion,letrasM);
            printf("Error: Registro no valido\n");
            return 1;
        } 
        
        if(token == 3){
        
            printf("Error: No se permiten ese tipo de valores\n");
            return 1;
        }

        if(strcmp(instruccion,"INC")==0){
            
           
            if(strcmp(letrasM,"Ax")==0) Ax++;
            else if(strcmp(letrasM,"Bx")==0) Bx++;
            else if(strcmp(letrasM,"Cx")==0) Cx++;
            else if(strcmp(letrasM,"Dx")==0) Dx++;
        }
        else if(strcmp(instruccion,"DEC")==0){
          
            if(strcmp(letrasM,"Ax")==0) Ax--;
            else if(strcmp(letrasM,"Bx")==0) Bx--;
            else if(strcmp(letrasM,"Cx")==0) Cx--;
            else if(strcmp(letrasM,"Dx")==0) Dx--;
        }



        printf("___________________________\n");
        printf("%s %s ",instruccion,letrasM);
        
        printf("Correcto\n");

        printf(" Ax:  %d, Bx: %d, Cx: %d, Dx %d \n",Ax,Bx,Cx,Dx);

    }

}
//---------------------------
//---Verifica la extencion---
//---------------------------
int VerificarE(const char *nombre_archivo) {
    char copia[255];
    char *token_acceso;
    

    strcpy(copia, nombre_archivo);

    token_acceso = strtok(copia, ".");
    while (token_acceso != NULL) {
// -----------avanzamos hasta la ultima parte
        char *ultimo = token_acceso;
        token_acceso = strtok(NULL, ".");

        if (token_acceso == NULL) {
//-------------cuando ya no hay mas, la extension
            if (strcmp(ultimo, "asm") == 0) {
                return 1;                               // extension valida
            } else {
                printf("Error: extension no valida\n");
                return 0;                               //  no es asm
            }
        }
    }
    return 0; 
}
//------------------------------
//--Ejecuta el archivo..........
//------------------------------
void ejecutar_archivo(const char *archivo) {
    int sig_linea=0;                              //va a hacer un contador de lineas, en este caso la primera

    if (!VerificarE(archivo)) {                   //si no cumple va a mandar un mensaje que no es correcto
        return;                                   // no es asm y lo rompe 
    } else{
    
    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo: %s\n", archivo);
        return;
    }

   // printf("Tokens del archivo %s\n", archivo);

    char linea[256];
    //'''''''Lee linea x linea'''''''''''''
    while (!feof(fp))
    {
    
        leer_instruccion(fp);               //lee la linea actualz
        sig_linea++;                       //lee la siguiente linea
        printf("%d\n",sig_linea);
    }
    

    fclose(fp);
}

}

/*
MOV Ax,5
ADD Bx,7
SUB Cx,4
MULT Dx,2
DIV Ax,2
INC Bx
DEC Cx
*/







#endif

