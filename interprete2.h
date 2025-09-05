#ifndef INTERPRETE_H
#define INTERPRETE_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
//---Variables globales----
int PC=1;
int Ax=0;
int Bx=0;
int Cx=0;
int Dx=0;
int ID=0;
//int PC=1;

/*
    Comprueba que la cadena sea digitos
*/
int Numeros(char *numero){
    if(numero[0] == '\0') return 0;         //si la cadena esta vacia regresa cero ya que no es valido
    int i=0;
    for ( i = 0; numero[i] != '\0'; i++)    //si la cadena no esta vacia va a revisar que lo ingresado no sea 1a2
    {
        if (!isdigit(numero[i]))            //si no es un numero lo que lee entonces Error
        {
            return 0;                       //si encuentra algun digito(numero) que no cumple regresa cero
        }
        
    }
    return 1;                               //si no encontro errores todo esta bien

}
/* 
    Comprobar si es correcto el registro de las letras
*/
int registroValido(char *letrasM){
   return(strcmp(letrasM,"Ax")==0 || strcmp(letrasM,"Bx")==0 || strcmp(letrasM,"Cx")==0 || strcmp(letrasM,"Dx")==0);
    //El registro va a ser valido si cumple con alguna de esas      
}


int leer_instruccion(FILE *fp, const char *archivo){
        char linea[256];                                //Una linea
        char instruccion[5];                            //MOV ADD ..ese tipo de instruccion
        char letrasM[3];                                //la letras Ax Bx ...
        char numeros[10];                               //Los numeros despues de las comas ,
        
    if(fgets(linea,sizeof(linea),fp)==NULL){             //lee la linea del archivo de la linea 
        return 1;
    }
    
        PC++;                                           //El contador
    linea[strcspn(linea,"\n")]='\0';                   //Si la linea es \n una salto de linea eto va a ser nulo

    if(linea == NULL){
                                                                     //si la linea no tiene nada no hace nada
        return 0;                                                    //indicamos que todo esta bien
    }

    int token;                                                        //vamos a dividirlo por tokens

/* Revisando que tenga alguna de la estroctura ---------------------------- */
                    //lee el formato de MOV Ax,5 
    token = sscanf(linea,"%s %[^,],%s",instruccion,letrasM,numeros);     //solo le damos el formato
    //printf("%s %s %s\n",instruccion,letrasM,numeros);

 /* Si da solo 2 */    
    if(token == 2){             
        token=sscanf(linea,"%s %s",instruccion, letrasM);       //Si solo leyo ese tipo de linea como DEC Ax o INC Bx
        //que es cuando la instruccion no lleva un numero
        numeros[0]='\0';                                        //como no resive numeros vaciamos la cadena que esta vacia
       
    }
    if(token == 1){                                                 //No leyo una estoctura correcta AD o solo tiene ADD sin ningua otra cosa
     printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s       Error: Hace falta registro\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion);
        return 1;

    }

   
/*  Una vez revisada la estoctura, revisa que si este mandando correctamente las palabras------------------------------------------------------------------------------*/
    if(strcmp(instruccion,"MOV")==0 || strcmp(instruccion,"ADD")==0 || strcmp(instruccion,"SUB")==0 || strcmp(instruccion,"MULT")==0 || strcmp(instruccion,"DIV")==0){
        
        if(token < 3){                                                     //Si el token detecto 2 entonces 
        printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s     Error: Hace falta un valor\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM);

            return 0;

        }
/*  Verificamos que si no es Ax, Bx, etc de un error -------------------------------------------------------------------------------------------------------------------*/
        if(!registroValido(letrasM)){
            printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s     Error: Registro no valido\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM);
            //va a decir error, si nunca entra significa que todo es correcto
            return 1;
        } 
            
/*    Si no encontro numeros ---------------------------------------------------------------------------------------------------------------------------------------------*/            
        if(!Numeros(numeros)){
            printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s,%s    Error: No es un numero\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM,numeros);

            return 1;
        }
/*    Se hacen las operacines correspondientes----------------------------------------------------------------------------------------------------------------------------*/
        if(strcmp(instruccion,"MOV")==0){                               //si cumple con MOV
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
                printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s,%s    Error: Divicion entre cero\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM,numeros);
                return 0;
            }
            if(strcmp(letrasM,"Ax")==0) Ax/=valor;
            else if(strcmp(letrasM,"Bx")==0) Bx/=valor;
            else if(strcmp(letrasM,"Cx")==0) Cx/=valor;
            else if(strcmp(letrasM,"Dx")==0) Dx/=valor;
        }
    
     
       printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s,%s      Correcto\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM,numeros);



        
        }
/*  Si no leyo ninguna MOV , etc y leyo algo de esto ------------------------------------------------------------*/
    else if(strcmp(instruccion,"INC")==0 || strcmp(instruccion,"DEC")==0){
            token = sscanf(linea, "%s %s %s", instruccion, letrasM, numeros);

        if(token < 2){                                              //Si solo leyo una instruccion como INC nadamas

        //printf("%s ",instruccion);
            printf("Error: Hace falta un registro\n");              //imprime Error solo leyo
      
            return 0;

        }
        //if(token )
        
        if(!registroValido(letrasM)){                               //Llamamos a la variable para que valide las letras
        //  printf("%s %s",instruccion,letrasM);                        //si leyo que no cumple manda un error
             printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s      Error: Registro no valido\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM);
            return 1;
        } 
        
        if(token == 3){                                             //Si solo cumplio con con tres cosas INC Ax algo_mas eso no es correcto
       // printf("%s %s \n",instruccion,letrasM);
         //   printf("Error: No se permiten ese tipo de valores-\n");
         printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s...     Error: Registro no valido\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM);
            return 1;
        }
        if(token > 3){                                             //Si solo cumplio con con tres cosas INC Ax algo_mas eso no es correcto
        printf("%s %s \n",instruccion,letrasM);
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


       printf("%2d  %2d  %2d  %2d  %2d  %2d    %2s     %2s  %2s     Correcto\n",ID,PC,Ax,Bx,Cx,Dx,archivo, instruccion,letrasM);

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
        //return;
    }
    
   // printf("Tokens del archivo %s\n", archivo);

    char linea[256];
    //'''''''Lee linea x linea'''''''''''''

    printf("___________________________________________________________________________\n");
    printf(" ID PC  Ax  Bx  Cx  Dx   PROCESO      IR            STATUS     \n");
    while (!feof(fp))
    {
    
        leer_instruccion(fp,archivo);               //lee la linea actual
        sig_linea++;                       //lee la siguiente linea
        
        
        
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

