#include <stdio.h>
#include <string.h>
//Bibliotecas personales

#include "interprete2.h"

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

    if (!VerificarE(archivo)) {                     //si no cumple va a mandar un mensaje que no es correcto
        return;                                    // no es asm y lo rompe 
    } else{
    
    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo: %s\n", archivo);
        return;
    }

    printf("Tokens del archivo %s\n", archivo);

    char linea[256];
    //'''''''Lee linea x linea'''''''''''''
    while (!feof(fp))
    {
        leer_instruccion(fp);               //lee la linea actualz
        sig_linea++;                       //lee la siguiente linea

    }
    

    fclose(fp);
}

}


